// automaticke prevody
// Number
// objekty na cisla pomoc valueOf (vestaveny objekt Date)
// objekty nebo cisla na stringy pomoc toString, stringy

// typeof, instanceof

var i, j, k;
i = j = k = 10;
i += 2;

// aritmeticke operatory
// x = 'abc';
// y = 5;
// z = x + y;
// alert(z); // abc5
// z = y + x;
// alert(z); // 5abc

a = 5 / 0; // infinity, typ je Number

// unarni operatory
i = +true;
// alert(1); // 1

j = 'abc';
// alert(+j); // NaN

// inkrementace, dekrementace, pre, post

// bitove operatory ~ & | ^ nezkracene vyhodnocovani
// << >>
// >>> vpravo, bezznamenkovy

// alert(null || 1);
// alert(null || null);
// alert(NaN || 1);
// alert(NaN || NaN);

// alert(10 < NaN); // false
// alert(10 >= NaN); // false

// relacni operatory: < > <= >=
// == != ===

// ternarni operatory
i = (j > k) ? i : j;
var x = (1, 2, 3, 4);
// alert(x); // 4

// alert(null == undefined) // true;
// var operators = [null, undefined, NaN, false, true]

var clovek = new Object();
clovek.ID = 10;
clovek.jmeno = 'jindra';
clovek.prijmeni = 'benes';
var vlastnosti = '';

for (var vlastnost in clovek) {
	vlastnosti += vlastnost + " = " + clovek[vlastnost] + ";";
}
alert(vlastnosti);