function Rodic() {
	this.vlastnostRodice = "rodic";
}

Rodic.prototype.nactiVlastnostRodice = function() {
	return this.vlastnostRodice;
}

function Dedic() {
	this.vlastnostDedice = "dedic";
}

Dedic.prototype = new Rodic();
Dedic.prototype.nactiVlastnostDedice = function() {
	return this.vlastnostDedice;
}

var instance = new Dedic();

// alert(instance.nactiVlastnostDedice());
// alert(instance.nactiVlastnostRodice());

// konstruktor
// alert(instance instanceof Dedic); // true
// alert(instance instanceof Rodic); // true
// alert(instance instanceof Object); // true

// alert(instance.constructor == Dedic); // false
// alert(instance.constructor == Rodic); // true
// alert(instance.constructor == Object); // fakse

// prototype
// alert(Dedic.prototype.isPrototypeOf(instance));

// prepsani metody rodice
Dedic.prototype.nactiVlastnostRodice = function() {
	return "jsem prepsana";
};

alert(instance.nactiVlastnostRodice());
irodic = new Rodic();
alert(irodic.nactiVlastnostRodice());

Dedic.prototype.nactiVlastnostDediceNaVelka = function() {
	return this.vlastnostDedice.toUpperCase();
}

// dedicnost pres volani konstruktor rodice
function Rodic2(jmeno) {
	this.jmeno = jmeno;
}

function Dedic2(jmeno, vyska) {
	Rodice2.call(this, jmeno);
	this.vyska = vyska;
}

var instance2 = new Dedic2("Franta", 185);
alert(instance2.constructor == Object);
alert(instance2.constructor == Rodic);
alert(instance2.constructor == Dedic);
