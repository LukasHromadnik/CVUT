; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"
.cseg

; Zacatek programu - po resetu
.org 0
	jmp start

.org	0x100
retez:	.db	"VLAK PRIJIZDI DO STANICE",0	; retezec zakonceny nulou

clear_disp:
	ldi r24, 5
	ldi r16, ' '
	ldi r17, 2
	clearing:
		call show_char
		inc r17
		dec r24
		brpl clearing
	ret

showspaces:
	ldi r16, ' '
	ldi r22, 0				; pocet mezer
	show:
		call show_char
;		dec r20
		inc r17
		inc r22
		dec r18
		brpl show

	sub r21, r22
	mov r18, r21		; pocet pismen - 1
	cycle3:
		sbiw r30, 1		; dekrementace Z
		dec r18			; aktualizace smycky
		brpl cycle3		; "hop"

	ret

; Zacatek programu - hlavni program
start:
    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp
init:
	ldi	r30, low(2*retez)	; dolni bajt bajtove adresy retezce
	ldi	r31, high(2*retez)	; horni bajt bajtove adresy retezce
	ldi r20, 0				; pocatecni stav: r20 + 1 = pocet vypsanych pismen na displeji
	ldi r22, 0				; vynulovani pomocneho registru
	
loadstr:
	ldi r17, 7				; posledni mozna pozice
	sub r17, r20			; nastaveni aktualni pozice zacatku vypisu
	mov r18, r20			; zacatek smycky
	mov r21, r20			; pomocny registr pro dekrementaci Z

	; Vypis znaku na displej
	cycle:
		lpm	r16, Z+			; nahraj bajt (znak) z retezce do r16, posun pozici v retezci
		and r16, r16
		brne showchar		; pokud neni nactena nula - vypis znak
		call showspaces		; dopln volna mista mezerami
		jmp pause			; pokracuj v programu
		showchar:
			call show_char	; zobraz znak
			inc r17			; posun pozici
			dec r18			; aktualizace smycky
			brpl cycle		; pokud je r18 kladne
	
	cont:
	cpi r20, 5				; pokud se na displeji vypisuji pismena od prvni pozice -
	breq regular_str		;  - odcitej pomoci konstatniho cisla

	; Dekrementace Z o pocet vypsanych pismen
	decZ:
		mov r18, r21		; pocet pismen - 1
		cycle2:
			sbiw r30, 1		; dekrementace Z
			dec r18			; aktualizace smycky
			brpl cycle2		; "hop"

		inc r20				; inkrementace poctu pismen
		jmp pause			; preskoc dekrementaci Z pomoci konstanty

	regular_str:
		sbiw r30, 5			; dekrementace Z pomoci konstanty

	pause:					; cekaci smycky

	ldi r21, 255
	cycle1:
		ldi r22, 10
			ldi r23, 30
		cycle2:
				cycle3:
					dec r23
					brne cycle3

		dec r21
		brne cycle1

			dec r22
			brne cycle2


	call clear_disp			; smazani displeje
	cpi r22, 6				; pokud je text na konci
	breq init				; spusti se znovu program od zacatku
	jmp loadstr				; "hop" na zacatek

end:
	jmp	end
