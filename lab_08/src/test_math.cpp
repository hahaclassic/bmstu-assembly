#include <stdio.h>
#include <time.h>

#define REPEATS_COUNT 1e7

template <typename Type>
void time_mesurement_cpp(int times, Type a, Type b)                
{
    Type res;

    clock_t start = clock();
    for (size_t i = 0; i < times; ++i)
        res = a + b;
    clock_t time_sum = clock() - start;

    start = clock();
    for (size_t i = 0; i < times; ++i)
        res = a * b;
    clock_t time_mul = clock() - start;

    printf("Sum: %zu ms     Mul: %zu ms\n", time_sum, time_mul);
}

template <typename Type>
void time_mesurement_asm(int times, Type a, Type b)             
{
    Type res;

    clock_t start = clock();
    for (size_t i = 0; i < times; ++i)                             
        __asm__("fld %1\n"                                          
                "fld %2\n"                                      
                "faddp\n"                                         
                "fstp %0\n"                                        
                : "=m"(res)                                         
                : "m"(a),                                           
                  "m"(b)                                           
                );
    clock_t time_sum = clock() - start;

    start = clock();
    for (size_t i = 0; i < times; ++i)
        __asm__("fld %1\n"                                        
                "fld %2\n"                                         
                "fmulp\n"                                         
                "fstp %0\n"                                      
                : "=m"(res)                                       
                : "m"(a),                                           
                  "m"(b)                                            
                );
    clock_t time_mul = clock() - start;

    printf("Sum: %zu ms     Mul: %zu ms\n", time_sum, time_mul);
}

#ifdef ASM
#define TIME_MEASUREMENT(Type) time_mesurement_asm<Type>
#else
#define TIME_MEASUREMENT(Type) time_mesurement_cpp<Type>
#endif

int main()
{
    double a = -2134234.3342134, b = 1234234.1303040;

    printf("Float:          ");
    TIME_MEASUREMENT(float)(REPEATS_COUNT, a, b);
    printf("Double:         ");
    TIME_MEASUREMENT(double)(REPEATS_COUNT, a, b);

    return 0;
}