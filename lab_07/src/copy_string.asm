section .text

global __copy_string


; 1-й целочисленный параметр (dst) - в rdi
; 2-й целочисленный параметр (src) - в rsi
; 3-й целочисленный параметр (n) - в rdx
; возвращаемое значение - в rax (целочисленное или указатель)

__copy_string:
    mov rbx, rdi 
    cmp rdi, 0
    je exit
    cmp rsi, 0
    je exit
    cmp rdi, rsi
    je exit

    mov rbx, rdi 
    mov rcx, rdx
    
    cmp rdi, rsi 
    jb copy ; dst < src

    mov rax, rdi
    sub rax, rsi
    cmp rax, rcx
    ja copy ; dst - src > len
    
    ; dst > src && dst - src < len
    ; Копируем в обратном порядке, чтобы избежать перезаписи байтов src до их непосредственного считывания
    add rdi, rcx
    dec rdi
    add rsi, rcx
    dec rsi
    std         

copy:
    rep movsb    
    cld          
exit:
    mov rax, rbx   
    ret
