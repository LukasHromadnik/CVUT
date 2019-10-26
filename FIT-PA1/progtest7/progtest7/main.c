//
//  main.c
//  progtest7
//
//  Created by Lukáš Hromadník on 04.12.13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

int compare(const void * a, const void * b) {
    if (*(int *)a == *(int *)b)
        return 0;
    else
        if (*(int *)a < *(int *)b)
            return 1;
        else
            return -1;
}

int * readInput(int * cnt, int * list) {
    int tmp_cnt, i, a, one_found = 0, duplicity = 0;
    
    printf("Zadejte pocet zavazi:\n");
    if (scanf("%d", &tmp_cnt) != 1 || tmp_cnt < 1) {
        printf("Nespravny vstup.\n");
        return NULL;
    }
    
    list = (int *)malloc(tmp_cnt * sizeof(int));
    
    printf("Zadejte hmotnosti zavazi:\n");
    for (i = 0; i < tmp_cnt; i++) {
        if (scanf("%d", &a) != 1 || a < 1) {
            printf("Nespravny vstup.\n");
            return NULL;
        }
        
        if (a == 1)
            one_found = 1;
        
        list[i] = a;
    }
    
    if ( ! one_found) {
        printf("Nespravny vstup.\n");
        return NULL;
    }
    
    qsort(list, tmp_cnt, sizeof(int), compare);
    
    for (i = 1; i < tmp_cnt; i++)
        if (list[i - 1] == list[i])
            duplicity = 1;
    
    if (duplicity == 1) {
        printf("Nespravny vstup.\n");
        return NULL;
    }
    
    *cnt = tmp_cnt;
    
    return list;
}

typedef struct Res {
    int Count;
    int * Content;
} Res;

int main(int argc, const char * argv[]) {
    int cnt = 0, * list = NULL, w, match = 0, i, arr_size = 0, j = 0, j2 = 0, i1 = 0, i2 = 0, br = INT_MAX, soucet, weight;
    Res * results = NULL, * tmp = NULL;
    
    list = readInput(&cnt, list);
    
    if (list == NULL)
        return 1;
    
    printf("Zadejte hledanou hmotnost:\n");
    
    while (scanf("%d", &w) == 1 && w > 0) {
        // Alokuj pole
        if (w > arr_size) {
            tmp = (Res *)malloc(w * sizeof(Res));
            for (i = 0; i < arr_size; i++)
                tmp[i] = results[i];
            free(results);
            // TO DO free in results;
            results = tmp;
        
        
            // Generuj pole
            for (i = arr_size; i < w; i++) {
                weight = i + 1;
                // Hleda shodu aktualni hmotnosti zavazi s hmotnostmi v listu
                for (j = 0; j < cnt; j++) {
                    if (list[j] == weight) {
                        match = 1;
                        break;
                    }
                }
            
                // Nenajde-li shodu, hleda nejlepsi reseni
                if (match == 0) {
                    for (j = 1; j <= (int) ceil(weight / 2); j++) {
                        j2 = weight - j;
                        // Kdyz je nalezeno lepsi reseni, ulozi se pocet a indexy zavazi
                        soucet = results[j - 1].Count + results[j2 - 1].Count;
//                        prntf("%d\n", soucet);
                        if (soucet < br) {
                            br = soucet;
                            i1 = j - 1;
                            i2 = j2 - 1;
                        }
//                        printf("i: %d, j: %d, j2: %d, soucet: %d\n", i, j, j2, soucet);
                    }

                    results[i].Count = br;
                    results[i].Content = (int *)malloc(br * sizeof(int));
                
                    // Slozeni prvni prvku z nejlepsi shody se prida do nove hmotnosti
                    for (j = 0; j < results[i1].Count; j++) {
                        results[i].Content[j] = results[i1].Content[j];
//                        printf("%d, ", results[i1].Content[j]);
                    }
                
                    // Slozeni druheho prvku z nejlepsi shody se prida do nove hmotnosti
                    for (j = results[i1].Count; j < br; j++)
                        results[i].Content[j] = results[i2].Content[j - results[i1].Count];
            
                    // Seradi se pole vypisu hmotnosti
                    qsort(results[i].Content, results[i].Count, sizeof(int), compare);
                }
                else {
                    results[i].Count = 1;
                    results[i].Content = (int *) malloc(sizeof(int));
                    results[i].Content[0] = weight;
                    match = 0;
                }
                br = INT_MAX;
            }
            
            arr_size = w;
        }
        
        // Vypis vysledek
        
//        printf("%d\n", w);
        soucet = 1;
        printf(" = ");
        for (i = 0; i < results[w - 1].Count; i++) {
            if (results[w - 1].Content[i] == results[w - 1].Content[i + 1]) {
                soucet++;
                continue;
            }
            else {
                printf("%d x %d", soucet, results[w - 1].Content[i]);
                if ((i + 1) != results[w - 1].Count)
                    printf(" + ");
                soucet = 1;
            }
        }
        printf("\n");
    }

    if ( ! feof(stdin)) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    return 0;
}
