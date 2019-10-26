var clovek = new Object();
clovek.id = 1;
clovek.jmeno = "Franta";
clovek.vypisJmeno = function() {
	alert(this.jmeno);
};

// funkce v typu factory
function vytvorCloveka($id, $name) {
	var c = new Object();
	c.id = $id;
	c.jmeno = $name;
	c.vypisJmeno = function() {
		alert(this.jmeno);
	};

	return c;
}

var clovek1 = vytvorCloveka(1, "Igor");
// clovek1.vypisJmeno();

// alert(typeof clovek1);
// alert(clovek1 instanceof Object);

// konstruktory
// function Clovek($id, $jmeno) {
// 	this.id = $id;
// 	this.jmeno = $jmeno;
// 	this.vypisJmeno = function() {
// 		alert(this.jmeno);
// 	};
// }

// Clovek(5, "XXX"); // obycejne globalnni promenne alert(id)
clovek2 = new Clovek(2, 'Lukáš');
// clovek2.vypisJmeno();
// alert(clovek2 instanceof Object); // true
// alert(clovek2 instanceof Clovek); // true
// alert(clovek1.constructor == Object); // true
// alert(clovek1.constructor == Clovek); // false
// alert(clovek2.constructor == Object); // false
// alert(clovek2.constructor == Clovek); // true

clovek3 = new Clovek(3, 'Jana');
// alert(clovek2.vypisJmeno == clovek3.vypisJmeno); // false

// function vypisJmeno() {
// 	alert(this.jmeno);
// }

// function Clovek($id, $name) {
// 	this.id = $id;
// 	this.jmeno = $name;
// 	this.vypisJmeno = vypisJmeno;
// }

// clovek4 = new Clovek(4, 'Igorek');
// clovek4.vypisJmeno();

// uziti prototype
function Clovek() {
	//
}

Clovek.prototype.id = 4;
Clovek.prototype.jmeno = 'Hanka';
Clovek.prototype.vypisJmeno = function() {
	alert(this.jmeno);
};
Clovek.prototype.jazyky = ['JS', 'PHP'];

clovek5 = new Clovek();
// clovek5.vypisJmeno();
clovek6 = new Clovek();
clovek6.jmeno = 'Martin';
// clovek6.vypisJmeno();
// clovek5.vypisJmeno();

// overovani prototype
// alert(Clovek.prototype.isPrototypeOf(clovek5)); // true
// alert(clovek5 instanceof Clovek); // true

// delete clovek6.jmeno; // smaze prekryti
// clovek6.vypisJmeno();

clovek6.jazyky[0] = "Java";
// alert(clovek6.jazyky[0]); // u referenční vlastnosti docházi ke změně v prototypu

// enumerovatelné property for - in zatím vlastní
// alert(clovek5.toString());
clovek5.toString = function() {
	return "clovek5";
}
vlastnosti = '';
for (v in clovek5)
	vlastnosti += v + " ; ";
// alert(vlastnosti); // po predefinovani toString (vlastni property), patri do seznamu

// prototype pomoci literalu
function Clovek() {
	//
}
Clovek.prototype = {
	constructor: Clovek,
	id: 7,
	jmeno: 'Tonda',
	vypisJmeno: function() {
		alert(this.jmeno);
	}
}
clovek7 = new Clovek();
// alert(clovek7.constructor == Clovek); // true je-li v prototypu, jinak false
// clovek7.vypisJmeno();

function Clovek(id, jmeno) {
	this.id = id;
	this.jmeno = jmeno;
	this.jazyky = ['JS', 'PHP'];
}

Clovek.prototype = {
	constructor: Clovek,
	vypisJmeno: function() {
		alert(this.jmeno);
	}
};

// kompaktni - dynamicky prototype
function Clovek(id, jmeno) {
	this.id = id;
	this.jmeno = jmeno;
	this.jazyky = ['JS', 'PHP'];

	if (typeof this.vypisJmeno != 'function') {
		Clovek.prorotype = {
			constructor: Clovek,
			vypisJmeno: function() {
				alert(this.jmeno);
			}
		};
	}
}





