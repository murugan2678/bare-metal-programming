PROJ_NAME = stm32f401ccux
CC = arm-none-eabi-gcc
SRC = startup_stm32f401ccux.s
SRC += main.c
LD_SCRIPT = STM32F401CCUX_FLASH.ld
ELF_FILE = $(PROJ_NAME).elf
BIN_FILE = $(PROJ_NAME).bin
HEX_FILE = $(PROJ_NAME).hex
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m4
CFLAGS += -T"STM32F401CCUX_FLASH.ld"
CFLAGS += --specs=nosys.specs -Wl,-Map="stm32f401ccux.map"
CFLAGS += -Wl,--gc-sections -static
CFLAGS += --specs=nano.specs
CFLAGS += -mfpu=fpv4-sp-d16     
CFLAGS += -mfloat-abi=hard 
CFLAGS += -mthumb 
CFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
CFLAGS += -g
#CFLAGS += --save-temps

sample.elf: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o  ${ELF_FILE}
	$(OBJCOPY) -O ihex $(ELF_FILE) $(HEX_FILE)
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(BIN_FILE)

.PHONEY:
	clean

clean:
	rm *.elf *.bin *.hex *.map

