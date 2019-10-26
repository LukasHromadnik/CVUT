#include <stdio.h>

int phi(int n) {
    int res = n;
    
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            res -= res / i;
        while (n % i == 0)
            n /= i;
    }
    
    if (n > 1)
        res -= res / n;
    
    return res;
}


int main(int argc, const char * argv[]) {
    int a;
    
    printf("Zadejte n:\n");
    
    if (scanf("%d", &a) != 1 || a < 1) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    printf("phi(%d) = %d\n", a, phi(a));
    
    return 0;
}
