//
//  main.c
//  zapocet1
//
//  Created by Lukáš Hromadník on 06.01.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define LINE_MAX 1000

int main(int argc, const char * argv[]) {
    char sym, station;
    int station_num, people, n, i, a = 'A', j = 'J', max_num = 99, arr_size = (j - a) * 100 + max_num + 1, maximum = 0, index = 0;
    int cekajici[arr_size], odjelo[arr_size], flag = 0;
    
    for (i = 0; i < arr_size; i++) {
        cekajici[i] = 0;
        odjelo[i] = 0;
    }
    
    printf("Zadejte pocet udalosti:\n");

    if (scanf("%d", &n) != 1) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    printf("Zadejte udalosti:\n");
    
    for (i = 0; i < n; i++) {
        if (scanf(" %c%c%d:%d", &sym, &station, &station_num, &people) != 4 || (sym != '+' && sym != '-') || (station - a) > (j - a) || (station - a) < 0) {
            printf("Nespravny vstup.\n");
            return 1;
        }
        index = (station - a) * 100 + station_num;
        if (sym == '+')
            cekajici[index] = people;
        else {
            if (people > cekajici[index]) {
                odjelo[index] = cekajici[index];
                cekajici[index] = 0;
            }
            else {
                odjelo[index] = people;
                cekajici[index] -= people;
            }
        }
    }
        
    for (i = 0; i < arr_size; i++) {
        if (odjelo[i] > maximum)
            maximum = odjelo[i];
    }
        
    printf("Nejvice odjelo pasazeru: %d, nastupiste: ", maximum);
    
    for (i = 0; i < arr_size; i++) {
        if (odjelo[i] == maximum) {
            if (flag) {
                printf(", ");
            }
            printf("%c%d", (i / 100) + a, i % 100);
            flag = 1;
        }
    }
    
    printf("\n");

    return 0;
}

