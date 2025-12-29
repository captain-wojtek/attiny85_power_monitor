MCU=attiny85
F_CPU=16500000UL

CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy

CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Ilib
CXXFLAGS=$(CFLAGS)
LDFLAGS=-mmcu=$(MCU)

TARGET=attiny85_battery_monitor

SRC_C = \
	lib/TinyWireM.c \
	lib/USI_TWI_Master.c

SRC_CPP = \
	attiny85_battery_monitor.cpp \
	lib/SSD1306_minimal.cpp \
	lib/ina219_min.cpp

OBJ = $(SRC_C:.c=.o) $(SRC_CPP:.cpp=.o)

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	@echo ">>> Plug in Digispark now..."
	micronucleus $(TARGET).hex

clean:
	rm -f $(OBJ) *.elf *.hex

