.386
EXTRN read_binary: near
EXTRN print_unsigned_oct: near
EXTRN print_signed_hex: near
EXTRN find_degree: near
EXTRN number: byte

SSEG SEGMENT PARA STACK 'STACK' USE16
    db 200 dup (?)
SSEG ENDS

DSEG SEGMENT PARA PUBLIC 'DATA' USE16
    menu_text db "1. Enter signed binary number "
              db 10
              db 13
              db "2. Convert to unsigned 2-byte octal"
              db 10
              db 13
              db "3. Convert to signed 1-byte hexadecimal"
              db 10
              db 13
              db "4. Find minimum degree of two that greater that num"
              db 10
              db 13
              db "0. Exit" 
              db 10
              db 13
              db "Enter command: $"
    f_table     dw 4 DUP (0)
DSEG ENDS

CSEG SEGMENT PARA PUBLIC 'CODE' USE16
    ASSUME CS:CSEG, DS:DSEG, SS:SSEG

space proc near
    mov ah, 2
    mov dl, ' '
    int 21h
    ret
space endp

new_line proc near
    mov ah, 2
    mov dl, 10
	int 21h
    mov dl, 13
	int 21h
    ret
new_line endp

main:
    mov ax, DSEG
    mov ds, ax
    
    mov f_table[0], exit
    mov f_table[2], read_binary
    mov f_table[4], print_unsigned_oct
    mov f_table[6], print_signed_hex
    mov f_table[8], find_degree

    print_menu:
        mov ah, 9
        mov dx, offset menu_text
        int 21h
        mov ah, 1
        int 21h

        xor ah,ah
        sub al, '0'
        mov dl, 2
        mul dl
        mov bx, ax

        call new_line
        call f_table[bx]
        call new_line
        
        jmp print_menu

exit proc near
    mov ax, 4c00h
    int 21h
exit endp

CSEG ENDS
END main
