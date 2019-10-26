// alert("hello, Jirko");

// alert(function() {
// 	return 10;
// }());

// <![CDATA[
/*   */
// ]]>

// definice promennych
var i, j = 10, k; // restrikce oboru platnosti na kontext objektu Global, v browseru Window
p = 100; // globalni promenna
jmeno = "Lukáš";
// alert(i); // undefined

if (p == 100) {
	var x = 5; // stale v objektu Global
}

// alert(x);

// datove typy: Undefined, Boolean, Number, String, Object
// vlastni: pole, Object
// alert(typeof i); // undefined
// alert(typeof jmeno); // String
// alert(neni);
// alert(typeof neni);

// alert(null);
// alert(typeof null);

// alert(null == undefined); // TRUE
// alert(null === undefined); // FALSE

// Boolean
// automaticka konverze napr. if (...): false z "", 0, null, NaN, undefined
// true z neprazdny string, nenull, cisla, objekty z mimo null

// number je doble
// y = 10;
// alert(typeof y); // number
var num = 012; // oktalove
// alert(typeof num);
num = 0x1A;
// alert(num);
dec = 1.2;
dec = 1.2e3;
dec = 0.1 + 0.2;
// alert(dec);
// dec = Number.MAX_VALUE + Number.MAX_VALUE;
// alert(dec);
// alert(typeof dec);
// alert(isFinite(dec));

// alert(0/0);
// alert(NaN == NaN); // nerovna se nicemu -> false
// alert(isNaN(0/0)); // true

// konverze funkci Number
// boolean -> 1, 0
// null -> 0
// undefined -> NaN
// stringy "" -> 0, cifry -> .. "0x12" -> cislo
// ostatni NaN

// funkce parseInt
// umi i oktalova. hexa - chape oktalove
// parseFloat - ignoruje vedouci nuly, u hexa vraci 0

// alert(parseFloat("     -012.34567e3"));

// String - nepromenne
// escape sekvence \, i znaky v unicode
// alert("abcs".length);
// konverze toString()
var abc = (10).toString();
// alert(abc);
// alert(typeof abc);

// funkce String
// alert(String(undefined));
// pozdrav("Lukáš", "Hromadník", "je", "naprosto", "největší", "borec"); //

function pozdrav(jmeno) {
	alert("Hello, " + jmeno);
}

// redefinice
function pozdrav(jmeno) {
	alert(jmeno);
}

function soucet(x, y) {
	var sum = 0;
	if (arguments.length > 2)
		for (var i in arguments)
			sum += arguments[i];
	else
		sum = x + y;

	alert(sum);
}

// soucet(10, 20, 40);

function moje() {
	var k = 5;
	alert(k);
	return k * 2;
}

alert(k);
vysledek = moje();
alert(vysledek);
alert(k);