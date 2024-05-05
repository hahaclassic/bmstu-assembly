#include <iostream>
#include <cmath>

#define PI_ACC_1 3.14
#define PI_ACC_2 3.141596


#define PRECISION "%.20f"

double fpu_sin(double num)
{
    double res;

    __asm__(
        "fld %1\n\t"                     // загружаем константу пи на вершину стека сопроцессора
        "fsin\n\t"                      // считаем синус числа, находящегося в ST(0) (на вершине стекасопроцессора); операнд считается заданным в радианах
        "fstp %0\n\t"                   // извлекаем число из FPU в память, в данном случае из ST(0) в res
        : "=m" (res)                    // res - выходной параметр
        : "m" (num) 
    );

    return res;
}

double fpu_sin_pi()                             // обёртка над ассемблерной вставкой, считающей sin(pi)
{
    double res;

    __asm__(
        "fldpi\n\t"                     // загружаем константу пи на вершину стека сопроцессора
        "fsin\n\t"                      // считаем синус числа, находящегося в ST(0) (на вершине стекасопроцессора); операнд считается заданным в радианах
        "fstp %0\n\t"                   // извлекаем число из FPU в память, в данном случае из ST(0) в res
        : "=m" (res)                    // res - выходной параметр
    );

    return res;
}

double fpu_sin_half_pi()                        // обёртка над ассемблерной вставкой, считающей sin(pi / 2)
{
    double res;
    const int divider = 2;
    
    __asm__(
        "fldpi\n\t"                     // загружаем константу пи на вершину стека сопроцессора
        "fild %1\n\t"                   // загружаем divider (целое число, поэтому 'i' в названии команды) на вершину стека
        "fdivp\n\t"                     // делим ST(1) на ST(0), сохраняем результат в ST(1) и извлекаем из стека сопроцессора (поэтому 'p' в названии команды)
        "fsin\n\t"                      // считаем синус числа, находящегося в ST(0) (на вершине стекасопроцессора); операнд считается заданным в радианах
        "fstp %0\n\t"                   // извлекаем число из FPU в память, в данном случае из ST(0) в res
        : "=m" (res)                    // res - выходной параметр
        : "m" (divider)                 // divider - входной параметр; да-да, я послал сюда число 2 через переменную, чтобы не возиться с загрузкой числа в стек сопроцессора
    );

    return res;
}

int main()
{
    printf("\nTest PI: \n");
    printf("LIB sin(3.14) =      " PRECISION "\n", sin(PI_ACC_1));
    printf("LIB sin(%lf) = " PRECISION "\n", PI_ACC_2, sin(PI_ACC_2));
    printf("LIB sin(M_PI) =      " PRECISION "\n", sin(M_PI));
    printf("FPU sin(3.14) =      " PRECISION "\n", fpu_sin(PI_ACC_1));
    printf("FPU sin(%lf) = " PRECISION "\n", PI_ACC_2, fpu_sin(PI_ACC_2));
    printf("FPU sin(FPU PI) =   " PRECISION "\n", fpu_sin_pi());
    

    printf("\nTest PI / 2: \n");
    printf("LIB sin(3.14 / 2) =     " PRECISION "\n", sin(PI_ACC_1 / 2));
    printf("LIB sin(%lf / 2) = " PRECISION "\n", PI_ACC_2, sin(PI_ACC_2 / 2));
    printf("LIB sin(M_PI / 2) =     " PRECISION "\n", sin(M_PI / 2));
    printf("FPU sin(3.14 / 2) =     " PRECISION "\n", fpu_sin(PI_ACC_1 / 2));
    printf("FPU sin(%lf / 2) = " PRECISION "\n", PI_ACC_2, fpu_sin(PI_ACC_2 / 2));
    printf("FPU sin(FPU PI / 2) =   " PRECISION "\n", fpu_sin_half_pi());

    return EXIT_SUCCESS;
}