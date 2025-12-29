#ifndef INA219_MIN_H
#define INA219_MIN_H

#include <stdint.h>

#define INA219_ADDR  0x40

void ina219_init(void);
uint16_t ina219_read_bus_volt(void);
int16_t ina219_read_shunt_volt(void);
int16_t ina219_read_current(void);
int16_t ina219_read_power(void);

#endif

