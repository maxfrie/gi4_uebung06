; Konstanten
LEN_HEX equ 8
LEN_DEZ equ 10
LEN_STRUKTUR equ LEN_HEX + 1 + LEN_DEZ + 1

SECTION .data
	eingabewert DD 42 ; zu verarbeitender Wert
	base10 DD 10 ; Basis Dezimalzahlen
	base16 DD 16 ; Basis Hexadezimalzahlen

SECTION .text
	global main
	extern malloc
	extern free

main:
	push dword [eingabewert] ; Argument auf Stack legen
	call convert ; Funktion aufrufen. Pointer auf Struct in eax

	add esp, 4 ; Argument von Stack nehmen

	; allokierten Speicher freigeben
	push eax
	call free
	add esp, 4

	mov ebx, 0 ; exit(0)
	mov eax, 1 ; Programm erfolgreich ausgeführt
	int 0x80
	
convert:  ; struct* convert(unsigned int zahl)
	push ebp	; neuer Stackframe
	mov ebp, esp

	push ebx ; C-Calling Convention: sicherstellen
	push ecx
	push edx

	; Speicher für Rückgabe anfordern
	push LEN_STRUKTUR ; Grösse der Struktur
	call malloc
	add esp, 4
	mov ebx, eax ; in ebx Basisadresse der Struktur

	;
	;
	; Hexadezimale Darstellung
	mov eax, [ebp+8] ; Funktionsargument in eax laden
	mov ecx, LEN_HEX ; Maximallänge Hex. als Anzahl der Durchläufe
	
hex:
	mov edx, 0 ; für Division edx zu Null setzen
	div dword [base16] ; eax = edx:eax /arg, Rest in edx
	cmp edx, 10	; Prüfen, ob Korrektur der Darstellung notwendig
	jl hex_noletter	; Rest kleiner als 10 -> kein Buchstabe in  Hex
	add edx, 0x37	; addiere 0x37, da 0x37 + 10 =0x41 = "A", für ASCII
	jmp hex_weiter

hex_noletter:
	add edx, 0x30	; addiere 0x30 = "0" für ASCII
hex_weiter:
	mov [ebx+ecx-1], dl ; aktuelle Stelle der Hexadezimalzahl schreiben
	loop hex
	mov byte [ebx+LEN_HEX], 0 ; Nullterminierung setzen

	;
	;
	; Dezimale Darstellung
	mov eax, [ebp+8]
	mov ecx, LEN_DEZ

dez:
	mov edx, 0
	div dword [base10]

	add edx, 0x30
	mov [ebx+LEN_HEX+1+ecx-1], dl
	loop dez
	mov byte [ebx+LEN_HEX+1+LEN_DEZ], 0

	;
	;
	; Rückkehr aus Funktion
	mov eax, ebx ; Rückgabewert (Adresse der Datenstruktur in eax)
	
	pop edx
	pop ecx
	pop ebx

	mov esp, ebp
	pop ebp
	ret
