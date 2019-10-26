// object window = instance okna prohlížeče
// systemova dialogova okna: alert, confirm, prompt
// var volba = confirm('stiskni tlacitko');
// if (volba)
// 	alert('stiknuto OK');
// var jmeno = prompt('jmeno:', 'Jindra'); // null, implicitni, nove
// if (jmeno != null && jmeno != '')
// 	alert(jmeno);

function zobrazOkno() {
	window.open('http://www.seznam.cz');
}

// _self - současné okno
function zobrazOkno2() {
	var newWin = window.open('http://www.seznam.cz', '_blank', 'toolbar=yes, loaction=yes, status=no, menubar=yes, scrollbars=no, resizeable=no, width=400, height=400');
	// newWin.close();
	// if (newWin.closed())
}

// pozice okna - screenLeft, Top, screenY, screenY
var left = (typeof window.screenLeft == "number") ? window.screnenLeft : window.screenX;
var top = (typeof window.screenTop == "number") ? window.screnenTop : window.screenY;
// alert(left + "; " + top);

// posun okna
// window.moveTo(100, 400);
// window.moveBy(10, 20);

// vnitrni sirka a vyska
var sirka = window.pageWidth;
var vyska = window.pageHeight;

// if (typeof sirka != "number") {
// 	if (document.compatMode == "CSS1Compat") {
// 		sirka = document.documentElement.clientWidth;
// 		vyska = document.documentElement.clientHeight;
// 	}
// 	else {
// 		sirka = document.body.clientWidth;
// 		vyska = document.body.clientHeight;
// 	}

// }
// alert(sirka + "; " + vyska); // mne to nefugnuje

// object screen
// availHeight, availWidth - bez panelu
// alert(screen.height + "; " + screen.width + "; " + screen.colorDepth);

// casovace, setTimeout, clearTimeout
// var pocet = 0;
// function poProdleve() {
// 	alert('haf');
// 	var id = setTimeout(poProdleve, 3000);
// 	pocet++;
// 	if (pocet >= 3)
// 		clearTimeout(id);
// }
// var id = setTimeout(poProdleve, 3000);

// object location
alert('pokracuji');
location.assing('http://www.seznam.cz'); // _self
window.location = 'http://seznam.cz';
location.href = 'http://seznam.cz';
location.replace(...) // nezapise do hsitorie

casti adresy
location.protocal, .hash, .search za ?, .hstname, .pathname

// reload znovu parametr true ze serveru. parametr false z cache