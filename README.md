# Helsgaun (Lin-Kernighan) with Exact Passengers Assignment.

This algorithm is a wrapper for Helsgaun Algorithm (See at [LKH - Helsgaun](http://www.akira.ruc.dk/~keld/research/LKH/)) to solve the Traveling Salesman Problem with Passengers.

## Getting Started

1. You need to install Gurobi in your machine, the used version was `6.5.2`.
2. Go to the main application directory and compile the application: `make`
3. Run the application: `./lksolver.sh` or `./lksolver.sh 2>/dev/null` (to ignore the debug output).

## Example

Run the following commands:

```
$ make
$ ./lksolver.sh 2>/dev/null
10 10 3

0 434 197 483 431 494 450 457 269 189 
434 0 382 142 147 88 441 183 121 321 
197 382 0 300 177 295 117 493 302 214 
483 142 300 0 450 92 156 140 202 73 
431 147 177 450 0 224 200 433 475 478 
494 88 295 92 224 0 337 74 390 414 
450 441 117 156 200 337 0 284 391 318 
457 183 493 140 433 74 284 0 116 479 
269 121 302 202 475 390 391 116 0 212 
189 321 214 73 478 414 318 479 212 0 

263 0 4
263 1 4
263 2 3
263 3 4
263 4 8
263 5 6
263 6 8
263 7 5
263 8 7
263 9 0
```

Get the following output:

```
667.500000 0.022000
Teste #1: (Numero de vertices: 10, Limite de passageiros: 3)

O Melhor resultado foi 667.500000 e ele foi encontrado 1 vez(es).
O Pior resultado foi 667.500000 e ele foi encontrado 1 vez(es).
A Média dos resultados foi de 667.500000.
O tempo médio da melhor resposta foi de 0.022000.
O tempo médio da pior resposta foi de 0.022000.
O tempo médio gasto foi de 0.022000.

```

It means the found result is 667.5, running for 0.022 (s).
