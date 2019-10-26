#include <stdio.h>

int saveVals(int * p, double a, double b) {
    printf("Souradnice: %f %f.\n", a, b);
    
    return 0;
}

int readInput(int res, int * p) {
    double a, b;
    char z;
    
    z = getchar();
    
    if (z == EOF)
        return 0;
    if (z != '[') {
        printf("Nespravny vstup.\n");
        res = 1;
        return res;
    }
    if (scanf("%lf,%lf", &a, &b) != 2) {
        printf("Nespravny vstup.\n");
        res = 1;
        return res;
    }
    if ((z = getchar()) != ']') {
        printf("Nespravny vstup.\n");
        res = 1;
        return res;
    }
    
    saveVals(* p, a, b);
    
    z = getchar();
    
    if (z == '\n' || z == ' ')
        res = readInput(res, * p);
    
    return res;
}

int main(int argc, const char * argv[])
{
    int letadla[100][2];
    int ** p = letadla;
    
    printf("Zadejte souradnice:\n");
    
    if (readInput(0, * p) == 1)
        return 1;
    
    printf("Tady to bude pokracovat.\n");
    
    return 0;
}

