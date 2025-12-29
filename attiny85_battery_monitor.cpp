#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lib/TinyWireM.h"
#include "lib/SSD1306_minimal.h"
#include "lib/ina219_min.h"

SSD1306_Mini ssd;

char buf_volt[12];
char buf_curr[12];
char buf_power[12];

int main(void)
{
    TinyWireM.begin();

    ssd.init(0x78);
    ssd.clear();

    ina219_init();

    while (1) {
        uint16_t vbus = ina219_read_bus_volt();
        int32_t vshunt = ina219_read_shunt_volt();
        int32_t vbat = vbus + vshunt;
        int32_t current = ina219_read_current();
        int32_t power = ina219_read_power();

        itoa(vbat, buf_volt, 10);
        itoa(current, buf_curr, 10);
        itoa(power, buf_power, 10);

        ssd.cursorTo(0, 0);
        ssd.printString("Volt:");
        ssd.printString(buf_volt);
        ssd.printString(" mV");

        ssd.cursorTo(0, 3);
        ssd.printString("Current:");
        ssd.printString(buf_curr);
        ssd.printString(" mA");

        ssd.cursorTo(0, 6);
        ssd.printString("Power:");
        ssd.printString(buf_power);
        ssd.printString(" mW");

        _delay_ms(500);
    }
}

