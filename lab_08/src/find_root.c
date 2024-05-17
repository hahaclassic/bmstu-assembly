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

int find_root(double a, double b, int iterations, double *result) 
{   
    double x = a, a_val, b_val, x_val = 0;

    if (a > b || iterations < 0 || function(a) * function(b) > 0) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < iterations; i++) 
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

        if (a_val * x_val < 0) {
            b = x;
        } else if (b_val * x_val < 0) {
            a = x;
        } else {
            return EXIT_FAILURE;
        }
    }

    (*result) = x;
    return EXIT_SUCCESS; 
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

    double result;
    int rc = find_root(a, b, count, &result);
    if (rc) {
        printf("root not found.\n");
    } else {
        printf("root in [%lf, %lf]: %.20lf\n", a, b, result);
    }

    return EXIT_SUCCESS;
}
