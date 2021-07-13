#include <ArduinoBLE.h>       // Bluetooth Low Energy
#include <Arduino_LSM9DS1.h>  // Accelerometer, Gyroscope, Magnetometer
#include <Arduino_HTS221.h>   // Thermometer, Humidity meter 

// define used pins 
#define MOTOR_PWM_A    P0_4
#define MOTOR_PWM_B    P0_5
#define MOTOR_IN_1_A   P1_11
#define MOTOR_IN_2_A   P1_12
#define MOTOR_IN_1_B   P1_15
#define MOTOR_IN_2_B   P1_13

#define ENCODER_INC_A  P1_14
#define ENCODER_DEC_A  P0_30
#define ENCODER_INC_B  P0_23
#define ENCODER_DEC_B  P0_29

#define STEPPER_IN1    P0_21
#define STEPPER_IN2    P0_27
#define STEPPER_IN3    P1_2
#define STEPPER_IN4    P0_31

#define LED_R          P0_3
#define LED_G          P0_28
#define LED_B          P0_2

#define BATTERY_CONTROL   1
#define DRIVE_CONTROL     2
#define SPEED_CONTROL     3
#define DISTANCE_CONTROL  4
#define LIVE_CONTROL      5


// definition of variables
// variables used to reading
int   old_battery_level;      // percentages
float temperature,            // Celsius degrees
      humidity,               // percentages
      ac_x, ac_y, ac_z,       // g(meter/second^2)
      gy_x, gy_y, gy_z,       // degrees/second
      mag_x, mag_y, mag_z;    // micro Tesla(uT)
      
// variable used to checking current activity in app
int manage;

// variables used to time counting
long current_millis, previous_millis = 0;

// variables used to encoders reading
long pulse_encA_up = 0, pulse_encA_down = 0, pulse_encB_up = 0, pulse_encB_down = 0;

// variables used to stepper motor setting 
int steps_to_do = 4095;
int current_steps = 0;
bool stepper_direction = true;

// variables used to ... 


// BLE Services and Characteristices
BLEService ManageService("b3926802-cf6f-11eb-b8bc-0242ac130003");
BLEService HTSService("44214a98-c442-11eb-8529-0242ac130003");
BLEService IMUService("44214d04-c442-11eb-8529-0242ac130003");
BLEService DriveControlService("118ef412-ccef-11eb-b8bc-0242ac130003");
BLEService SpeedControlService("118ef66a-ccef-11eb-b8bc-0242ac130003");
BLEService DistanceControlService("118ef778-ccef-11eb-b8bc-0242ac130003");
BLEService LiveControlService("118ef872-ccef-11eb-b8bc-0242ac130003");

BLEShortCharacteristic manage_short("c0c2673e-cf6f-11eb-b8bc-0242ac130003", BLEWrite);

