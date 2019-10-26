//
//  main.c
//  progtest4
//
//  Created by Lukáš Hromadník on 12.11.13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int pocet_nadrzi, i, a, b, c, d;
    int pole[20][3];
    
    printf("Zadejte pocet nadrzi:\n");
    if (scanf("%d", &pocet_nadrzi) != 1 || pocet_nadrzi < 1) {
        printf("Nespravny vstup.\n");
        return 0;
    }
    
    printf("Zadejte parametry nadrzi:\n");
    for (i = 0; i < pocet_nadrzi; i++) {
        if (scanf("%d %d %d %d", &a, &b, &c, &d) != 4 || b < 1 || c < 1 || d < 1) {
            printf("Nespravny vstup.\n");
            return 0;
        }
        
        // Nadmorska vyska pocatku
        pole[i][0] = a;
        
        // Objem pro jeden vyskovy metr
        pole[i][1] = c * d;
        
        // Nadmorska vyska konce
        pole[i][2] = a + b;
    }
    
    return 0;
}

