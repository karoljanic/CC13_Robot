#include <LED.h>

//#######################################################
// class  DigitalLED_RGB
//#######################################################
void DigitalLED_RGB::On() {
	On_R();
	On_G();
	On_B();
}

void DigitalLED_RGB::On(bool r, bool g, bool b) {
	r ? On_R() : Off_R();
	g ? On_G() : Off_G();
	b ? On_B() : Off_B();
}

void DigitalLED_RGB::Off() {
	Off_R();
	Off_G();
	Off_B();
}

void DigitalLED_RGB::Toggle() {
	Toggle_R();
	Toggle_G();
	Toggle_B();
}



//#######################################################
// class  AnalogLED
//#######################################################

void AnalogLED::SetN(double x) {
	Set((uint32_t)(powf(x, 2.2) * period));
}

double AnalogLED::GetN() {
	return powf(((double)Get())/period, 1/2.2);
}


//#######################################################
// class  AnalogLED_RGB
//#######################################################

void AnalogLED_RGB::On() {
	On_R();
	On_G();
	On_B();
}

void AnalogLED_RGB::Off() {
	Off_R();
	Off_G();
	Off_B();
}

void AnalogLED_RGB::Set(uint32_t value_r, uint32_t value_g, uint32_t value_b) {
	Set_R(value_r);
	Set_G(value_g);
	Set_B(value_b);
}

void AnalogLED_RGB::Set(uint32_t value) {
	Set_R(value);
	Set_G(value);
	Set_B(value);
}

void AnalogLED_RGB::SetN(double r, double g, double b) {
	SetN_R(r);
	SetN_G(g);
	SetN_B(b);
}

void AnalogLED_RGB::SetN(double x) {
	SetN_R(x);
	SetN_G(x);
	SetN_B(x);
}

double AnalogLED_RGB::GetN_R() {
	return powf(((double) Get_R()) / period, 1 / 2.2);
}

double AnalogLED_RGB::GetN_G() {
	return powf(((double) Get_G()) / period, 1 / 2.2);
}

double AnalogLED_RGB::GetN_B() {
	return powf(((double) Get_B()) / period, 1 / 2.2);
}
