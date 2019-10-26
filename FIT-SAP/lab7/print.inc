;---------------------------------
; init_disp - inicializace displeje. Nutno zavolat pred zobrazovanim na displeji.
init_disp:
	ldi r16, 0xB7
	sts LCDCRB, r16
	ldi r16, 0x10
	sts LCDFRR, r16
	ldi r16, 0x0F
	sts LCDCCR, r16
	ldi r16, 0x80
	sts LCDCRA, r16
	ret

;---------------------------------
; show_char - zapis znak na danou pozici na displeji
; ( r16 = co, r17 = kam )
; (zalohuje a obnovuje registry)
show_char:
	;ldi r16, 'F' ;co
	;ldi	r17, 3   ;kam
	push r0
	push r1
	push r16
	push r17
	push r18
	push r19
	push XL
	push XH
	push ZL
	push ZH
	subi r17, 2     ; upravime cislo pozice
	cpi r16, ' '
	brne nemezera
	ldi r16, ',' 	; misto mezery nedefinovana carka
nemezera:
	subi r16, '*'	; acsii -> index do tab. znaku
	;;mov r1, r16
	lsl r16
	lsl r16  ; r16=4*r16
	ldi	ZL, low(znaky<<1)
	ldi	ZH, high(znaky<<1)
	clr r0
	add ZL, r16
	adc ZH, r0  ; Z ... adresa tvaru znaku
	; vypocti pozici
	ldi XL, LCDDR0
	clr	XH
	mov r18, r17 	; uchovej pozici
	lsr r17 		; posun v bytech
	add XL, r17		; baze pro seg.
	ldi r19, 4 		; bajtu z tabulky znaku
	ldi r16, 0xf0 	; maska, abychom neprepsali vedlejsi znak
	sbrc r18, 0 	; preskoc swap masky, je-li sudy znak (od 0)
	swap r16
zobzc:
	lpm  r0, Z+
	sbrc r18, 0		; preskoc swap, je-li sudy znak
	swap r0
	ld   r1, X		; nacteme z displeje segmenty patrici 2 znakum
	and  r1, r16	; vynulujeme ty, co patri nasemu
	or	 r1, r0		; nastavime podle naseho znaku
	st   X, r1 		; zapiseme zpatky na displej
	adiw XL, 5		; jdeme na dalsi radek segmentu
	dec  r19
	brne zobzc

	pop ZH
	pop ZL
	pop XH
	pop XL
	pop r19
	pop r18
	pop r17
	pop r16
	pop r1
	pop r0

	ret

;============ sem vlozime tabulku znaku, lze vlozit i jinam
.include "znaky.inc"
