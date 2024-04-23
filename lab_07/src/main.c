#include <stdio.h>
#include <string.h>
#include "custom_strlen.h"

extern char *__copy_string(char *dst, const char *src, size_t n);


void test_len() 
{
    char str[] = "hello world";
    size_t n = strlen(str);
    printf("[LEN] standard c func: %zu\n", n);

    n = custom_strlen(str);
    printf("[LEN] asm func: %zu\n", n);

    n = custom_strlen(NULL);
    printf("[LEN] asm func: %zu\n", n);
}

void test_copy_default()
{
    char str1[] = "hello world";
    char str2[] = "my name is paul muaddib atreides";
    char *res = __copy_string(str2, str1, 11);
    printf("[COPY] Default: %s\n", res);
}

void test_copy_overlapping_dest_first()
{
    char str1[] = "hello world";
    char *res = __copy_string(str1, str1 + 3, 5);
    printf("[COPY] overlapping dest first: %s\n", res); 
}

void test_copy_overlapping_src_first()
{
    char str1[] = "hello world";
    char *res = __copy_string(str1 + 3, str1, 5);
    printf("[COPY] overlapping src first: %s\n", res); 
}

void test_copy_equal()
{
    char str1[] = "hello world";
    char *res = __copy_string(str1, str1, 5);
    printf("[COPY] equal ptrs: %s\n", res); 
}

void test_copy_null()
{
    char str1[] = "hello world";
    char *res = __copy_string(NULL, str1, 5);
    printf("[COPY] null dest: %s\n", res); 
}

int main()
{
    test_len();

    test_copy_default();
    
    test_copy_overlapping_dest_first();

    test_copy_overlapping_src_first();

    test_copy_equal();

    test_copy_null();

    return EXIT_SUCCESS;
}