#include "ina219_min.h"
#include "TinyWireM.h"

#define INA219_SHUNT_OHMS 0.1 

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
       - 320mV shunt
       - 12-bit ADC
       - Continuous bus + shunt
    */
    ina219_write16(0x00, 0x399F);
}

uint16_t ina219_read_bus_volt(void)
{
    uint16_t raw = ina219_read16(0x02);
    raw >>= 3;              // lower 3 bits are flags
    return raw * 4;         // 4 mV per bit
}

int16_t ina219_read_shunt_volt(void)
{
    int16_t raw = (int16_t)ina219_read16(0x01);
    // 10uV per bit for default 12-bit ADC
    return raw * 10 / 1000; // convert to mV
}

// Current in mA
int16_t ina219_read_current(void)
{
    int16_t vshunt = ina219_read_shunt_volt();
    return (vshunt) / INA219_SHUNT_OHMS;
}

int16_t ina219_read_power(void)
{
    int16_t power = (ina219_read_bus_volt() * ina219_read_current())/1000;
    return power;  
}
