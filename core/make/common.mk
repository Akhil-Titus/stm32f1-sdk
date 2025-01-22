# Check to make sure that the required variables are set
ifndef DEVICE
    $(error Please set the required DEVICE variable in your makefile.)
endif

ifndef FLASH
    $(error Please set the required FLASH variable in your makefile.)
endif

ifeq ($(FLASHING_TOOL), stm32flash)
	ifndef FLASHING_SERIAL_PORT
		$(error Please set the required FLASHING_SERIAL_PORT variable in your makefile)
	endif
endif
	

# Standard values for (linked) STM32-base folders
# STM32_BASE_PATH   ?= ./STM32-base
# STM32_CUBE_PATH   ?= ./STM32-base-STM32Cube

# let base path be root directory
STM32_BASE_PATH   ?= .

# STM32-base sub-folders
BASE_LINKER   = $(STM32_BASE_PATH)/core/linker
BASE_MAKE     = $(STM32_BASE_PATH)/core/make
BASE_STARTUP  = $(STM32_BASE_PATH)/core/startup

# Standard values for project folders
BUILD_FOLDER ?= ./build
BIN_FOLDER ?= $(BUILD_FOLDER)/bin
OBJ_FOLDER ?= $(BUILD_FOLDER)/obj
SRC_FOLDER ?= ./src
INC_FOLDER ?= ./inc

# Determine the series folder name
include $(BASE_MAKE)/series-folder-name.mk

# Include the series-specific makefile
include $(BASE_MAKE)/$(SERIES_FOLDER)/common.mk
MAPPED_DEVICE ?= $(DEVICE)

# Default flashing tool is st-flash
FLASHING_TOOL ?= st-flash

# The toolchain path, defaults to using the globally installed toolchain
ifdef TOOLCHAIN_PATH
    TOOLCHAIN_SEPARATOR = /
endif

TOOLCHAIN_PATH      ?=
TOOLCHAIN_SEPARATOR ?=
TOOLCHAIN_PREFIX    ?= arm-none-eabi-

CC      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)gcc
CXX     = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)g++
LD      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)ld -v
AR      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)ar
AS      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)objdump
SIZE    = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)size

# Flags - Overall Options
CPPFLAGS += -specs=nosys.specs

# Flags - C Language Options
CFLAGS += -ffreestanding

# Flags - C++ Language Options
CXXFLAGS += -fno-threadsafe-statics
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-unwind-tables

# Flags - Warning Options
CPPFLAGS += -Wall
CPPFLAGS += -Wextra

# Flags - Debugging Options
CPPFLAGS += -g

# Flags - Optimization Options
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections

# Flags - Preprocessor options
CPPFLAGS += -D $(MAPPED_DEVICE)

# Flags - Assembler Options
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

# Flags - Linker Options
# CPPFLAGS += -nostdlib
CPPFLAGS += -Wl,-L$(BASE_LINKER),-T$(BASE_LINKER)/$(SERIES_FOLDER)/$(DEVICE).ld

# Flags - Directory Options
CPPFLAGS += -I$(INC_FOLDER)
CPPFLAGS += -I$(BASE_STARTUP)

# Include component header files
CPPFLAGS += -I ./components/driver/gpio/include
CPPFLAGS += -I ./components/driver/i2c/include
CPPFLAGS += -I ./components/mpu6050/include

# Flags - Machine-dependant options
CPPFLAGS += -mcpu=$(SERIES_CPU)
CPPFLAGS += -march=$(SERIES_ARCH)
CPPFLAGS += -mlittle-endian
CPPFLAGS += -mthumb
CPPFLAGS += -masm-syntax-unified

# Input files
SRC ?=
SRC += $(SRC_FOLDER)/*.c

# Include component source files
SRC += ./components/driver/gpio/*.c
SRC += ./components/driver/i2c/*.c
SRC += ./components/mpu6050/src/*.c


# Output files
ELF_FILE_NAME ?= stm32_executable.elf
BIN_FILE_NAME ?= stm32_bin_image.bin
OBJ_FILE_NAME ?= startup_$(MAPPED_DEVICE).o

ELF_FILE_PATH = $(BIN_FOLDER)/$(ELF_FILE_NAME)
BIN_FILE_PATH = $(BIN_FOLDER)/$(BIN_FILE_NAME)
OBJ_FILE_PATH = $(OBJ_FOLDER)/$(OBJ_FILE_NAME)



# Startup file
DEVICE_STARTUP = $(BASE_STARTUP)/$(SERIES_FOLDER)/$(MAPPED_DEVICE).s

# Include the CMSIS files, using the HAL implies using the CMSIS
# ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
#     CPPFLAGS += -I$(STM32_CUBE_PATH)/CMSIS/ARM/inc
#     CPPFLAGS += -I$(STM32_CUBE_PATH)/CMSIS/$(SERIES_FOLDER)/inc

#     SRC += $(STM32_CUBE_PATH)/CMSIS/$(SERIES_FOLDER)/src/*.c
# endif

# # Include the HAL files
# ifdef USE_ST_HAL
#     CPPFLAGS += -D USE_HAL_DRIVER
#     CPPFLAGS += -I$(STM32_CUBE_PATH)/HAL/$(SERIES_FOLDER)/inc

#     # A simply expanded variable is used here to perform the find command only once.
#     HAL_SRC := $(shell find $(STM32_CUBE_PATH)/HAL/$(SERIES_FOLDER)/src/*.c ! -name '*_template.c')
#     SRC += $(HAL_SRC)
# endif

CPPFLAGS += -I./drivers/CMSIS/Include
CPPFLAGS += -I./drivers/CMSIS/Device/ST/$(SERIES_FOLDER)/Include

# Make all
all:$(BIN_FILE_PATH)

$(BIN_FILE_PATH): $(ELF_FILE_PATH)
	$(OBJCOPY) -O binary $^ $@

$(ELF_FILE_PATH): $(SRC) $(OBJ_FILE_PATH) | $(BIN_FOLDER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(OBJ_FILE_PATH): $(DEVICE_STARTUP) | $(OBJ_FOLDER)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BIN_FOLDER):
	mkdir $(BIN_FOLDER)

$(OBJ_FOLDER):
	mkdir $(BUILD_FOLDER)
	mkdir $(OBJ_FOLDER)

# Make clean
clean:
	rm -f $(ELF_FILE_PATH)
	rm -f $(BIN_FILE_PATH)
	rm -f $(OBJ_FILE_PATH)

# Make flash
flash:
ifeq ($(FLASHING_TOOL), st-flash)
	st-flash write $(BIN_FOLDER)/$(BIN_FILE_NAME) $(FLASH)
else ifeq ($(FLASHING_TOOL), stm32flash)
	stm32flash -w $(BIN_FOLDER)/$(BIN_FILE_NAME) -v -R $(FLASHING_SERIAL_PORT)
else
	$(error $(FLASHING_TOOL) not recognized)
endif

.PHONY: all clean flash
