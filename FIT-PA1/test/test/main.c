//
//  main.c
//  test
//
//  Created by Lukáš Hromadník on 28.10.13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int rec(int x, int y){
    if(x<=0) return (0);
    return y + rec2(y, x-1);
}

int rec2(int x, int y){
    if(y<=0) return (0);
    return x + rec2(y-1, x);
}
int main(int argc, const char * argv[]) {
    int x;
    x=rec(5,4);

    printf("%d\n", x);
    x=rec(3,7);
    printf("%d\n", x);
}

