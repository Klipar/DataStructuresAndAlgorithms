3) Hľadanie racionálnych čísel v postupnosti / Searching for rational numbers in a sequence

Vašou úlohou je vytvoriť program, ktorý najprv načíta rastúcu postupnosť racionálnych čísel túto postupnosť si zapamätá. Na riešenie zadania môžete používať iba dátové typy pre celé čísla (napr. int alebo long) a znaky (char) - t.j. nie float a podobne. Tieto racionálne čísla majú 0 až 10 desatinných miest (t.j. cifier za čiarkou).
Následne bude tento program dostávať na vstupe ľobovoľné racionálne číslo (v rovnakom formáte ako čísla tvoriace postupnosť uvedenú vyššie), pričom podstatou tohto programu je pre každé zadané číslo nájsť dané číslo v rámci zadanej postupnosti a vrátiť pozíciu (t.j. poradové číslo) nájdeného čísla. Poradie čísel sa čísluje od čísla 1. Ak sa zadané číslo v postupnosti nenachádza, program vráti číslo 0.
Vstupom je najprv prirodzené číslo N (v rozsahu od 1 do 50000) vyjadrujúce veľkosť postupnosti. Naleduje N riadkov, pričom každý riadok obsahuje jedno číslo z postupnosti.
Za touto postupnosťou nasleduje prirodzené číslo M (v rozsahu od 1 do 100000) vyjadrujúce počet hľadaných čísel. Nasleduje M riadkov, pričom každý riadok obsahuje jedno číslo, ktoré je potrebné nájsť v zadanej postupnosti.
Výstupom sú čísla reprezentujúce poradie zadaných hľadaných čísel v postupnosti, pričom každé číslo je v samostatnom riadku.
----------------------------------------------------------
Your task is to create a program that first reads an increasing sequence of rational numbers and remembers this sequence. You can only use data types for whole numbers and characters (e.g. int, long or char) to solve the assignment, not float. These rational numbers have a precision of 0 to 10 digits after the decimal point.
Subsequently, this program will receive an arbitrary rational number at the input (in the same format as the numbers forming the sequence mentioned above), while the essence of this program is to find the given number within the specified sequence for each entered number and to return the position (i.e. ordinal number) of the found number. The sequence of numbers is numbered from number 1. If the entered number is not in the sequence, the program returns number 0.
The input is first a natural number N (in the range from 1 to 50000) expressing the size of the sequence. There are N rows, each row containing one number from the sequence.
This sequence is followed by a natural number M (in the range from 1 to 100000) expressing the number of searched numbers. There are M lines, each line containing one number to be found in the given sequence.
The output is numbers representing the order of the entered search numbers in sequence, with each number in a separate line.

Do 07.03.2025, 12:00
Ukážkový vstup

10
-849243,9810
-555,434
-555,230
-40,10993
0,231
3,141592
239
10934,234
234301,0
294348134
6
-555,23
808
239
0,231
234301,0
-555,434
Ukážkový výstup

3
0
7
5
9
2