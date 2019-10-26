var dog = {
	name: "dog",
	paws: 4,
	speak: function() {
		return "haf";
	}
};

var my_dog = Object.create(dog); // dog bude prototyp pro my_dog
// alert(my_dog.paws);
// alert(my_dog.speak());

// alert(dog.isPrototypeOf(my_dog)); // true

my_dog.hungry = true;

vlastnosti = "";

// for (v1 in my_dog) {
// 	vlastnosti += v1 + " ";
// }

// alert(vlastnosti);

// nove vlastnosti
var descriptor = {
	// value: 1, //default undefined
	// writeable: true, // def false
	enumerable: true, // def false
	configurable: true, // def. false = nelze delete, menit typ
	set: function(value) {
		vlastnost = 7 * value;
	},
	get: function(value) {
		return vlastnost;
	}
};

Object.defineProperty(dog, "vek", descriptor);

for (v1 in my_dog) {
	vlastnosti += v1 + " ";
}

// alert(vlastnosti);

dog.vek = 3;
// alert(dog.vek);

var your_dog = Object.create(dog, {
	sex: {
		set: function(value) {
			sex = value;
		},
		get: function() {
			return sex;
		},
		// value: "female"
	}
});

your_dog.sex = "male"; // nutno writebale nebo set a get
// alert(your_dog.sex);

// Object.seal(dog); // zabrani pridavat a ubirat vlastnosti
// Object.isSealed(dog);

// dog.color = "brown";
// alert(dog.color);
// delete dog.name;

// Object.freeze(dog); // zabranuje menit hodnoty
// Object.isFrozen(dog);

// dedicnost pomoct Object.create
var rodicProto = {
	vlastnostRodice: "rodic",
	vypisVlastnostRodice: function() {
		alert(this.vlastnostRodice);
	}
};

var dedicProto = Object.create(rodicProto, {
	vlastnostDedice: {
		value: "dedic",
		writable: true,
	},
	vypisVlastnostDedice: function() {
		alert(this.vlastnostDedice);
	}
});

var instanceR = Object.create(rodicProto);
var instanceD = Object.create(dedicProto);