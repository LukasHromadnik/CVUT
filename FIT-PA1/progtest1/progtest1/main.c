//
//  main.c
//  progtest1
//
//  Created by Lukáš Hromadník on 25/10/13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <math.h>

double obsah_kruznice(double r) {
    return M_PI * r * r;
}

int main(int argc, const char * argv[])
{
    double x1, y1, r1, x2, y2, r2, sp, vs, vp, mp, prekryti, min = 1e-12;
    int mk, vk;
    
    printf("Zadejte parametry kruznice #1:\n");
    if (scanf("%lf %lf %lf", &x1, &y1, &r1) != 3 || r1 <= 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    printf("Zadejte parametry kruznice #2:\n");
    if (scanf("%lf %lf %lf", &x2, &y2, &r2) != 3 || r2 <= 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    sp = r1 + r2;
    
    vs = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    
    if (r1 > r2) {
        vp = r1;
        mp = r2;
        mk = 2;
        vk = 1;
    }
    else {
        vp = r2;
        mp = r1;
        mk = 1;
        vk = 2;
    }
    
    if (fabs(vs - sp) < min * sp)
        printf("Vnejsi dotyk, zadny prekryv.\n");
    else if (vs != 0 && r1 != r2 && vs < sp) {
        if (fabs(vs + mp - vp) < min * vp)
            printf("Vnitrni dotyk, kruznice #%d lezi uvnitr kruznice #%d, prekryv: %f\n", mk, vk, obsah_kruznice(mp));
        else if ((vs + mp) < vp)
            printf("Kruznice #%d lezi uvnitr kruznice #%d, prekryv: %f\n", mk, vk, obsah_kruznice(mp));
        else {
            prekryti = r2 * r2 * acos((sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) * sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) + r2 * r2 - r1 * r1) / (2 * sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) * r2)) + r1 *r1 * acos((sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) * sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) + r1 * r1 - r2 * r2) / (2 * sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) * r1)) - 0.5 * sqrt((-sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) + r2 + r1) * (sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) + r2 - r1) * (sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) - r2 + r1) * (sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) + r2 + r1));
            
            printf("Kruznice se protinaji, prekryv: %f\n", prekryti);
        }
    }
    else if (vs > sp)
        printf("Kruznice lezi vne sebe, zadny prekryv.\n");
    else if (vs == 0 && r1 == r2) {
        printf("Kruznice splyvaji, prekryv: %f\n", obsah_kruznice(r1));
    }
    
    return 0;
}

