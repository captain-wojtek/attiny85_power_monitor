#include "ina219_min.h"
#include "TinyWireM.h"

#define config_reg      0x00
#define shunt_reg       0x01
#define bus_reg         0x02
#define power_reg       0x03
#define current_reg     0x04
#define calibration_reg 0x05

#define R_shunt         0.1 
#define max_current     2          //amps
#define bus_lsb         0.004f     //V
#define shunt_lsb       0.00001f   //V

float current_lsb = (max_current/32768.0f);
float power_lsb = current_lsb * 20;
uint16_t calib_value = (uint16_t)(((0.04096f)/(current_lsb * R_shunt)) + 0.5f);

static void ina219_write16(uint8_t reg, uint16_t value)
{
    TinyWireM.beginTransmission(INA219_ADDR);
    TinyWireM.send(reg);
    TinyWireM.send((uint8_t)(value >> 8));
    TinyWireM.send((uint8_t)(value & 0xFF));
    TinyWireM.endTransmission();
}

static uint16_t ina219_read16(uint8_t reg)
{
    uint16_t val;

    TinyWireM.beginTransmission(INA219_ADDR);
    TinyWireM.send(reg);
    TinyWireM.endTransmission();

    TinyWireM.requestFrom(INA219_ADDR, (uint8_t)2);
    val  = ((uint16_t)TinyWireM.receive()) << 8;
    val |= TinyWireM.receive();

    return val;
}

void ina219_init(void)
{
    /* Configuration:
       - 32V range
       - +-320mV shunt range
       - 12-bit ADC
       - Shunt + Bus, continuous
    */
    
    ina219_write16(config_reg, 0x399F);
    ina219_write16(calibration_reg, calib_value);
}

float ina219_read_bus_volt(void)
{
    uint16_t raw = ina219_read16(bus_reg);
    raw >>= 3;              // lower 3 bits are flags
    return raw * bus_lsb;         
}

float ina219_read_shunt_volt(void)
{
    int16_t raw = (int16_t)ina219_read16(shunt_reg);
    return raw * shunt_lsb; 
}

float ina219_read_current(void)
{
    int16_t raw = (int16_t)ina219_read16(current_reg);
    return raw * current_lsb;
}

float ina219_read_power(void)
{
    uint16_t raw = ina219_read16(power_reg);
    return raw * power_lsb;  
}

int16_t ina219_read_calib(void)
{
    int16_t raw = ina219_read16(calibration_reg);
    return raw;
}
