.386
EXTRN number: word
EXTRN new_line: near

PUBLIC print_unsigned_oct

DSEG SEGMENT PARA PUBLIC 'DATA' USE16
	output_oct_msg        db "Unsigned octal number: ", '$'
DSEG ENDS

CSEG SEGMENT PARA PUBLIC 'CODE' USE16
	assume CS:CSEG, DS:DSEG

print_unsigned_oct proc near
    call new_line
	mov dx, OFFSET output_oct_msg       
	mov ah, 09h
	int 21h

    mov bx, number                                                        
    mov ah, 02h  
    xor dx, dx

    ; Обрабатываем первый бит, не являющийся знаковым
    rol bx, 1   ; сдвигаем bx на 1 разряд влево через флаг CF
	adc dl, '0' ; сложение число1 + число2 + CF                
    int 21h 

    ror bx, 13
    mov cx, 5     

print_oct_digits_loop:
    mov dl, bl                   
    and dl, 07h                    

    add dl, '0'                  
    int 21h         
    
    rol bx, 3                          
    loop print_oct_digits_loop
    
	call new_line
    ret
print_unsigned_oct endp

CSEG ENDS
END
