#ifndef __PROGTEST__
#include <stdio.h>
#endif /* __PROGTEST__ */

int monthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int isLeapYear(long int y) {
    if (y % 4 == 0 && ! (y % 100 == 0 && y % 400 != 0) && y % 4000 != 0) return 1;
    
    return 0;
}

int countLeapYears(long int y) {
    int count = 0;
    if (y >= 4) {
        count += y / 4;
        
        if (y >= 100) {
            count -= y / 100;
            
            if (y >= 400) {
                count += y / 400;
                
                if (y >= 4000)
                    count -= y / 4000;
            }
        }
    }
    
    if (isLeapYear(y)) count--;
    
    return count;
}

int checkDate(long int y, int m, int d, int h, int i) {
    if (y >= 1600 && m >= 1 && m <= 12 && d >= 1 && d <= (monthDays[m] + isLeapYear(y)) && h >= 0 && h <= 23 && i >= 0 && i <= 59)
        return 1;

    return 0;
}

long long int countCuckooq(long int y, int m, int d) {
    long long int cuckoo = 0;
    int cuckooPerDay = 180, j;
    
    cuckoo += (countLeapYears(y - 1) + y - 1) * 365 * cuckooPerDay;
    
    for (j = 1; j <= (m - 1); j++) {
        cuckoo += monthDays[j] * cuckooPerDay;
    }
    
    if (isLeapYear(y) && m > 2) cuckoo += cuckooPerDay;
    
    cuckoo += (d - 1) * cuckooPerDay;
    
    return cuckoo;
}

int countCuckooNow(int h, int i, int first) {
    int cuckoo = 0;
    int cuckooPerDay = 180;
    
    if (h > 12) {
        h -= 12;
        cuckoo += (h * (1 + h)) / 2 + h;
        cuckoo += cuckooPerDay / 2 + 12;
    }
    else {
        cuckoo += (h * (1 + h)) / 2 + 12 + h;
    }
    
    if (first) {
        if (i == 0)
            cuckoo -= h;
        if (i == 30)
            cuckoo -= 1;
    }
    else
        if (i >= 30)
            cuckoo++;
    
    return cuckoo;

}

int cuckooClock(long int y1, int m1, int d1, int h1, int i1, long int y2, int m2, int d2, int h2, int i2, long long int * cuckoo) {
    long long int tmp_cuckoo;
    
    if ( ! checkDate(y1, m1, d1, h1, i1)) return 0;
    if ( ! checkDate(y2, m2, d2, h2, i2)) return 0;
    if (y1 > y2 || m1 > m2 || d1 > d2) return 0;
    if (y1 == y2 && m1 == m2 && d1 == d2 && h1 > h2) return 0;
    if (y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && m1 > m2) return 0;
    
    tmp_cuckoo = countCuckoo(y2, m2, d2) - countCuckoo(y1, m1, d1);
    tmp_cuckoo += countCuckooNow(h2, i2, 0) - countCuckooNow(h1, i1, 1);
    
    if (tmp_cuckoo == 0 && i1 == 0)
        tmp_cuckoo = h1;
    
    if (h1 == 0 && i1 == 0)
        tmp_cuckoo += 12;
    
    * cuckoo = tmp_cuckoo;
    
    return 1;
}

