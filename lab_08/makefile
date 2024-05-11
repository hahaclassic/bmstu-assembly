SRC_DIR := ./src
OUT_DIR := ./out
ASM_DIR := ./assembly
DIS_DIR := ./disassembly

.PHONY: math sin root assembly disassembly clean

build-math: 
	g++ $(SRC_DIR)/test_math.cpp -mno-80387 -O0 -o test_math_no_m80387.exe
	g++ $(SRC_DIR)/test_math.cpp -m80387 -mno-sse -O0 -o test_math_m80387.exe
	g++ $(SRC_DIR)/test_math.cpp -masm=intel -DASM -O0 -o test_math_asm.exe

math:
	@echo
	@echo "no m80387: "
	./test_math_no_m80387.exe

	@echo
	@echo "m80387: "
	./test_math_m80387.exe

	@echo
	@echo "asm inserts: "
	./test_math_asm.exe


sin:
	@echo
	g++ $(SRC_DIR)/test_sin.cpp -masm=intel -O0 -o test_sin.exe
	./test_sin.exe

root:
	gcc $(SRC_DIR)/find_root.c -masm=intel -std=c99 -Wall -Wextra -Werror -Wpedantic -o find_root.exe

assembly: 
	mkdir -p $@
	g++ -S $(SRC_DIR)/test_math.cpp -mno-80387 -O0 -o $(ASM_DIR)/test_math_no_m80387.s
	g++ -S $(SRC_DIR)/test_math.cpp -m80387 -mno-sse -O0 -o $(ASM_DIR)/test_math_m80387.s
	g++ -S $(SRC_DIR)/test_math.cpp -masm=intel -DASM -O0  -o $(ASM_DIR)/test_math_asm.s
	g++ -S $(SRC_DIR)/test_sin.cpp -masm=intel -O0 -o $(ASM_DIR)/test_sin.s
	g++ -S $(SRC_DIR)/find_root.c -masm=intel -O0 -o $(ASM_DIR)/find_root.s

disassembly: $(OUT_DIR)
	mkdir -p $@
	g++ -c $(SRC_DIR)/test_math.cpp -mno-80387 -O0 -o $(OUT_DIR)/test_math_no_m80387.o
	g++ -c $(SRC_DIR)/test_math.cpp -m80387 -O0 -o $(OUT_DIR)/test_math_m80387.o
	g++ -c $(SRC_DIR)/test_math.cpp -masm=intel -DASM -O0 -o $(OUT_DIR)/test_math_asm.o
	g++ -c $(SRC_DIR)/test_sin.cpp -masm=intel -O0 -o $(OUT_DIR)/test_sin.o
	g++ -c $(SRC_DIR)/find_root.c -masm=intel -O0 -o $(OUT_DIR)/find_root.o

	objdump -M intel-mnemonic -d $(OUT_DIR)/test_math_no_m80387.o  > $(DIS_DIR)/test_math_no_m80387.txt
	objdump -M intel-mnemonic -d $(OUT_DIR)/test_math_m80387.o  > $(DIS_DIR)/test_math_m80387.txt
	objdump -M intel-mnemonic -d $(OUT_DIR)/test_math_asm.o  > $(DIS_DIR)/test_math_asm.txt
	objdump -M intel-mnemonic -d $(OUT_DIR)/test_sin.o  > $(DIS_DIR)/test_sin.txt
	objdump -M intel-mnemonic -d $(OUT_DIR)/find_root.o  > $(DIS_DIR)/find_root.txt

clean:
	rm -rf out/* assembly/* disassembly/* *.exe

$(OUT_DIR): 
	mkdir -p $@
