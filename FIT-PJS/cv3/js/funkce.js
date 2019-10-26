// funkce lze z funkci i vracet
function vytvorFunkci (koef1, koef2) {
	return function(x) {
		return koef1 * x + koef2;
	}
}

var vytvorena1 = vytvorFunkci(2, 0);
var vytvorena2 = vytvorFunkci(1, 1);

// alert(vytvorena1(3) + "; " + vytvorena2(3));
// alert(vytvorFunkci(2,3)(3)); // okamzite volani vytvorene fce

// v JS se parametry predavaji vzdy hodnotou

// deinovane parametry ktetre nebyly pri volani pouzity, maji automaticky hodnotu undefined

// interni objekt this - odkaz na obor platnosti (objekt) ve kt. je dana fce provozovana
// jmeno = "Franta";
// var jmeno = "Franta";
window.jmeno = "Franta"; // window je globalni objekt v prohlizeci (vsetky premenne a fce su na najvyssej urovni jeho vlastnosti a metody)

var clovek = {jmeno: "Eva"}; // definice objektu pomoci literalu

function vypisJmeno () {
	alert(this.jmeno);
}

// vypisJmeno(); // Franta - jsme v globalnim kontextu

clovek.vypis = vypisJmeno; // prirazeni fce vlastnosti objektu vznika metoda

// clovek.vypis(); // Eva - ted to volam v kontextu objektu clovek

// metody funkci apply a call - pouzivaju sa k nastaveniu this nezavisle na aktualnom kontexte

function suma (p1, p2) {
	return p1 + p2;
}

function volej1 (n1, n2) {
	return suma.apply(this, arguments); // pouziti apply s arguments
}

function volej2 (n1, n2) {
	return suma.apply(this, [n1, n2]); // pouziti apply s polem
}

function volej3 (n1, n2) {
	return suma.call(this, n1, n2); // pouziti call se seznamem argumentu
}

// hlavni pouziti apply a call je nastaveni oboru platnosti BEZ prirazeni fce jako metody objektu

// vypisJmeno.call(clovek); // Eva - nastavil som kontext (this) na clovek


// vlastnost callee objektu arguments - odkaz na fci, kt. vlastni objekt arguments (na danou fci)

function faktorial (n) {
	if (n <= 1) return 1;
	else 
		// return n * faktorial(n-1); // pozor - faktorial lze prepsat
		return n * arguments.callee(n-1); // zde nevystupuje jmeno funkce primo
}

// alert(faktorial(4));
var fakt = faktorial;
faktorial = function () { return 2; }

alert(fakt(4));

// prilad retezu promennych klasicky (jako v C, Java, ...)
var x = 5;

function zmenPromennu () {
	// pristupne x a y
	var y = 2;

	function prohodPromenne () {
		// pristupne x, y a tmp
		var tmp;

		tmp = y;
		y = x;
		x = tmp;
	}

	prohodPromenne(x, y);
	// alert("y=" + y);
}

// pristupne pouze x

zmenPromennu();
// alert("x=" + x);

// dynamicke uplatneni oboru platnosti
function vratFunkci (p) {
	return function () {
		return p;
	};
}

prvni = vratFunkci(1);
druha = vratFunkci(2);

alert(prvni()); // 1 - parametr davno ukoncene funkce ale jeji kontext zustava aktivni protoze na nej vede pointer vytvorene funkce

alert(druha());

// anonymni fce nemaji jmeno
// lze je pouzit (jejich definici) primo v miste parametru

function volej (i, j, f) {
	f(i,j);
}

volej(2,3,function (r, s) {
	// alert(r + s);
});

// uzavery (closures): funkce ktere maji pristup k promennym z obrou platnosti jine fce (casto jsou to fce vytvorene uvnitr jine fce)

// uzaver obdrzi posledni hodnotu promene z rodicovske fce
// function vytvorFunkce () {
// 	var poleFunkci = new Araay();

// 	for (var i = 0; i < 10; i++) {
// 		poleFunkci[i] = function () {
// 			return i;
// 		};
// 	}

// 	return poleFunkci;
// }

// var funkce = vytvorFunkce();
// alert(funkce[2]()); // vrati posledni hodnotu - 10

// pozor na this u anonymnich funkci - nejdeli o prime prirazeni vlastnosti objektu, jsou brany v kontextu global

var jmeno = "globalni";
var obj = {
	jmeno: "jmeno v objektu",
	fceZobrazJmeno: function () { // metoda ktera vraci anonymni fci
		return function () {
			return this.jmeno; // vracena fce neni vazana na vlastnost objektu - proto je v kontextu global
		};
	}
};

// alert(obj.fceZobrazJmeno()());

// pouziti anonymnich funkci k vytvareni lokalnich promennych BLOKU

(function () { // blok s lokalnimi promennymi
	var z = "uvnitr";
	// alert(z);
})(); // okamzite vyvolani 

// alert(typeof(z)); // uz neexistuje - undefined

(function () {
	// privatni vlastnosti a metody
	var privatniPromenna = 3;

	// klasicka definice fce je vzdy privatni v obklopujici fci
	function privatniFce (x) {
		return x + x;
	}

	// definice konstruktoru
	MujObjekt = function() { // prirazeni bez var = chci globalni promennou
		// prazdny konstruktor
	};

	// verejna metoda
	MujObjekt.prototype.verejnaFce = function () {
		return privatniFce(privatniPromenna++); // pouziva soukromne cleny
	};

})();

var obj = new MujObjekt();
alert(obj.verejnaFce());
alert(obj.verejnaFce());