CC = arm-none-eabi-gcc

SRC_D = src
OBJ_D = obj
INC_D = inc
DEBUG_D = debug
STARTUP_D = startup

SRC  = $(wildcard $(SRC_D)/*.c)
SRC += $(wildcard $(STARTUP_D)/*.c)
OBJ := $(patsubst $(SRC_D)/%.c, $(SRC_D)/$(OBJ_D)/%.o, $(SRC))
OBJ := $(patsubst $(STARTUP_D)/%.c, $(SRC_D)/$(OBJ_D)/%.o, $(OBJ))
LS   = $(wildcard $(STARTUP_D)/*.ld)

MARCH = cortex-m4

CFLAGS = -g -Wall -Wextra -mcpu=$(MARCH) -mthumb  -std=gnu11 -I./$(INC_D)
LFLAGS = -nostdlib -T $(LS) -Wl,-Map=$(DEBUG_D)/main.map

TARGET = $(DEBUG_D)/main.elf

all: $(OBJ) $(TARGET)

$(SRC_D)/$(OBJ_D)/%.o : $(SRC_D)/%.c | mk_obj_d
	$(CC) $(CFLAGS) -c -o $@ $^

$(SRC_D)/$(OBJ_D)/%.o : $(STARTUP_D)/%.c | mk_obj_d
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET) : $(OBJ) | mk_debug_d
	$(CC) $(LFLAGS) -o $@ $^

mk_obj_d:
	mkdir -p $(SRC_D)/$(OBJ_D)

mk_debug_d:
	mkdir -p $(DEBUG_D)

clean:
	rm -rf $(DEBUG_D) $(SRC_D)/$(OBJ_D)

.PHONY = mk_obj_d mk_debug_d clean