BLEShortCharacteristic battery_short("1ad84f14-c443-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEShortCharacteristic temperature_short("1ad8514e-c443-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEShortCharacteristic humidity_short("1ad85234-c443-11eb-8529-0242ac130003", BLERead | BLENotify);

BLEFloatCharacteristic ac_x_float("66b888d0-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic ac_y_float("66b88b00-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic ac_z_float("66b88bfa-c444-11eb-8529-0242ac130003", BLERead | BLENotify);

BLEFloatCharacteristic gy_x_float("66b88ccc-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic gy_y_float("66b88d94-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic gy_z_float("66b88f2e-c444-11eb-8529-0242ac130003", BLERead | BLENotify);

BLEFloatCharacteristic mag_x_float("66b89000-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic mag_y_float("66b890b4-c444-11eb-8529-0242ac130003", BLERead | BLENotify);
BLEFloatCharacteristic mag_z_float("66b89172-c444-11eb-8529-0242ac130003", BLERead | BLENotify);

BLEShortCharacteristic rgb1_short("47eb805c-ccef-11eb-b8bc-0242ac130003", BLEWrite);
BLEShortCharacteristic rgb2_short("47eb8296-ccef-11eb-b8bc-0242ac130003", BLEWrite);
BLEUnsignedIntCharacteristic joy1_unsigned_int("47eb8386-ccef-11eb-b8bc-0242ac130003", BLEWrite);
BLEUnsignedIntCharacteristic joy2_unsigned_int("47eb857a-ccef-11eb-b8bc-0242ac130003", BLEWrite);

BLEShortCharacteristic speed_short("e04bc9fe-cf3f-11eb-b8bc-0242ac130003", BLEWrite);
BLEShortCharacteristic direction_short("e04bcc74-cf3f-11eb-b8bc-0242ac130003", BLEWrite);

BLEStringCharacteristic route_string("e8331bc2-cf3f-11eb-b8bc-0242ac130003", BLEWrite, 36);
BLEShortCharacteristic abort_short("e8331df2-cf3f-11eb-b8bc-0242ac130003", BLEWrite);

BLEShortCharacteristic joy1_x_v2_short("edaee266-cf3f-11eb-b8bc-0242ac130003", BLEWrite);
BLEShortCharacteristic joy1_y_v2_short("edaee4dc-cf3f-11eb-b8bc-0242ac130003", BLEWrite);
BLEShortCharacteristic joy2_x_v2_short("edaee5c2-cf3f-11eb-b8bc-0242ac130003", BLEWrite);
BLEShortCharacteristic joy2_y_v2_short("edaee680-cf3f-11eb-b8bc-0242ac130003", BLEWrite);


// Initialization of all used sensors
// returns number of uninitiated devices
int sensor_initialization(){
    int result = 0;
    
    if(!HTS.begin()){
        Serial.println("Failed to initialize HTS221!");
        result++;
    }else{
        Serial.println("HTS221 initialized correctly!");
    }

    if(!IMU.begin()){
        Serial.println("Failed to start LSM9DS1!");
        result++;
    }else{
        Serial.println("LSM9DS1 initialized correctly!");
    }

    return result;
}

// Initialization of Bluetooth Low Energy 
// returns number of errors
int BLE_initialization(){

    if (!BLE.begin()){
        Serial.println("Failed to start BLE!");
        return 1;
    }else{
        Serial.println("BLE initialized correctly!");
    }

    BLE.setLocalName("CC13");

    BLE.setAdvertisedService(ManageService);
    BLE.setAdvertisedService(HTSService); 
    BLE.setAdvertisedService(IMUService);
    BLE.setAdvertisedService(DriveControlService);
    BLE.setAdvertisedService(SpeedControlService);
    BLE.setAdvertisedService(DistanceControlService);
    BLE.setAdvertisedService(LiveControlService);

    ManageService.addCharacteristic(manage_short);
     
    HTSService.addCharacteristic(battery_short); 
    HTSService.addCharacteristic(temperature_short); 
    HTSService.addCharacteristic(humidity_short); 

    IMUService.addCharacteristic(ac_x_float); 
    IMUService.addCharacteristic(ac_y_float); 
    IMUService.addCharacteristic(ac_z_float); 
    IMUService.addCharacteristic(gy_x_float); 
    IMUService.addCharacteristic(gy_y_float); 
    IMUService.addCharacteristic(gy_z_float); 
    IMUService.addCharacteristic(mag_x_float); 
    IMUService.addCharacteristic(mag_y_float); 
    IMUService.addCharacteristic(mag_z_float); 

    DriveControlService.addCharacteristic(rgb1_short);
    DriveControlService.addCharacteristic(rgb2_short);
    DriveControlService.addCharacteristic(joy1_unsigned_int);
    DriveControlService.addCharacteristic(joy2_unsigned_int);

    SpeedControlService.addCharacteristic(speed_short);
    SpeedControlService.addCharacteristic(direction_short);

    DistanceControlService.addCharacteristic(route_string);
    DistanceControlService.addCharacteristic(abort_short);

    LiveControlService.addCharacteristic(joy1_x_v2_short);
    LiveControlService.addCharacteristic(joy1_y_v2_short);
    LiveControlService.addCharacteristic(joy2_x_v2_short);
    LiveControlService.addCharacteristic(joy2_y_v2_short);
    
    BLE.addService(ManageService);
    BLE.addService(HTSService); 
    BLE.addService(IMUService); 
    BLE.addService(DriveControlService);
    BLE.addService(SpeedControlService);
    BLE.addService(DistanceControlService);
    BLE.addService(LiveControlService);
    
    battery_short.writeValue(67);
    temperature_short.writeValue(23);
    humidity_short.writeValue(44);
    ac_x_float.writeValue(0.0);
    ac_y_float.writeValue(0.0);
    ac_z_float.writeValue(0.0);
    gy_x_float.writeValue(0.0);
    gy_y_float.writeValue(0.0);
    gy_z_float.writeValue(0.0);
    mag_x_float.writeValue(0.0);
    mag_y_float.writeValue(0.0);
    mag_z_float.writeValue(0.0);

    BLE.advertise();
    
    return 0;
}

// reads voltage and maps it on range 0-100%
void read_battery_level(){
    int battery = analogRead(A0);
    int battery_level = map(battery, 0, 1023, 0, 100);

    if(battery_level != old_battery_level){     
        battery_short.writeValue(battery_level); 
        old_battery_level = battery_level;       
    }
}

// reads values from HTS sensor
void readHTS(){
    temperature = HTS.readTemperature();
    humidity    = HTS.readHumidity();

    temperature_short.writeValue((short)temperature);
    humidity_short.writeValue((short)humidity);
}

// reads values from IMU sensor
void read_IMU(){
    if(IMU.accelerationAvailable()){
        IMU.readAcceleration(ac_x, ac_y, ac_z);
    }
    ac_x_float.writeValue(ac_x);
    ac_y_float.writeValue(ac_y);
    ac_z_float.writeValue(ac_z);

    if(IMU.gyroscopeAvailable()){
        IMU.readGyroscope(gy_x, gy_y, gy_z);
    }
    gy_x_float.writeValue(gy_x);
    gy_y_float.writeValue(gy_y);
    gy_z_float.writeValue(gy_z);

    if(IMU.magneticFieldAvailable()){
        IMU.readMagneticField(mag_x, mag_y, mag_z); 
    }    
    mag_x_float.writeValue(mag_x);
    mag_y_float.writeValue(mag_y);
    mag_z_float.writeValue(mag_z);
}

// converts byte array value to long long value
long long byte_array_to_long_long(long long x){
    // int m = ( (2<<7) - 1);
    int m = 255;
    long long result = 0;
    for(int i = 0; i<4; i++)
    {
        result <<= 8;
        result += (x&m);
        x >>= 8;
    }
    return result;
}

// functions used in encoder interrupts 
void encoderup_A(){
    pulse_encA_up++;
}

void encoderdown_A(){
    pulse_encA_down++;
}

void encoderup_B(){
    pulse_encB_up++;
}

void encoderdown_B(){
    pulse_encB_down++;
}

// sets work-parameter for stepper motor
void set_stepper(){
    if(stepper_direction == 1){
        current_steps++;}
    if(stepper_direction == 0){
        current_steps--;}
    if(current_steps>7){
        current_steps=0;}
    if(current_steps<0){
        current_steps=7;}
}

// sets an appropriate output signal for the stepper motor
void stepper(int xw){
    for (int x=0;x<xw;x++){
      switch(current_steps){
         case 0:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, HIGH);
         break; 
         case 1:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, HIGH);
           digitalWrite(STEPPER_IN4, HIGH);
         break; 
         case 2:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, HIGH);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
         case 3:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, HIGH);
           digitalWrite(STEPPER_IN3, HIGH);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
         case 4:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, HIGH);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
         case 5:
           digitalWrite(STEPPER_IN1, HIGH); 
           digitalWrite(STEPPER_IN2, HIGH);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
           case 6:
           digitalWrite(STEPPER_IN1, HIGH); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
         case 7:
           digitalWrite(STEPPER_IN1, HIGH); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, HIGH);
         break; 
         default:
           digitalWrite(STEPPER_IN1, LOW); 
           digitalWrite(STEPPER_IN2, LOW);
           digitalWrite(STEPPER_IN3, LOW);
           digitalWrite(STEPPER_IN4, LOW);
         break; 
       }
       set_stepper();
    }
} 