#ifndef __PROGTEST__
int main(int argc, const char * argv[])
{
    long long int cuckoo;
    int res = 0 ;
    
	res = cuckooClock ( 2013, 10,  1, 13, 15, 2013, 10,  1, 18, 45, &cuckoo ); // 1
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;

	res = cuckooClock ( 2013, 10,  1, 13, 15, 2013, 10,  2, 11, 20, &cuckoo ); // 2
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013,  1,  1, 13, 15, 2013, 10,  5, 11, 20, &cuckoo ); // 3
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2012,  1,  1, 13, 15, 2012, 10,  5, 11, 20, &cuckoo ); // 4
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1900,  1,  1, 13, 15, 1900, 10,  5, 11, 20, &cuckoo ); // 5
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013, 10,  1,  0,  0, 2013, 10,  1, 12,  0, &cuckoo ); // 6
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013, 10,  1,  0, 15, 2013, 10,  1,  0, 25, &cuckoo ); // 7
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013, 10,  1, 12,  0, 2013, 10,  1, 12,  0, &cuckoo ); // 8
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013, 11,  1, 12,  0, 2013, 10,  1, 12,  0, &cuckoo ); // 9
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2013, 10, 32, 12,  0, 2013, 11, 10, 12,  0, &cuckoo ); // 10
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
	res = cuckooClock ( 2100,  2, 29, 12,  0, 2100,  2, 29, 12,  0, &cuckoo ); // 11
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2400,  2, 29, 12,  0, 2400,  2, 29, 12,  0, &cuckoo ); // 12
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2013, 10, 1, 12, 0, 2013, 10, 1, 12, 0, &cuckoo ); // 13
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2100, 12, 26, 0, 0, 2100, 12, 31, 0, 0, &cuckoo ); // 14
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2000, 12, 24, 0, 0, 2000, 12, 29, 0, 0, &cuckoo ); // 15
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2046, 10, 12, 0, 0, 2046, 10, 12, 0, 0, &cuckoo ); // 16
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1908, 4, 13, 3, 32, 2036998342, 10, 17, 8, 57, &cuckoo ); // 17
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1905, 10, 9, 14, 57, 2088, 3, 10, 11, 28, &cuckoo ); // 18
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2034, 7, 5, 23, 45, 2034, 7, 5, 22, 45, &cuckoo ); // 19
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2000, 1, 1, 24, 0, 2000, 1, 2, 23, 59, &cuckoo ); // 20
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1922, 4, 3, 17, 53, 1945, 9, 7, 7, 41, &cuckoo ); // 21
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2023, 10, 20, 0, 0, 2023, 10, 20, 0, 30, &cuckoo ); // 22
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2035, 3, 10, 16, 44, 2153, 10, 29, 19, 43, & cuckoo ); // 23
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2017, 11, 22, 0, 0, 2017, 11, 22, 3, 45, &cuckoo ); // 24
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2046, 8, 19, 0, 0, 2046, 8, 19, 12, 45, &cuckoo ); // 25
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2086, 9, 11, 18, 47, 2161, 2, 22, 6, 58, &cuckoo ); // 26
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2076, 4, 20, 10, 45, 1905, 11, 24, 3, 52, &cuckoo ); // 27
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2070, 11, 26, 15, 44, 2028904991, 11, 2, 16, 50, &cuckoo ); // 28
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2081, 1, 27, 0, 30, 2081, 1, 27, 3, 30, &cuckoo ); // 29
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1995, 3, 12, 6, 42, 2160, 8, 12, 10, 0, &cuckoo ); // 30
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2083, 2, 17, 2, 56, 2182, 2, 15, 10, 52, &cuckoo ); // 31
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2100, 1, 1, 0, 0, 2100, 12, 20, 0, 0, &cuckoo ); // 32
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1907, 3, 19, 5, 6, 2031184000, 4, 27, 8, 6, &cuckoo ); // 33
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2000, 11, 30, 0, 0, 2000, 12, 29, 0, 0, &cuckoo ); // 34
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2077, 3, 15, 1, 0, 2077, 3, 15, 2, 0, &cuckoo ); // 35
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2067, 1, 15, 0, 0, 2067, 1, 15, 12, 30, &cuckoo ); // 36
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2061, 8, 27, 0, 15, 2061, 8, 27, 0, 45, &cuckoo ); // 37
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2136, 4, 2, 9, 3, 2193, 6, 11, 17, 34, &cuckoo ); // 38
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2011, 11, 24, 22, 13, 2065, 1, 18, 0, 2, &cuckoo ); // 39
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 1983, 11, 21, 14, 0, 2042, 7, 2, 21, 37, &cuckoo ); // 40
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
	
	res = cuckooClock ( 2073, 4, 6, 0, 0, 2073, 4, 6, 1, 30, &cuckoo ); // 41
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2050, 7, 7, 0, 30, 2050, 7, 7, 2, 0, &cuckoo ); // 42
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2000, 11, 30, 0, 0, 2000, 12, 24, 0, 0, &cuckoo ); // 43
	printf("res = %d, cuckoo = %lld\n", res, cuckoo);
	res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2057, 5, 25, 22, 45, 2057, 5, 25, 22, 45, &cuckoo ); // 44
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2164, 8, 15, 21, 12, 2199, 2, 27, 21, 56, &cuckoo ); // 45
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2072, 7, 2, 20, 16, 2062961132, 11, 8, 4, 7, &cuckoo ); // 46
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2090, 4, 15, 1, 0, 2090, 4, 15, 12, 15, &cuckoo ); // 47
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2091, 9, 5, 0, 45, 2091, 9, 5, 0, 45, &cuckoo ); // 48
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock ( 2082, 7, 10, 23, 0, 2082, 7, 10, 23, 45, &cuckoo ); // 49
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock (2048, 11, 5, 12, 0, 2048, 11, 5, 13, 45, &cuckoo ); // 50
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;
    
    res = cuckooClock (2013, 12, 16, 23, 0, 2013, 12, 17, 0, 0, &cuckoo ); // 50
    printf("res = %d, cuckoo = %lld\n", res, cuckoo);
    res = 0; cuckoo = 0;

    
    return 0;
}
#endif /* __PROGTEST__ */
