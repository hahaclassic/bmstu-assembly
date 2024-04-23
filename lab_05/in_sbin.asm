.386
EXTRN space: near
EXTRN new_line: near

PUBLIC number 
PUBLIC read_binary

DSEG SEGMENT PARA PUBLIC 'DATA' USE16
    number  dw 0
    sign db 0
    enter_msg  db "Enter signed binary: $"
    error_msg db "[ERR]: Invalid number. $"
DSEG ENDS

CSEG SEGMENT PARA PUBLIC 'CODE' USE16
    ASSUME CS:CSEG, DS:DSEG 

read_binary proc near
    mov ax, DSEG
    mov ds, ax
    call new_line

    mov ah, 9
    mov dx, offset enter_msg
    int 21h

    xor bx, bx 
    mov sign, 0
    mov ah, 01h

read_first_symbols: ; Пропускаем незначащие нули в начале и/или устанавливаем знак
    int 21h
    cmp al, '-'
    je set_negative

    cmp al, '+'
    je set_positive

    cmp al, '0'
    je read_first_symbols

    cmp al, 13
    je exit_success
    cmp al, '1'
    ja exit_failure
    cmp al, '0'
    jb exit_failure  

    sub al, '0'
    shl bx, 1 ; Сдвигаем число на 1 разряд
    add bl, al 


    mov cx, 14 ; Максимальное кол-во разрядов двоичного знакового числа - 1 (один разряд считан)

read_significant_digit: ; Читаем значащие цифры (на момент достижения метки первая цифра считана)
    int 21h

    jmp check_digit

    add_digit:
        sub al, '0'
        shl bx, 1 ; Сдвигаем число на 1 разряд
        add bl, al

    loop read_significant_digit

exit_success:
    mov al, sign
    cmp al, 1
    jne save_number

    NEG bx

save_number:
    mov number, bx
    ret

exit_failure:
    call new_line
    mov dx, offset error_msg
    mov ah, 09h
    int 21h
    call new_line
    ret

set_negative:
    mov sign, 1
    jmp read_first_symbols

set_positive:
    mov sign, 0
    jmp read_first_symbols

check_digit:
    cmp al, 13
    je exit_success

    cmp al, '1'
    ja exit_failure
    cmp al, '0'
    jb exit_failure 

    jmp add_digit

read_binary endp

CSEG ENDS
END
