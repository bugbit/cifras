/*
	CIFRAS [tipo de generador de los numeros]
		Tipo de generador de numeros:
			1: random tv2
			2: random canal sur
			3: random 1-100

	CIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]

*/


#include <stdlib.h>
#include <stdio.h>

#ifdef __BORLANDC__
	#define	FASTCALL	_fastcall
#else
	#define	FASTCALL
#endif


enum ERandom { Ninguno=-1, TV2=0, CanalSur,R100 };


enum ERandom mRandom;
int mNumeros[6];
int mObjetivo;

int readargs(int argc,char **argv)
{
	int pRandom;

	switch(argc)
	{
		case 2:
			pRandom=atoi(argv[1]);
			if (pRandom<0 || pRandom>3)
			{
				printf(
					"El tipo de generador tiene que ser unos de estos tipos:\n"
					"1: random tv2\n"
					"2: random canal sur\n"
					"3: random 1-100"
					);

				return -1;
			}

			mRandom=pRandom;

			return 0;
		case 7:
			if (
				scanf
					(
						"%d %d %d %d %d %d %d",
						&mNumeros[0],
						&mNumeros[1],
						&mNumeros[2],
						&mNumeros[3],
						&mNumeros[4],
						&mNumeros[5],
						&mObjetivo
					)!=7)
			{
				printf("Tienes que poner 6 numeros mas el objetivo separado por espacios");

				return -1;
			}
			mRandom=Ninguno;

			return 0;
		default:
			printf("Tienes que poner o la tipo de generador de los numeros o 6 numeros mas el objetivo separado por espacios");

			return -1;

	}
}


int main(int argc,char **argv)
{
	if (readargs(argc,argv))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}