void set_motors(){
  
}

void set_head(){
  
}

void set_leds(){
  
}

void change_speed(){

    Serial.println(speed_short.value());
    Serial.println(direction_short.value());
    Serial.println("");
}

void do_route(){
    String s = route_string.value();
    Serial.println(s);
 
}

void off_all(){
  
}

void setup(){
    Serial.begin(9600);  
    //while (!Serial);

    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(STEPPER_IN1, OUTPUT); 
    pinMode(STEPPER_IN2, OUTPUT); 
    pinMode(STEPPER_IN3, OUTPUT); 
    pinMode(STEPPER_IN4, OUTPUT); 

    pinMode(ENCODER_INC_A, INPUT_PULLUP);
    pinMode(ENCODER_DEC_A, INPUT_PULLUP);
    pinMode(ENCODER_INC_B, INPUT_PULLUP);
    pinMode(ENCODER_DEC_B, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_INC_A), encoderup_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_DEC_A), encoderdown_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_INC_B), encoderup_B, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_DEC_B), encoderdown_B, CHANGE);
  
    pinMode(MOTOR_PWM_A, OUTPUT);
    pinMode(MOTOR_IN_1_A, OUTPUT);
    pinMode(MOTOR_IN_2_A, OUTPUT);

    pinMode(MOTOR_PWM_B ,OUTPUT);
    pinMode(MOTOR_IN_1_B ,OUTPUT);
    pinMode(MOTOR_IN_2_B ,OUTPUT);

    digitalWrite(MOTOR_IN_1_A, LOW); 
    digitalWrite(MOTOR_IN_2_A, HIGH);

    digitalWrite(MOTOR_IN_1_B, HIGH); 
    digitalWrite(MOTOR_IN_2_B, LOW);
    
    if(sensor_initialization()>0){
        while(1);
    }
    if(BLE_initialization()>0){
        while(1);
    }

    manage = 0;
}

void loop(){
    BLEDevice central = BLE.central();
  
    if(central){
    digitalWrite(LED_BUILTIN, HIGH);
    
    while(central.connected()){
      
        current_millis = millis();
        if(current_millis - previous_millis >= 500){
            manage = manage_short.value();
            previous_millis = current_millis;
        }
        
        if(manage == BATTERY_CONTROL){
            //readBatteryLevel();
            //readHTS();
        }else if(manage == DRIVE_CONTROL){
            set_motors();
            set_head();
            set_leds();
        }else if(manage == SPEED_CONTROL){
            change_speed();
        }else if(manage == DISTANCE_CONTROL){
            do_route();
        }else if(manage == LIVE_CONTROL){
            read_IMU();
            set_motors();
            set_head();
        }
    }
    
    off_all();    

    digitalWrite(LED_BUILTIN, LOW); 
  }
}
