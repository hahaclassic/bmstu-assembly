#include <stdio.h>
#include <time.h>

#define REPEATS_COUNT 1e7

template <typename Type>
void time_mesurement_cpp(int times, Type a, Type b)                 // шаблон подсчёта общего времени выполнения суммы и произведения двуж чисел (REPEATS_COUNT раз) средствами языка c++
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
void time_mesurement_asm(int times, Type a, Type b)                 // шаблон подсчёта общего времени выполнения суммы и произведения двуж чисел (REPEATS_COUNT раз) с помощью ассемблерных вставок
{
    Type res;

    clock_t start = clock();
    for (size_t i = 0; i < times; ++i)                              // не выделяем ассемблерную вставку в обёртку, так как нам не нужно учитывать время, которое тратится на вызов обёртки
        __asm__("fld %1\n"                                          // загружаем a на вершину стека
                "fld %2\n"                                          // загружаем b на вершину стека
                "faddp\n"                                           // складываем ST(1) и ST(0), сохраняем результат в ST(1) и извлекаем из стека сопроцессора (поэтому 'p' в названии команды)
                "fstp %0\n"                                         // извлекаем число из FPU в память, в данном случае из ST(0) в res
                : "=m"(res)                                         // res - выходной параметр
                : "m"(a),                                           // a - входной параметр
                  "m"(b)                                            // b - входной параметр
                );
    clock_t time_sum = clock() - start;

    start = clock();
    for (size_t i = 0; i < times; ++i)
        __asm__("fld %1\n"                                          // загружаем a на вершину стека
                "fld %2\n"                                          // загружаем b на вершину стека
                "fmulp\n"                                           // перемножаем ST(1) и ST(0), сохраняем результат в ST(1) и извлекаем из стека сопроцессора (поэтому 'p' в названии команды)
                "fstp %0\n"                                         // извлекаем число из FPU в память, в данном случае из ST(0) в res
                : "=m"(res)                                         // res - выходной параметр
                : "m"(a),                                           // a - входной параметр
                  "m"(b)                                            // b - входной параметр
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