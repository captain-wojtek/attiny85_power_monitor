#ifndef INA219_MIN_H
#define INA219_MIN_H

#include <stdint.h>

#define INA219_ADDR  0x40

void ina219_init(void);
float ina219_read_bus_volt(void);
float ina219_read_shunt_volt(void);
float ina219_read_current(void);
float ina219_read_power(void);

#endif

