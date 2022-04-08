<h1>Cifras (programa \"Cifras y letras\" de la tele)</h1><br>
<a href='https://bugbit.github.io/cifras/'>Cifras live (versión WASM, compile emscripten)</a>

	CIFRAS [tipo de generador de los numeros]
		Tipo de generador de numeros:
			1: random tv2
			2: random canal sur
			3: random 1-100

	CIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]

3*3

Random tv2:

los concursantes elegían de entre los 4 grupos de 6 fichas, conteniendo los 3 primeros dos juegos de números del 1 al 9, y en el cuarto grupo los números 10, 10, 25, 50, 75 y 100

Random Canal Sur:

 los generaba un ordenador, probablemente dando el doble de probabilidad a los números del 1 al 9. En este caso pueden salir más de 2 fichas del 1-9 y más de 1 ficha del 10-100

Máxima longitud de nodos:

Nodo	Numeros		Solución<br>
1	1 2 3 4 5 6	1 + 6 = 7<br>
2	2 3 4 5 7	2 + 5 = 7'<br>
3	3 4 7 7'	3 + 4 = 7''<br>
4	7 7' 7''	(1 + 6) + (2 + 5) = 14<br>
5	7'' 14		(3 + 4) + ((1 + 6)+(2 + 5)) = 21<br>

Función heuristica para saber si hay que aproximar:

comparar si el cuadrado del sumatorio de las pistas es menor que el doble del número a hallar

Otros metodos:

http://eliatron.blogspot.com.es/2011/02/asombroso-juego-de-numeros.html

25, 50, 75, 100, 3, 6 => 952

Vamos a intentar dar una explicación a esto. Un primer acercamiento al número pudo ser: 
  75 / 25 = 3 , 3 * 3 = 9 , 100 + 6 = 106 , 106 * 9 = 954 .
Ahora, sólo haría falta conseguir un 2 para restar, pero éste sólo se podría obtener haciendo 50/25 y el 25 ya lo habíamos utilizado. 
Por eso, hace falta usar el Factor Común. 
La fórmula original resultaría 954 = (100 + 6) * 3 * 75 : 25. Por lo tanto, para poder introducir el 50 : 25, es fácil ver dónde hay que hacerlo: 
	952 = (100 + 6) * 3 * 75 : 25 - 50 : 25 = [(100 +6 ) * 3 * 75 - 50] : 25 y YA ESTÁ. Al final, no era tan complicado llegar al resultado.

1=> 75 / 25 = 3<br>
2=> #1(3) * 3= 9<br>
3=> 100 + 6 = 106<br>
4=> #2(9) * #3(106) = 954<br>

952 - 954 =-2 => 50 / 25 = 2<br>

1= #5(23800) / 25 = 952<br>
2=> 75 * 3 = 225<br>
3=> 100 + 6 = 106<br>
4=> #2(225) * #3(106) = 23850<br>
5=> #4(23850) - 50 = 23800<br>


http://gaussianos.com/los-reyes-de-la-prueba-de-numeros-de-cifras-y-letras/

Como ya he dicho antes, muchos de los números exactos que se han conseguido en programas de este tipo ha sido muy meritorios (de la versión española recuerdo uno que encontró Carlos, uno de sus concursantes más conocidos, usando las curiosas propiedades del número 37), pero encontrar estos es realmente magnífico. Y todo ello en un minuto, que es lo que se deja a los concursantes para pensar.

http://gaussianos.com/multiplos-de-37/
