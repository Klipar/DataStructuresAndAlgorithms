## Searching for rational numbers in a sequence
Write a program that reads an increasing sequence of up to 50,000 rational numbers (0–10 decimal places, using only integer/char types, no float), then answers up to 100,000 queries by returning the 1-based position of each queried number in the sequence, or 0 if not found.

## Generating prime numbers
Write a program that reads two integers (0–1000000) defining an interval and outputs all prime numbers within that interval (inclusive), separated by a comma and a space.

## Greatest common divisor
Write a program that reads N pairs of natural numbers (each up to 18446744073709551615) and outputs the greatest common divisor for each pair, one result per line.

## Brackets
Write a program that reads N sequences of bracket pairs ( ), [ ], <>, { } (nesting allowed) and for each sequence counts the number of errors — a missing opening/closing bracket or mismatched pair types — outputting each count on a separate line.

## Binary Decision Diagram (BDD) for Boolean functions
Create a program that builds a reduced Binary Decision Diagram (BDD) representing a given Boolean function, implementing BDD_create (build a reduced BDD from a Boolean expression and a variable order), BDD_create_with_best_order (try at least N different variable orders and return the BDD with the fewest nodes), and BDD_use (evaluate the function for a given input combination by traversing the BDD from root to leaf, returning '1', '0', or -1 on error). You may not use existing BDD libraries; reduction must be done (ideally incrementally), and correctness must be verified by testing all input combinations for functions with at least 13 variables and at least 100 different Boolean functions.

## Searching in dynamic sets
Implement and compare at least two of these three data structures — two balanced binary search trees (excluding AVL and Splay; e.g. red-black, (2,3), (2,3,4), weight-balanced) and a hash table with open addressing and double hashing (including resizing) — measuring insert, search, and delete performance per operation with a dedicated test program, and documenting which structure suits which usage scenario.