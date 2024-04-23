.386
EXTRN number: word
EXTRN new_line: near

PUBLIC print_signed_hex

DSEG SEGMENT PARA PUBLIC 'DATA' USE16
	output_hex_msg        db "Signed 1-byte hexadecimal number: ", '$'
DSEG ENDS

CSEG SEGMENT PARA PUBLIC 'CODE' USE16
	assume CS:CSEG, DS:DSEG

print_signed_hex proc near
	call new_line
	mov dx, OFFSET output_hex_msg           
	mov ah, 09h
	int 21h

	xor dx, dx
    mov bx, number                      
	cmp bl, 7Fh                           
    jna skip_sign

	mov ah, 02h
	mov dl, '-'
	int 21h                               
	neg bl                               

skip_sign:
	ror bl, 4                              
	mov cx, 2                                                        

print_hex_digits_loop:
	mov dl, bl                    
	and dl, 0Fh                    

	cmp dl, 9                      
	ja convert_to_letter

	add dl, '0'                    
	jmp print_hex_digit

	convert_to_letter:
		sub dl, 10
		add dl, 'A'                    

	print_hex_digit:
		mov ah, 02h
		int 21h                        

	rol bl, 4                      
	loop print_hex_digits_loop
    
	call new_line
    ret
print_signed_hex endp

CSEG ENDS
END
