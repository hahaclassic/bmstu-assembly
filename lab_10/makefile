load:
	sudo apt install libgtk-3-dev

LIBS := $(shell pkg-config --libs gtk+-3.0)

build:
	nasm -f elf64 -o main.o main.asm
	gcc -masm=intel -o find_root.o -c find_root.c 
	gcc main.o find_root.o $(LIBS) -L /usr/lib/x86_64-linux-gnu -o main.exe -no-pie
