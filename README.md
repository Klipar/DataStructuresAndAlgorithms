4) Zátvorky / Brackets

Vašou úlohou je analyzovať zadané postupnosti a spočítať množstvo chýb v každej postupnosti, a toto množstvo chýb vypísať na výstup.
Postupnosti pozostávajú z párov zátvoriek typu ( ), [ ], <> alebo { }. Chybou je, ak nejaký pár nemá začínajúcu alebo končiacu zátvorku alebo ak nejaký pár pozostáva z rozličných zátvoriek ako napr. ( ] alebo <}. Zátvorky sa môžu do seba ľubovoľne veľa-krát vnárať.
Na vstupe je najprv číslo N (v rozsahu od 1 do 200) vyjadrujúce počet postupností, pričom každá postupnosť sa nachádza na samostatnom riadku. Dĺžka jednej postupnosti môže byť 1 až 100000 znakov.
Výstup pozostáva z počtov chýb, pričom tieto čísla sú vypísané v samostatných riadkoch.
------------------------------------------------------------
Your task is to analyze the entered sequences and count the number of errors in each sequence, and output this number of errors.
Sequences consist of pairs of parentheses of the type ( ), [ ], <>, or { }. It is an error if a pair does not have a starting or ending parenthesis or if a pair consists of different parentheses, such as ( ] or <}. Parentheses can nest into each other any number of times.
At the input there is first a number N (in the range from 1 to 200) expressing the number of sequences, while each sequence is located on a separate line. The length of one sequence can be from 1 to 100000 characters.
The output consists of error counts, and these numbers are written on separate lines.

Do 14.03.2025, 12:00
Ukážkový vstup

5
{[]([]{}){[]{}}}
((([]()){}]}
[{([}]}
{[]
({})]
Ukážkový výstup

0
2
3
1
1