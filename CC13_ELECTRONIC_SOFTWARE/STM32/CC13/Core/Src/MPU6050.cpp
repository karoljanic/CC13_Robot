#include <MPU6050.h>
#include <cstdlib>
#include <cstring>


MPU6050::MPU6050() {
    devAddr = MPU6050_DEFAULT_ADDRESS;
    accelScale = 0x00;
    gyroScale = 0x00;
}

MPU6050::MPU6050(uint8_t address) {
    devAddr = address;
    accelScale = 0x00;
    gyroScale = 0x00;
}

void MPU6050::initialize() {
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    setFullScaleGyroscopeRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelerationRange(MPU6050_ACCEL_FS_2);
    setSleepEnabled(false);
}

bool MPU6050::testConnection() {
    return getDeviceID() == 0x34;
}

uint8_t MPU6050::getRate() {
    I2Cdev::readByte(devAddr, MPU6050_RA_SMPLRT_DIV, buffer);
    return buffer[0];
}

void MPU6050::setRate(uint8_t rate) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_SMPLRT_DIV, rate);
}

void MPU6050::setExternalFrameSync(uint8_t sync) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, sync);
}

void MPU6050::setDLPFMode(uint8_t mode) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

void MPU6050::setFullScaleGyroscopeRange(uint8_t range) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
    gyroScale = range;
}

void MPU6050::setFullScaleAccelerationRange(uint8_t range) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
    accelScale = range;
}

void MPU6050::setDHPFMode(uint8_t bandwidth) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_ACCEL_HPF_BIT, MPU6050_ACONFIG_ACCEL_HPF_LENGTH, bandwidth);
}

void MPU6050::setFreefallDetectionThreshold(uint8_t threshold) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_FF_THR, threshold);
}

void MPU6050::setFreefallDetectionDuration(uint8_t duration) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_FF_DUR, duration);
}

void MPU6050::setMotionDetectionThreshold(uint8_t threshold) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_MOT_THR, threshold);
}

void MPU6050::setMotionDetectionDuration(uint8_t duration) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_MOT_DUR, duration);
}

void MPU6050::setZeroMotionDetectionThreshold(uint8_t threshold) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_ZRMOT_THR, threshold);
}

void MPU6050::setZeroMotionDetectionDuration(uint8_t duration) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_ZRMOT_DUR, duration);
}

void MPU6050::setSlaveAddress(uint8_t num, uint8_t address) {
    if (num > 3) return;
    I2Cdev::writeByte(devAddr, MPU6050_RA_I2C_SLV0_ADDR + num*3, address);
}

uint8_t MPU6050::getIntStatus() {
    I2Cdev::readByte(devAddr, MPU6050_RA_INT_STATUS, buffer);
    return buffer[0];
}

void MPU6050::setIntEnabled(uint8_t enabled) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_INT_ENABLE, enabled);
}

void MPU6050::getAcceleration(Vector<int16_t>* v) {
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
    v -> x = (((int16_t)buffer[0]) << 8) | buffer[1];
    v -> y = (((int16_t)buffer[2]) << 8) | buffer[3];
    v -> z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050::getAccelerationFloat(Vector<float>* v) {
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
    switch(accelScale){
    case 0x00:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/8192.0f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/8192.0f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/8192.0f;
    	break;
    case 0x01:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/4096.0f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/4096.0f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/4096.0f;
    	break;
    case 0x02:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/2048.0f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/2048.0f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/2048.0f;
    	 break;
    case 0x03:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/1024.0f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/1024.0f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/1024.0f;
    	break;
    }
}

int16_t MPU6050::getAccelerationX() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050::getAccelerationY() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_YOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050::getAccelerationZ() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050::getTemperature() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_TEMP_OUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void MPU6050::getRotation(Vector<int16_t>* v) {
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
    v -> x = (((int16_t)buffer[0]) << 8) | buffer[1];
    v -> y = (((int16_t)buffer[2]) << 8) | buffer[3];
    v -> z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050::getRotationFloat(Vector<float>* v) {
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
    switch(gyroScale){
    case 0x00:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/131.0f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/131.0f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/131.0f;
    	break;
    case 0x01:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/65.5f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/65.5;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/65.5f;
    	break;
    case 0x02:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/32.8f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/32.8f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/32.8f;
    	break;
    case 0x03:
    	v -> x = (float)((((int16_t)buffer[0]) << 8) | buffer[1])/16.4f;
		v -> y = (float)((((int16_t)buffer[2]) << 8) | buffer[3])/16.4f;
		v -> z = (float)((((int16_t)buffer[4]) << 8) | buffer[5])/16.4f;
    	break;
    }
}


int16_t MPU6050::getRotationX() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050::getRotationY() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_YOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

int16_t MPU6050::getRotationZ() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_GYRO_ZOUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

bool MPU6050::getZeroMotionDetected() {
    I2Cdev::readBit(devAddr, MPU6050_RA_MOT_DETECT_STATUS, MPU6050_MOTION_MOT_ZRMOT_BIT, buffer);
    return buffer[0];
}

void MPU6050::reset() {
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, true);
}

void MPU6050::resetFIFO() {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_RESET_BIT, true);
}

