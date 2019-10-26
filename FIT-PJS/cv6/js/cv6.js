// pole
var jazyky = ['java', 'php', 'c#'];

// spojovani poli
var jazyky2 = jazyky.concat('python', ['js', 'perl']);
// alert(jazyky2);

// casti pole
jazyky3 = jazyky2.slice(2);
// alert(jazyky3);

var jazyky4 = jazyky2.slice(1, 4); // od indexu do indexu
// alert(jazyky4);

// vkladani a nahrazovani
var jazyky2 = jazyky.splice(2, 1, 'js', 'perl');
// alert(jazyky); // zmenene
// alert(jazyky2); // vymazane hodnoty

// mazani .splice(2, 2);

// typ Date
var now = new Date();
// alert(now);

var april = Date.parse('4/1/2014');
// alert(april); // milisekundy od 1. 1. 1970

var cerven = Date.UTC(2014, 5, 1);
// alert(cerven);

var mojeDatum = new Date(cerven);
// alert(mojeDatum);

// toString()
// alert(mojeDatum.toLocaleString());
// toLocalDateString, toLocaleTimeString

// Regularni vyrazy RegExp
// literalove
var pattern = /a(b+(c))/ig;
// i - ignore case, g - global, m multiline

// s konstruktorem
var pattern2 = new RegExp('a(b+(c))', 'ig');
// alert(pattern2.multiline);
// ignoreCase, global, lastIndex
// alert(pattern2.lastIndex); // 0
var matches = pattern2.exec('xyzabbbbcklamabcopg');
// alert(matches);
// alert(matches.index); // 3
// alert(matches[0]); // abbbc
// alert(matches[1]); // bbbc
// alert(matches[2]); // c

var matches = pattern2.exec('aaaabbbcccuto');
// alert(matches);

var matches = pattern2.exec('xyzabbbbcklamabcopg');
// alert(matches);

// var matches = pattern2.exec('xyz')

// obalove typy
var str2 = 'abcdef'.substr(2, 3);

// pro typ boolean
var falseObj = new Boolean(false);
// alert(falseObj.toString());
var result = falseObj && true;
// alert(result); // true

var falseVar = false;
// alert(falseVar.toString());
var result = falseVar && true;
// alert(result); // false

// typ NUMBER
var numObj = new Number(3);
// alert(numObj.toString());
// alert(numObj + 5);

var num = 3;
// alert(num.toString());
// alert(num + 5);

// alert(num.toFixed(2));
var num = 10;
// alert(num.toExponential(1));

num = 99;
// alert(num.toPrecision(1));
// alert(num.toPrecision(2));

// STRINGY
// .toString(), valueof
// .length charAt(1) - vyber znaku, charCodeAt(1)

var str = 'abcd';
// alert(str); // lze pristupovat ke stringu jako k poli znaku
// retezeni concat, +
// podretezce: slice, substring (umi i zaporne indexy)

// vyhledavani
// alert('aba'.indexOf('a'));
// lastIndexOf
// toUpperCase, toLowerCase

// regularni vyrazy
var pattern = /ab+c/g;
var matches = 'xyzabbbbcklamabcopg'.match(pattern);
// alert(matches); // vsechyn shody

var index = 'xyzabbbbcklamabcopg'.search(pattern);
// alert(index);

var nahrazeny = 'xyzabbbbcklamabcopg'.replace(pattern, 'KLM');
// alert(nahrazeny);
var text = "\"prvni\", 'druhy', 'treti'";
// alert(text.replace(/(['\"])([^'\"]*)\1/g, "[$2]"));

var jmena = "Jirka, Eva, Hanka";
var pole = jmena.split(", ", 3);
// str.localComepare() -1 0 1
// alert(pole);

// alert(String.fromCharCode(65, 66, 67, 68));

// Global - NaN, parseInt, ...
// eval
var prikazy = "i = 2; j = 3; alert(i + j);"
// eval(prikazy);

// Math
var x = Math.PI;
var e = Math.E;
// .max, Math.min(1, 2, 8, 0)

// random <0; 1>
var min = 1;
var max = 6;
for (i = 0; i < 10; i++) {
	var kostka = Math.floor(Math.random() * max + min);
	alert(kostka);
}
