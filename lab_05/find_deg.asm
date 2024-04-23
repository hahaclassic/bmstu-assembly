.386
EXTRN number: word
EXTRN new_line: near

PUBLIC find_degree

DSEG SEGMENT PARA PUBLIC 'DATA' USE16
	output_bin_degree_msg       db "Minimum degree of 2: ", '$'
DSEG ENDS

CSEG SEGMENT PARA PUBLIC 'CODE' USE16
    ASSUME CS:CSEG, DS:DSEG

find_degree proc near
    call new_line
    mov dx, offset output_bin_degree_msg
    mov ah, 09h
    int 21h

    mov bx, number
    cmp bx, 8000h
    jae max_num

    mov bx, 1
    xor cl, cl
    
next_degree:
    cmp bx, number
    ja exit

    inc cl
    shl bx, 1
    jmp next_degree

exit:
    mov ah, 02h
    cmp cl, 9
    jb skip_first_digit

    sub cl, 10
    mov dl, '1'
    int 21h

skip_first_digit:
    mov dl, cl
    add dl, '0'
    int 21h
    call new_line
    ret

max_num:
    mov cl, 16
    jmp exit

find_degree endp

CSEG ENDS
END
