/*
    eMAKER MMC5603NJ driver
    Copyright (C) 2023-2024  jmgiacalone for eMAKER Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef MMC5603NJ_H

#define MMC5603NJ_H

#include <Arduino.h>
#include <Wire.h>

// I2C Address
#define MMC5603NJ_DEFAULT_ADDRESS    (0x30)
#define MMC5603NJ_WHO_AM_I              16
#define MMC5603NJ_REG_ID             (0x39)

//Registers

//Config
#define MMC5603NJ_REG_ODR       (0x1A)
#define MMC5603NJ_REG_CONF0     (0x1B)
#define MMC5603NJ_REG_CONF1     (0x1C)
#define MMC5603NJ_REG_CONF2     (0x1D)

#define MMC5603NJ_REG_CONF0_MEASM 0b00000001
#define MMC5603NJ_REG_CONF0_MEAST 0b00000010
#define MMC5603NJ_REG_STATUS_MEASM 6
#define MMC5603NJ_REG_STATUS_MEAST 7


//Data
#define MMC5603NJ_REG_XOUT0     (0x00)
#define MMC5603NJ_REG_XOUT1     (0x01)
#define MMC5603NJ_REG_XOUT2     (0x06)
#define MMC5603NJ_REG_YOUT0     (0x02)
#define MMC5603NJ_REG_YOUT1     (0x03)
#define MMC5603NJ_REG_YOUT2     (0x07)
#define MMC5603NJ_REG_ZOUT0     (0x04)
#define MMC5603NJ_REG_ZOUT1     (0x05)
#define MMC5603NJ_REG_ZOUT2     (0x08)
#define MMC5603NJ_REG_TOUT      (0x09)
#define MMC5603NJ_REG_STATUS1   (0x18)


typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} magSensorXYZ;

typedef struct {
    float x;
    float y;
    float z;
} magSensorFloatXYZ;  

// Class

class MMC5603NJ {
    public:
        bool begin(uint8_t addr = MMC5603NJ_DEFAULT_ADDRESS, TwoWire *theWire = &Wire);
        uint8_t whoami();
        magSensorXYZ getRawMag();
        magSensorFloatXYZ getMag();
        uint8_t getRawTemp();
        float getTemp();
    private:
        TwoWire *_wire;
        uint8_t _addr;
        bool write(uint8_t reg, uint8_t *wbuf, uint8_t len);
        bool readRegister(uint8_t reg, uint8_t *rbuf, uint8_t len = 1);
};
#endif

