# Übung 6: Funktionen und parallele Programmierung I

## Funktionen mit Assembler
Schreiben Sie eine 32Bit Assemblerfunktion, welche eine 32Bit breite nichtnegative Ganzzahl als Parameter übergeben bekommt und einen Zeiger auf die nachfolgend dargestellte C-Datenstruktur zurückgibt. Die Datenstruktur enthält die jeweilige Darstellung der Ganzzahl als C-Zeichenkette in hexadezimaler und in dezimaler Schreibweise.

	struct Darstellung {
		char hex[9];
		char dezimal[11];
	};

**Hinweise:** Die Instruktion div teilt den Wert aus `edx:eax durch` das Argument und speichert das Ergebnis in `eax`. Der Rest der Division wird in `edx` gespeichert. Laut der ASCII- Tabelle entspricht das Zeichen 0 dem Wert 0x30, während der Buchstabe A dem Wert 0x41 entspricht. Sie können Speicher mit der C-Funktion `int malloc(int anzahl_bytes);` anfordern. 

## Parallele Programmierung I
In dieser und in der nächsten Übung wird die näherungsweise Lösung des linearen Gleichungssystems

	Ax=b
	
also

	&sum; a(i,j)x(j) = b(i)

ermittelt. Dabei soll das Jacobi-Verfahren verwendet werden, welches die i-te Gleichung nach xi auflöst. Hieraus ergibt sich folgende Iterationsvorschrift für den m+1-ten Iterati-onsschritt:

	x(i)^(m+1) = 1 / a(i,i) (b(i) - &sum; a(i,j)^m

Um zu erkennen, ob das Iterationsverfahren die Lösung gefunden hat, soll der Euklidische Abstand zwischen x^m und x^(m+1) verwendet werden. Ist dieser sehr klein (bei uns kleiner als `&radic; (n 􏰄􏰂￼0.0000001 · n)`), wurde der Lösungsvektor gefunden und das Iterationsverfahren kann abgebrochen werden. Der Abstand wird wie folgt bestimmt:

	&radic; (&sum; ((x(i)^m - x(i)^(m+1))^2))
	
1. Dieses git-Repository enthält ein C-Programm, das die Matrizen für diese Aufgabe initialisiert. Analysieren Sie die Datei `main.c` und vollziehen Sie den Code nach!
2. Lösen Sie das lineare Gleichungssystem, welches das gegebene Programm selbst erzeugt, sequentiell mit dem Jacobi-Verfahren. Verwenden Sie den Euklidischen Abstand, um zu bestimmen, ob das Iterationsverfahren abzubrechen ist. Tragen Sie in der Datei `main.c` Ihre Lösung an der Stelle ein, die durch den Kommentar *TODO* gekennzeichnet ist.
3. In der nächsten Übung wird der obige Gleichungslöser mit Hilfe von Threads und SSE-Instruktionen parallelisiert. Überlegen Sie sich, wie Sie dies realisieren wollen und skizzieren Sie Ihre Strategie.