// objekty
var clovek = new Object();
// typeof Object
clovek.jmeno = "Pavel";
clovek.ID = 10;

// pomoci literalu
var zam = {jmeno: "Tana", ID: 20};
// alert(zam['jmeno']);

// pole
var vektor = new Array();
// alert(vektor.length);
// alert(vektor[0]);
vektor[5] = 10;
// alert(vektor[5]);
// alert(vektor.length); // 6
vektor.length = 2;
// alert(vektor[5]); // undefined

// okmazita inicializace
var barvy = new Array('bila', 'modra');
// var cisla = new Array(1, 2, 3);
// var cisla = Array(2); // pole delky 2 s undefined prvky

// pomoci literalu
var dalsi = [1, 2, 3];

// pole referenci nebo hodnoty datovy typ? - referenci
var pole = dalsi;
dalsi[0] = 0;
// alert(pole[0]);

// prazdne pole
var prazdne = [];
// alert(prazdne);
// alert(barvy.toString()); // zretezi a oddeli carkou
// alert(barvy); // chova se stejne jako s toString

// barvy.push('zrzava'); // navratova hodnota je novy pocet prvku
// alert(barvy);
// barvy.pop(); // ubira zeshora, navratova hodnota je hodnota prvku, ktery byl odebran
// alert(barvy);
// barvy.shift();
// alert(barvy); // ubira zespoda, navratova hodnota je hodnota prvku, ktery byl odebran

var cisla = [1, 2, 3, 4, 5];
cisla.reverse(); // obrati poradi prvku v poli
// alert(cisla);

cisla = [0, 1, 15, 2, 10];
cisla.sort();
// alert(cisla);

// vlastni porovnavaci funkce - u cisel staci pocitat p1 - p2
function compare(p1, p2) {
	if (p1 > p2) return 1;
	else if (p1 == p2) return 0;
	else return -1;
}
