CC := gcc

OUT_DIR := ./out
INC_DIR := ./inc
SRC_DIR := ./src
ASM_DIR := ./asm

CFLAGS :=  -masm=intel -std=c99 -Wall -Wextra -Werror -Wpedantic -I $(INC_DIR)

CFILES := $(wildcard $(SRC_DIR)/*.c)

OBJFILES := $(CFILES:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)

ASM_FILES := $(CFILES:$(SRC_DIR)/%.c=$(ASM_DIR)/%.asm)

# Функции
app.exe: $(OUT_DIR) $(ASM_DIR) $(OBJFILES)
	nasm -f elf64 $(SRC_DIR)/copy_string.asm -o $(OUT_DIR)/copy_string.o
	$(CC) $(OBJFILES) $(OUT_DIR)/copy_string.o -o $@

$(OUT_DIR):
	@mkdir -p $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

assembly: $(ASM_DIR) $(ASM_FILES)
	@echo ".c files converted to assembly."

disassembly: $(ASM_DIR) $(OBJFILES)
	objdump -D -S $(OUT_DIR)/main.o > $(ASM_DIR)/main.d
	objdump -D -S $(OUT_DIR)/copy_string.o > $(ASM_DIR)/copy_string.d
	objdump -D -S $(OUT_DIR)/custom_strlen.o > $(ASM_DIR)/custom_strlen.d

$(ASM_DIR):
	@mkdir -p $@

$(ASM_DIR)/%.asm: $(SRC_DIR)/%.c
	$(CC) -S $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUT_DIR)/*
	rm -rf $(ASM_DIR)/*
	rm -f ./app.exe 