// document
// alert(document.documentElement.nodeType); // Type element = 1
// alert(document.body.nodeType); // Type element = 1
// alert(document.doctype.nodeType); // type document_type = 10

var html = document.documentElement;
// alert(html.nodeName); // HTML
// alert(html.value); // undefined

// alert(document.title); // k textu uvnitr title
// document.URL, .domain, .referer

// childNodes (x children) uzly a texty
// alert(document.childNodes[1].nodeName); // #text
// alert(document.childNodes[1].nodeValue); // text v komentari))

// alert(html.childNodes[0].nodeName);
// alert(html.childNodes[0].nodeValue);
// alert(html.childNodes[0].nodeType);

var potomekBody = document.body.firstChild; // volny text
// lastChild
// alert(potomekBody.nodeName);
// alert(potomekBody.nodeType);
// alert(potomekBody.nodeValue);
// nextSibling

// alert(document.body.childNodes[5].nodeName); // P
// alert(document.body.childNodes[5].nodeValue); // null
// alert(document.body.childNodes[5].nodeType); // 1 - element

var p = document.body.childNodes[5];
var dalsi = p.firstChild;
// alert(dalsi.nodeValue); // text v paragrafu
