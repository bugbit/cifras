	CIFRAS [tipo de generador de los numeros]
		Tipo de generador de numeros:
			1: random tv2
			2: random canal sur
			3: random 1-100

	CIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]

Random tv2:

los concursantes elegían de entre los 4 grupos de 6 fichas, conteniendo los 3 primeros dos juegos de números del 1 al 9, y en el cuarto grupo los números 10, 10, 25, 50, 75 y 100

Random Canal Sur:

 los generaba un ordenador, probablemente dando el doble de probabilidad a los números del 1 al 9. En este caso pueden salir más de 2 fichas del 1-9 y más de 1 ficha del 10-100

Máxima longitud de nodos:

Nodo	Numeros		Solución
1	1 2 3 4 5 6	1+6=7
2	2 3 4 5 7	2+5=7'
3	3 4 7 7'	3+4=7''
4	7 7' 7''	(1+6)+(2+5)=14
5	7'' 14		(3+4)+((1+6)+(2+5))=21

Función heuristica para saber si hay que aproximar:

comparar si el cuadrado del sumatorio de las pistas es menor que el doble del número a hallar

Multiplicación:

Ej 25 3 8 10 50 10 => 571

50 25 10 10 8 3

sqrt(571)=23

571/50=11

50 * 10, 50 * 10, 50 * 8, 50 * 3
