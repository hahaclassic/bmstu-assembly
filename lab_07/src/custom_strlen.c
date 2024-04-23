#include "custom_strlen.h"

size_t custom_strlen(const char *str)         
{
    size_t len = 0;

    __asm__(
        ".intel_syntax noprefix\n\t"   
        "mov rcx, -1\n\t"              
        "mov rdi, %1\n\t"    
        "cmp rdi, 0\n\t"
        "je exit\n\t"           
        "mov al, 0\n\t"                 
        "repne scasb\n\t" // поиск байта, равного al; сдвиг вправа (увеличение адреса), т.к. DF = 0           
        "neg rcx\n\t"
        "sub rcx, 2\n\t"  // Вычитаем нулевой символ и единицу, полученную в результате перевода в доп код              
        "mov %0, rcx\n\t" 
        "exit: \n\t" 
        "mov rcx, 0\n\t"           
        : "=r" (len)                  
        : "r" (str)                  
        : "rcx", "rdi", "al"           
    );                             

    return len;
}