void MPU6050::resetI2CMaster() {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_RESET_BIT, true);
}

void MPU6050::resetSensors() {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_SIG_COND_RESET_BIT, true);
}

void MPU6050::setSleepEnabled(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

void MPU6050::setFIFOEnabled(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, enabled);
}

void MPU6050::setI2CMasterModeEnabled(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

void MPU6050::setClockSource(uint8_t source) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void MPU6050::setOTPBankValid(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OTP_BNK_VLD_BIT, enabled);
}

uint16_t MPU6050::getFIFOCount() {
    I2Cdev::readBytes(devAddr, MPU6050_RA_FIFO_COUNTH, 2, buffer);
    return (((uint16_t)buffer[0]) << 8) | buffer[1];
}

uint8_t MPU6050::getFIFOByte() {
    I2Cdev::readByte(devAddr, MPU6050_RA_FIFO_R_W, buffer);
    return buffer[0];
}

void MPU6050::getFIFOBytes(uint8_t *data, uint8_t length) {
    if(length > 0){
        I2Cdev::readBytes(devAddr, MPU6050_RA_FIFO_R_W, length, data);
    } else {
    	*data = 0;
    }
}

void MPU6050::setFIFOByte(uint8_t data) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_FIFO_R_W, data);
}

uint8_t MPU6050::getDeviceID() {
    I2Cdev::readBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
    return buffer[0];
}

void MPU6050::setDeviceID(uint8_t id) {
    I2Cdev::writeBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id);
}

void MPU6050::setXAccelerationOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_XA_OFFS_H, offset);
}

void MPU6050::setYAccelerationOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_YA_OFFS_H, offset);
}

void MPU6050::setZAccelerationOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_ZA_OFFS_H, offset);
}

void MPU6050::setXGyroscopeOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_XG_OFFS_USRH, offset);
}

void MPU6050::setYGyroscopeOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_YG_OFFS_USRH, offset);
}

void MPU6050::setZGyroscopeOffset(int16_t offset) {
    I2Cdev::writeWord(devAddr, MPU6050_RA_ZG_OFFS_USRH, offset);
}

bool MPU6050::getIntDMPStatus() {
    I2Cdev::readBit(devAddr, MPU6050_RA_INT_STATUS, MPU6050_INTERRUPT_DMP_INT_BIT, buffer);
    return buffer[0];
}

void MPU6050::setDMPEnabled(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_EN_BIT, enabled);
}

void MPU6050::resetDMP() {
    I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_RESET_BIT, true);
}

void MPU6050::setMemoryBank(uint8_t bank, bool prefetchEnabled, bool userBank) {
    bank &= 0x1F;
    if (userBank) bank |= 0x20;
    if (prefetchEnabled) bank |= 0x40;
    I2Cdev::writeByte(devAddr, MPU6050_RA_BANK_SEL, bank);
}

void MPU6050::setMemoryStartAddress(uint8_t address) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_MEM_START_ADDR, address);
}

uint8_t MPU6050::readMemoryByte() {
    I2Cdev::readByte(devAddr, MPU6050_RA_MEM_R_W, buffer);
    return buffer[0];
}

void MPU6050::writeMemoryByte(uint8_t data) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_MEM_R_W, data);
}

void MPU6050::readMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address) {
    setMemoryBank(bank);
    setMemoryStartAddress(address);
    uint8_t chunkSize;
    for (uint16_t i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;

        // read the chunk of data as specified
        I2Cdev::readBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, data + i);

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
}

