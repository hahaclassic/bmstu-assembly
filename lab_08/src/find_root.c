#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-10

// y = sin(x^2 + 5x)
double function(double x)
{
    double res;
    const int k = 5;

    __asm__(
        "fld %1\n\t"    
        "fld %1\n\t"
        "fmulp\n\t"
        "fld %1\n\t"
        "fild %2\n\t"
        "fmulp\n\t"
        "faddp\n\t"                 
        "fsin\n\t"                      
        "fstp %0\n\t"                  
        : "=m" (res)                    
        : "m" (x), "m" (k)
    );

    return res;
}

double find_root(double a, double b, size_t iterations) 
{   
    double x = 0, a_val, b_val, x_val = 0;

    if (function(a) * function(b) > 0) {
        printf("root not found\n");
        return INFINITY;
    }

    for (size_t i = 0; i < iterations; i++) 
    {
        a_val = function(a);
        b_val = function(b);
        if (fabs(a_val - b_val) < EPS) {
            break;
        }

        // x = a - (b - a) / (b_val - a_val) * a_val;
        __asm__(
            "fld %1\n\t" // a     
            "fld %2\n\t" // a b 
            "fld %1\n\t" // a b a
            "fsubp\n\t" // a (b - a)
            "fld %4\n\t" // a (b - a) b_val
            "fld %3\n\t" // a (b - a) b_val a_val 
            "fsubp\n\t" // a (b - a) (b_val - a_val) 
            "fdivp\n\t" // a ((b - a) / (b_val - a_val))
            "fld %3\n\t" // a ((b - a) / (b_val - a_val)) a_val
            "fmulp\n\t"  // a (((b - a) / (b_val - a_val)) * a_val)
            "fsubp\n\t" // a - (((b - a) / (b_val - a_val)) * a_val)
            "fstp %0\n\t"           
            : "=m" (x)                  
            : "m" (a), "m" (b), "m" (a_val), "m" (b_val)
        );
        x_val = function(x);

        printf("a: %.20lf, a_val: %.20lf,\n\
b: %.20lf, b_val: %.20lf,\n\
x: %.20lf, x_val: %.20lf\n\n\n", a, a_val, b, b_val, x, x_val);

        if (a_val * x_val < 0) {
            b = x;
        } else if (b_val * x_val < 0) {
            a = x;
        } else {
            printf("root not found\n");
            return INFINITY;
        }
    }

    return a_val; 
}

int main()
{
    double a, b;
    int count;

    printf("Enter a: ");
    int input = scanf("%lf", &a);
    if (input != 1)
    {
        printf("[ERR]: error while reading float num\n");
        return EXIT_FAILURE;
    }
    printf("Enter b: "); 
    input = scanf("%lf", &b);
    if (input != 1)
    {
        printf("[ERR]: error while reading float num\n");
        return EXIT_FAILURE;
    } 
    printf("Enter num of interations: ");
    input = scanf("%d", &count);
    if (input != 1 || count <= 0)
    {
        printf("[ERR]: error while reading num of iterations\n");
        return EXIT_FAILURE;
    } 
    if (a > b) {
        double temp;
        temp = a;
        a = b;
        b = temp;
    }

    printf("root in [%lf, %lf]: %.20lf\n", a, b, find_root(a, b, count));
    return EXIT_SUCCESS;
}


// __asm__ (
//     "fld %2\n\t"
//     "fld %3\n\t"
//     "fmulp\n\t"
//     "ftst\n\t"
//     "fstsw ax\n\t"
//     "sahf\n\t"
//     "ja set_a\n\t"
//     "set_b:\n\t"
//     "   fld %4\n\t"
//     "   fstp %1\n\t"
//     "   jmp exit\n\t"
//     "set_a:\n\t"
//     "   fld %4\n\t"
//     "   fstp %0\n\t"
//     "exit:\n\t"
//     "   fnop\n\t"          
//     : "=m" (a), "=m" (b)                  
//     : "m" (x_val), "m" (a_val), "m" (x)
//     : "rax"
// );