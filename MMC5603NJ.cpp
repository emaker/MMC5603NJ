#include <MMC5603NJ.h>

bool MMC5603NJ::begin(uint8_t addr, TwoWire *theWire) {
    _wire = theWire; 
    _addr = addr;
    uint8_t whoAmI = whoami();
    // Serial.println(whoAmI);
    if (whoAmI == MMC5603NJ_WHO_AM_I) {
        return true;
    } else {
        return false;
    }
}

uint8_t MMC5603NJ::whoami() {
    uint8_t buffer[1];
    if (readRegister(MMC5603NJ_REG_ID, buffer)) {
        return buffer[0];
    } 
    return -1;
}

void MMC5603NJ::setContinuousMode(uint8_t odr){
    uint8_t buf;
    write(MMC5603NJ_REG_ODR,&odr,1);
    buf = MMC5603NJ_REG_CONF0_FREQ | MMC5603NJ_REG_CONF0_SR;
    write(MMC5603NJ_REG_CONF0,&buf,1);
    buf = MMC5603NJ_REG_CONF1_BW12;
    write(MMC5603NJ_REG_CONF1,&buf,1);
    buf = MMC5603NJ_REG_CONF2_CMMEN;
    write(MMC5603NJ_REG_CONF2,&buf,1);
}

uint8_t MMC5603NJ::getRawTemp(){
    uint8_t out;
    //set temp trigger
    uint8_t conf = MMC5603NJ_REG_CONF0_MEAST;
    if(!write(MMC5603NJ_REG_CONF0,&conf,1)){
        return 127;
    }

    //check read done
    uint8_t status;

    while(!(status >> MMC5603NJ_REG_STATUS_MEAST)){
        readRegister(MMC5603NJ_REG_STATUS1,&status);
        delay(5);
    }

    //read temp
    if(readRegister(MMC5603NJ_REG_TOUT, &out)){
        return out;
    }
    return 127;

}

float MMC5603NJ::getTemp(){
    return (float)getRawTemp() * 0.8 - 75;
}

magSensorXYZ MMC5603NJ::getRawMag(){
    //...
    uint8_t rbuf[9];
    magSensorXYZ raw = {-9999,-9999,-9999};
    //set temp trigger
    uint8_t conf = MMC5603NJ_REG_CONF0_MEASM;
    if(!write(MMC5603NJ_REG_CONF0,&conf,1)){
        return raw;
    }

    //check read done
    uint8_t status = 0;
    uint8_t i = 0;

    while(!(status >> MMC5603NJ_REG_STATUS_MEASM)){
        readRegister(MMC5603NJ_REG_STATUS1,&status);
        if(i++>5){
            return raw;
        }
        delay(5);
    }

    //read Mag once
    if(readRegister(MMC5603NJ_REG_XOUT0, rbuf, 9)){
        //
        raw.x = (uint32_t)rbuf[0] << 12 | (uint32_t)rbuf[1] << 4 | rbuf[6] >> 4;
        raw.y = (uint32_t)rbuf[2] << 12 | (uint32_t)rbuf[3] << 4 | rbuf[7] >> 4;
        raw.z = (uint32_t)rbuf[4] << 12 | (uint32_t)rbuf[5] << 4 | rbuf[8] >> 4;

        raw.x -= (uint32_t)1 << 19;
        raw.y -= (uint32_t)1 << 19;
        raw.z -= (uint32_t)1 << 19;

    }

    return raw;

}

magSensorFloatXYZ MMC5603NJ::getMag(){
    magSensorXYZ raw = {0,0,0};
    magSensorFloatXYZ out = {0,0,0};
    raw = getRawMag();
    out.x = (float)raw.x * 0.00625;
    out.y = (float)raw.y * 0.00625;
    out.z = (float)raw.z * 0.00625;

    return out;

}

bool MMC5603NJ::write(uint8_t reg, uint8_t *buffer, uint8_t len) {
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    for(uint8_t i = 0; i < len; i++) {
        _wire->write(buffer[i]);
    }
    if (_wire->endTransmission() != 0) {
        return false;
    }
    return true;
}

bool MMC5603NJ::readRegister(uint8_t reg, uint8_t *buffer, uint8_t len) {
    uint8_t rx_bytes = 0;

    _wire->beginTransmission(_addr);
    _wire->write(reg);
    uint8_t err = _wire->endTransmission();
    if (err!= 0) {
        Serial.println(err);   
        return false;
    }
    rx_bytes = _wire->requestFrom(_addr, len);
    if (rx_bytes != len) {
        Serial.println("No data");
        return false;
    }    
    for (uint8_t i = 0; i < len; i++) {
        buffer[i] = _wire->read();
    }
    return true;

}