bool MPU6050::writeMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify) {
    setMemoryBank(bank);
    setMemoryStartAddress(address);
    uint8_t chunkSize;
    uint8_t *verifyBuffer;
    uint8_t *progBuffer=0;
    uint16_t i;
    if (verify) verifyBuffer = (uint8_t *)malloc(MPU6050_DMP_MEMORY_CHUNK_SIZE);
    for (i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;

        // write the chunk of data as specified
        progBuffer = (uint8_t *)data + i;

        I2Cdev::writeBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, progBuffer);

        // verify data if needed
        if (verify && verifyBuffer) {
            setMemoryBank(bank);
            setMemoryStartAddress(address);
            I2Cdev::readBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, verifyBuffer);
            if (memcmp(progBuffer, verifyBuffer, chunkSize) != 0) {
                free(verifyBuffer);
                return false; // uh oh.
            }
        }

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
    if (verify) free(verifyBuffer);
    return true;
}
bool MPU6050::writeProgMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify) {
    return writeMemoryBlock(data, dataSize, bank, address, verify);
}

bool MPU6050::writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize) {
    uint8_t *progBuffer = 0;
	uint8_t success, special;
    uint16_t i;

    // config set data is a long string of blocks with the following structure:
    // [bank] [offset] [length] [byte[0], byte[1], ..., byte[length]]
    uint8_t bank, offset, length;
    for (i = 0; i < dataSize;) {
        bank = data[i++];
        offset = data[i++];
        length = data[i++];

        // write data or perform special action
        if (length > 0) {
            // regular block of data to write
            progBuffer = (uint8_t *)data + i;
            success = writeMemoryBlock(progBuffer, length, bank, offset, true);
            i += length;
        } else {
            special = data[i++];

            if (special == 0x01) {
                I2Cdev::writeByte(devAddr, MPU6050_RA_INT_ENABLE, 0x32);  // single operation
                success = true;
            } else {
                success = false;
            }
        }
        if (!success) {
            return false; // uh oh
        }
    }
    return true;
}

bool MPU6050::writeProgDMPConfigurationSet(const uint8_t *data, uint16_t dataSize) {
    return writeDMPConfigurationSet(data, dataSize);
}

uint8_t MPU6050::getDMPConfig1() {
    I2Cdev::readByte(devAddr, MPU6050_RA_DMP_CFG_1, buffer);
    return buffer[0];
}
void MPU6050::setDMPConfig1(uint8_t config) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_DMP_CFG_1, config);
}

uint8_t MPU6050::getDMPConfig2() {
    I2Cdev::readByte(devAddr, MPU6050_RA_DMP_CFG_2, buffer);
    return buffer[0];
}

void MPU6050::setDMPConfig2(uint8_t config) {
    I2Cdev::writeByte(devAddr, MPU6050_RA_DMP_CFG_2, config);
}

uint8_t MPU6050::DMPInitialize() {
    // reset device
    reset();
    // wait after reset
    HAL_Delay(30);

    // disable sleep mode
    setSleepEnabled(false);

    // get MPU hardware revision
    setMemoryBank(0x10, true, true);
    setMemoryStartAddress(0x06);
    setMemoryBank(0, false, false);

    // setup weird slave stuff (?)
    setSlaveAddress(0, 0x7F);
    setI2CMasterModeEnabled(false);
    setSlaveAddress(0, 0x68);
    resetI2CMaster();
    HAL_Delay(20);

    // load DMP code into memory banks
    if (writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) {

        // write DMP configuration
        if (writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE)) {

            setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
            setIntEnabled(0x12);
            setRate(4); // 1khz / (1 + 4) = 200 Hz
            setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);
            setDLPFMode(MPU6050_DLPF_BW_42);
            setFullScaleGyroscopeRange(MPU6050_GYRO_FS_2000);

            //write start address MSB into register
            setDMPConfig1(0x03);
            //write start address LSB into register
            setDMPConfig2(0x00);

            setOTPBankValid(false);

            uint8_t dmpUpdate[16], j;
            uint16_t pos = 0;

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            resetFIFO();

            uint16_t fifoCount = getFIFOCount();
            uint8_t fifoBuffer[128];

            getFIFOBytes(fifoBuffer, fifoCount);

            setMotionDetectionThreshold(2);
            setZeroMotionDetectionThreshold(156);
            setMotionDetectionDuration(80);
            setZeroMotionDetectionDuration(0);

            resetFIFO();

            setFIFOEnabled(true);
            setDMPEnabled(true);
            resetDMP();

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            while ((fifoCount = getFIFOCount()) < 3);

            getFIFOBytes(fifoBuffer, fifoCount);

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            while ((fifoCount = getFIFOCount()) < 3);

            getFIFOBytes(fifoBuffer, fifoCount);

            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            setDMPEnabled(true);

            dmpPacketSize = 42;

            resetFIFO();
            getIntStatus();
        } else {
            return 2; // configuration block loading failed
        }
    } else {
        return 1; // main binary block loading failed
    }
    return 0; // success
}

