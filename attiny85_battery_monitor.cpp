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

void print_fixed_3(int32_t value)
{
    char buf[12];

    int32_t whole = value / 1000;
    int32_t frac  = value % 1000;
    if (frac < 0) frac = -frac;   // safety for negatives

    itoa(whole, buf, 10);
    ssd.printString(buf);
    ssd.printString(".");

    if (frac < 100) ssd.printString("0");
    if (frac < 10)  ssd.printString("0");

    itoa(frac, buf, 10);
    ssd.printString(buf);
}


int main(void)
{
    TinyWireM.begin();

    ssd.init(0x78);
    ssd.clear();

    ina219_init();

    while (1) {
        int32_t vbus = (int32_t)(ina219_read_bus_volt() * 1000.0f + 0.5f);
        int32_t vshunt = (int32_t)(ina219_read_shunt_volt() * 1000.0f + 0.5f);
        int32_t vbat = (int32_t)(vbus + vshunt);
        int32_t current = (int32_t)(ina219_read_current() * 1000.0f * 0.5f);
        int32_t power = (int32_t)(ina219_read_power() * 1000.0f * 0.5f) ;

        ssd.cursorTo(0, 0);
        ssd.printString("Volt:");
        print_fixed_3(vbat);
        ssd.printString(" V");

        ssd.cursorTo(0, 3);
        ssd.printString("Current:");
        print_fixed_3(current);
        ssd.printString(" A");

        ssd.cursorTo(0, 6);
        ssd.printString("Power:");
        print_fixed_3(power);
        ssd.printString(" W");

        _delay_ms(500);
    }
}

