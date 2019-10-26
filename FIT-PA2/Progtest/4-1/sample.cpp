bool status;
int  balance;
char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
CBank a;
status = a . NewAccount ( "123456", 1000 );
 // status = true
status = a . NewAccount ( "987654", -500 );
 // status = true
status = a . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 // status = true
status = a . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 // status = true
status = a . NewAccount ( "111111", 5000 );
 // status = true
status = a . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" );
 // status = true
balance = a . Account ( "123456" ). Balance ( );
 // balance = -2190
balance = a . Account ( "987654" ). Balance ( );
 // balance = 2980
balance = a . Account ( "111111" ). Balance ( );
 // balance = 4710
cout << a . Account ( "123456" );
/*
---8<---8<---8<---8<---8<---8<---8<---
123456:
   1000
 - 300, to: 987654, sign: XAbG5uKz6E=
 - 2890, to: 987654, sign: AbG5uKz6E=
 = -2190
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << a . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 290, from: 111111, sign: Okh6e+8rAiuT5=
 = 2980
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << a . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 290, to: 987654, sign: Okh6e+8rAiuT5=
 = 4710
---8<---8<---8<---8<---8<---8<---8<---
*/
status = a . TrimAccount ( "987654" );
 // status = true
status = a . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 // status = true
cout << a . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   2980
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 3103
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank c;
strncpy ( accCpy, "123456", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, 1000 );
 // status = true
strncpy ( accCpy, "987654", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, -500 );
 // status = true
strncpy ( debCpy, "123456", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 300, signCpy );
 // status = true
strncpy ( debCpy, "123456", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
 // status = true
strncpy ( accCpy, "111111", sizeof ( accCpy ) );
status = c . NewAccount ( accCpy, 5000 );
 // status = true
strncpy ( debCpy, "111111", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
 // status = true
balance = c . Account ( "123456" ). Balance ( );
 // balance = -2190
balance = c . Account ( "987654" ). Balance ( );
 // balance = 5580
balance = c . Account ( "111111" ). Balance ( );
 // balance = 2110
cout << c . Account ( "123456" );
/*
---8<---8<---8<---8<---8<---8<---8<---
123456:
   1000
 - 300, to: 987654, sign: XAbG5uKz6E=
 - 2890, to: 987654, sign: AbG5uKz6E=
 = -2190
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << c . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 = 5580
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << c . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 = 2110
---8<---8<---8<---8<---8<---8<---8<---
*/
status = c . TrimAccount ( "987654" );
 // status = true
strncpy ( debCpy, "111111", sizeof ( debCpy ) );
strncpy ( credCpy, "987654", sizeof ( credCpy ) );
strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
status = c . Transaction ( debCpy, credCpy, 123, signCpy );
 // status = true
cout << c . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   5580
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 5703
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank e;
status = e . NewAccount ( "123456", 1000 );
 // status = true
status = e . NewAccount ( "987654", -500 );
 // status = true
status = e . NewAccount ( "123456", 3000 );
 // status = false
status = e . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" );
 // status = false
status = e . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" );
 // status = false
status = e . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" );
 // status = false
balance = e . Account ( "666" ). Balance ( );
 // exception thrown
cout << e . Account ( "666" );
 // exception thrown
status = e . TrimAccount ( "666" );
 // status = false

CBank g;
status = g . NewAccount ( "123456", 1000 );
 // status = true
status = g . NewAccount ( "987654", -500 );
 // status = true
status = g . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 // status = true
status = g . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 // status = true
status = g . NewAccount ( "111111", 5000 );
 // status = true
status = g . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
 // status = true
CBank h ( g );
status = g . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 // status = true
status = h . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
 // status = true
status = g . NewAccount ( "99999999", 7000 );
 // status = true
status = g . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
 // status = true
status = g . TrimAccount ( "111111" );
 // status = true
status = g . Transaction ( "123456", "111111", 221, "Q23wr234ER==" );
 // status = true
cout << g . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   -1802
 + 221, from: 123456, sign: Q23wr234ER==
 = -1581
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << g . Account ( "99999999" );
/*
---8<---8<---8<---8<---8<---8<---8<---
99999999:
   7000
 + 3789, from: 111111, sign: aher5asdVsAD
 = 10789
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << g . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 + 123, from: 111111, sign: asdf78wrnASDT3W
 = 5703
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << h . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
 = 1321
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << h . Account ( "99999999" );
 // exception thrown
cout << h . Account ( "987654" );
/*
---8<---8<---8<---8<---8<---8<---8<---
987654:
   -500
 + 300, from: 123456, sign: XAbG5uKz6E=
 + 2890, from: 123456, sign: AbG5uKz6E=
 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
 + 789, from: 111111, sign: SGDFTYE3sdfsd3W
 = 6369
---8<---8<---8<---8<---8<---8<---8<---
*/

CBank i;
CBank j;
status = i . NewAccount ( "123456", 1000 );
 // status = true
status = i . NewAccount ( "987654", -500 );
 // status = true
status = i . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
 // status = true
status = i . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
 // status = true
status = i . NewAccount ( "111111", 5000 );
 // status = true
status = i . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
 // status = true
j = i;
status = i . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
 // status = true
status = j . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
 // status = true
status = i . NewAccount ( "99999999", 7000 );
 // status = true
status = i . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
 // status = true
status = i . TrimAccount ( "111111" );
 // status = true
cout << i . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   -1802
 = -1802
---8<---8<---8<---8<---8<---8<---8<---
*/
cout << j . Account ( "111111" );
/*
---8<---8<---8<---8<---8<---8<---8<---
111111:
   5000
 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
 = 1321
---8<---8<---8<---8<---8<---8<---8<---
*/