bool MPU6050::DMPPacketAvailable() {
    return getFIFOCount() >= DMPGetFIFOPacketSize();
}

uint8_t MPU6050::DMPGetAcceleration(Vector<int16_t> *v, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = (packet[28] << 8) | packet[29];
    v -> y = (packet[32] << 8) | packet[33];
    v -> z = (packet[36] << 8) | packet[37];
    return 0;
}

uint8_t MPU6050::DMPGetQuaternion(int16_t *data, const uint8_t* packet) {
    if (packet == 0){ packet = dmpPacketBuffer;}
    data[0] = ((packet[0] << 8) | packet[1]);
    data[1] = ((packet[4] << 8) | packet[5]);
    data[2] = ((packet[8] << 8) | packet[9]);
    data[3] = ((packet[12] << 8) | packet[13]);
    return 0;
}

uint8_t MPU6050::DMPGetQuaternion(Quaternion<float> *q, const uint8_t* packet) {
    int16_t qI[4];
    uint8_t status = DMPGetQuaternion(qI, packet);
    if (status == 0) {
        q -> w = (float)qI[0] / 16384.0f;
        q -> x = (float)qI[1] / 16384.0f;
        q -> y = (float)qI[2] / 16384.0f;
        q -> z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}

uint8_t MPU6050::DMPGetRotation(Vector<int16_t> *v, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = (packet[16] << 8) | packet[17];
    v -> y = (packet[20] << 8) | packet[21];
    v -> z = (packet[24] << 8) | packet[25];
    return 0;
}

uint8_t MPU6050::DMPGetLinearAcceleration(Vector<int16_t> *v, Vector<int16_t> *vRaw, Vector<float> *gravity) {
    // get rid of the gravity component (+1g = +8192 in standard DMP FIFO packet, sensitivity is 2g)
    v -> x = vRaw -> x - gravity -> x*8192;
    v -> y = vRaw -> y - gravity -> y*8192;
    v -> z = vRaw -> z - gravity -> z*8192;
    return 0;
}

uint8_t MPU6050::DMPGetGravity(Vector<float> *v, Quaternion<float> *q) {
    v -> x = 2 * (q -> x*q -> z - q -> w*q -> y);
    v -> y = 2 * (q -> w*q -> x + q -> y*q -> z);
    v -> z = q -> w*q -> w - q -> x*q -> x - q -> y*q -> y + q -> z*q -> z;
    return 0;
}

uint8_t MPU6050::DMPGetEuler(float *data, Quaternion<float> *q) {
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);   // psi
    data[1] = -asin(2*q -> x*q -> z + 2*q -> w*q -> y);                              // theta
    data[2] = atan2(2*q -> y*q -> z - 2*q -> w*q -> x, 2*q -> w*q -> w + 2*q -> z*q -> z - 1);   // phi
    return 0;
}

uint8_t MPU6050::DMPGetYawPitchRoll(float *data, Quaternion<float> *q, Vector<float> *gravity) {
    // yaw: (about Z axis)
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(gravity -> x / sqrt(gravity -> y*gravity -> y + gravity -> z*gravity -> z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(gravity -> y / sqrt(gravity -> x*gravity -> x + gravity -> z*gravity -> z));
    return 0;
}

uint8_t MPU6050::DMPProcessFIFOPacket(const unsigned char *dmpData) {
    return 0;
}

uint8_t MPU6050::DMPReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed) {
    uint8_t status;
    uint8_t buf[dmpPacketSize];
    for (uint8_t i = 0; i < numPackets; i++) {
        // read packet from FIFO
        getFIFOBytes(buf, dmpPacketSize);

        // process packet
        if ((status = DMPProcessFIFOPacket(buf)) > 0) return status;

        // increment external process count variable, if supplied
        if (processed != 0) (*processed)++;
    }
    return 0;
}

uint16_t MPU6050::DMPGetFIFOPacketSize() {
    return dmpPacketSize;
}
