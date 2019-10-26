//
//  main.c
//  progtest6
//
//  Created by Lukáš Hromadník on 03.12.13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void stripNonAlpha(char * t) {
    while (*t) {
        if ( ! isalpha(*t))
            *t = ' ';
        t++;
    }
}

void  stripSpaces ( char * str ) {
    char * dst, * src;
    int inSpaces = 1;
    
    dst = str;
    src = str;
    
    while (*src) {
        if (inSpaces) {
            if (*src != ' ') {
                *dst++ = *src;
                inSpaces = 0;
            }
        }
        else {
            *dst++ = *src;
            if (*src == ' ')
                inSpaces = 1;
        }
        src ++;
    }
    if (inSpaces && dst > str)
        dst --;
    *dst = 0;
}

int countWords(char * t) {
    int cnt = 0, sf = 0, lf = 0;

    if (*t == '\n')
        return cnt;
    
    if (*t == ' ')
        sf = 1;
    
    while (*t) {
        
        if (*t == ' ' && *(t + 1) != ' ' && *(t + 1) != '\n')
            cnt++;
        
        if (*t == '\n')
            break;
        
        t++;
        lf = 1;
    }
    
    if ( ! sf && lf)
        cnt++;
    
    return cnt;
}

static int compare (const void * a, const void * b) {
    return strcasecmp (*(const char **) a, *(const char **) b);
}

int deleteDuplicates(char ** list, int cnt) {
    int i, d_cnt = 0, new_cnt = 0;
    
    for (i = 1; i < cnt; i++)
        if (strcasecmp(list[i], list[i - 1]) == 0)
            d_cnt++;
    for (i = (cnt - 1); i > 0; i--) {
        if (strcasecmp(list[i], list[i - 1]) == 0) {
            free(list[i]);
            list[i] = NULL;
        }
    }
    new_cnt = cnt - d_cnt;
    
    return new_cnt;
}

void freeVars(char ** list1, char ** list2, char * text1, char * text2, int cnt1, int cnt2) {
    int i;
    
    for (i = 0; i < cnt1; i++)
        free(list1[i]);
    
    for (i = 0; i < cnt2; i++)
        free(list2[i]);
    
    free(list1);
    free(list2);
    
    free(text1);
    free(text2);
}

int sameWords (const char * a, const char * b) {
    char ** list1 = NULL, ** list2 = NULL, * text1 = NULL, * text2 = NULL, * p1 = NULL, * p2 = NULL;
    int cnt1 = 0, cnt2 = 0, i = 0, new_cnt1 = 0, new_cnt2 = 0, posun1 = 0, posun2 = 0;
    
    if (a == NULL)
        a = "";
    if (b == NULL)
        b = "";
    
    text1 = (char *) malloc((strlen(a) + 1) * sizeof(char));
    strcpy(text1, a);
    
    text2 = (char *) malloc((strlen(b) + 1) * sizeof(char));
    strcpy(text2, b);
    
    stripNonAlpha(text1);
    stripNonAlpha(text2);
    
    stripSpaces(text1);
    stripSpaces(text2);

    cnt1 = countWords(text1);
    cnt2 = countWords(text2);
    
    list1 = (char **) malloc(cnt1 * sizeof(char *));
    list2 = (char **) malloc(cnt2 * sizeof(char *));
    
    p1 = strtok(text1, " ");
    
    while (p1) {
        list1[i] = (char *) malloc((strlen(p1) + 1) * sizeof(char));
        strcpy(list1[i], p1);
        p1 = strtok(NULL, " ");
        i++;
    }
    
    i = 0;
    
    p2 = strtok(text2, " ");
    
    while (p2) {
        list2[i] = (char *) malloc((strlen(p2) + 1) * sizeof(char));
        strcpy(list2[i], p2);
        p2 = strtok(NULL, " ");
        i++;
    }
    
    qsort(list1, cnt1, sizeof(char *), compare);
    qsort(list2, cnt2, sizeof(char *), compare);
    
    new_cnt1 = deleteDuplicates(list1, cnt1);
    new_cnt2 = deleteDuplicates(list2, cnt2);

    if (new_cnt1 != new_cnt2) {
        freeVars(list1, list2, text1, text2, cnt1, cnt2);
        return 0;
    }
    
    if (new_cnt1 == 0) {
        freeVars(list1, list2, text1, text2, cnt1, cnt2);
        return 1;
    }
    
    for (i = 0; i < new_cnt1; i++) {
        while (list1[i + posun1] == NULL) {
            if ((i + posun1) < new_cnt1)
                posun1++;
            else
                break;
        }
        while (list2[i + posun2] == NULL) {
            if ((i + posun2) < new_cnt1)
                posun2++;
            else
                break;
        }
        if ((i + posun1) >= new_cnt1 && (i + posun2) >= new_cnt1 )
            break;
        if (strcasecmp(list1[i + posun1], list2[i + posun2]) != 0) {
            freeVars(list1, list2, text1, text2, cnt1, cnt2);
            return 0;
        }
    }
    
    freeVars(list1, list2, text1, text2, cnt1, cnt2);
    return 1;
}

void printRes(int res, int progtest) {
    printf("res = %d/%d\n", res, progtest);
}

#ifndef __PROGTEST__
int main(int argc, const char * argv[])
{
    int res;

    res = sameWords ( "Hello hello .....students.", "HELLO studEnts!" );
    printRes(res, 1);
    
    res = sameWords ( " He said 'hello!'", "'Hello.' he   said." );
    printRes(res, 1);
    
    res = sameWords ( "He said he would do it.", "IT said: 'He would do it.'" );
    printRes(res, 1);
    
    res = sameWords ( "one two three", "one two five" );
    printRes(res, 0);
    
    res = sameWords ( "thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld", "thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld" );
    printRes(res, 1);

    res = sameWords ( " ", "" );
    printRes(res, 1);
    
    res = sameWords ( "one ONE oNe OnE TWO two", "One TwO ONE TWO two two twO" );
    printRes(res, 1);

    res = sameWords ( "", NULL );
    printRes(res, 1);
    
    return 0;
}
#endif /* __PROGTEST__ */
