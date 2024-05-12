#include <stdio.h>

int main() {
    char str[20] = "\n";
    double a;
    sprintf(str, "%lf", 10.0);
    printf("%s\n", str);
    return 0;
}