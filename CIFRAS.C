/*
	CIFRAS [tipo de generador de los numeros] [grupo1] [grupo2] [grupo3] [grupo4]
		Tipo de generador de numeros:
			1: random tv2
			2: random canal sur
			3: random 1-100

	CIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __BORLANDC__
	#define	FASTCALL	_fastcall
#else
	#define	FASTCALL
#endif


int tv1grupos[4];
int mNumeros[6];
int mObjetivo;
int mgrupos[][6]=
{
	{ 1,2,3,4,5,6 },
	{ 7,8,9,1,2,3 },
	{ 4,5,6,7,8,9 },
	{ 10,10,25,50,75,100 }
};

void mostrarinstrucciones()
{
	printf
	(
		"Instrucciones:\n"
		"Tipo random o tecleo los 6 n£meros m s objetivo\n"
		"r1\trandom tv2. Puede elegir los 4 grupos\n"
		"r2\trandom canal sur\n"
		"r3\tRandom 1-100\n"
		"exit\tsalir\n"

	);
}

void mezclargr(int *result)
{
	int i,j,k,idx1,idx2,tmp;

	memcpy(result,mgrupos,4*6*sizeof(int));
	//memcpy(result,mgrupos,4*6*sizeof(int));
	for (i=0;i<100;i++)
	{
		idx1=random(3*6);
		idx2=random(3*6);
		tmp=result[idx1];
		result[idx1]=result[idx2];
		result[idx2]=tmp;
		idx1=3*6+random(6);
		idx2=3*6+random(6);
		tmp=result[idx1];
		result[idx1]=result[idx2];
		result[idx2]=tmp;
	}
}

int randomgrupo(int n123,int num)
{
	int p=random(100);
	int p123=(num) ? 100*n123/num : 0;
	int p100123=(100-p123)/2;
	int g=(p<=p100123) ? random(3)+1 : 4;

	return g;
}

void generar_random_tv(int num,char **grupos)
{
	int result[4][6];
	int i;
	int grupo;

	mezclargr(result);
	for (i=0;i<6;i++)
	{
		printf("elija un grupo 1-4: ");
		grupo=(i<num) ? atoi(grupos[i]) : 0;
		if (grupo<1 || grupo>4)
			scanf("%d",&grupo);
		else
			printf("%d\n",grupo);
		mNumeros[i]=result[grupo-1][random(6)];
		printf("N£mero: %d\n",mNumeros[i]);
	}
	mObjetivo=100+random(900);
	printf("N£mero objetivo: %d\n",mObjetivo);
}

void generar_random_canalsur()
{
	int result[4][6];
	int i;
	int n123=0;
	int grupo;

	mezclargr(result);
	for (i=0;i<6;i++)
	{
		grupo=randomgrupo(n123,i+1);
		mNumeros[i]=result[grupo-1][random(6)];
		printf("%d ",mNumeros[i]);
		if (grupo<4)
			n123++;
	}
	mObjetivo=100+random(900);
	printf("%d\n",mObjetivo);
}

void generar_random_1_100()
{
	int i;

	for (i=0;i<6;i++)
	{
		mNumeros[i]=random(100)+1;
		printf("%d ",mNumeros[i]);
	}
	mObjetivo=100+random(900);
	printf("%d\n",mObjetivo);
}


int leerargs(int num,char **argv)
{
	char *arg1=*argv;
	int i;

	if (!strcmpi(arg1,"r1"))
	{
		if (num==1)
			generar_random_tv(0,NULL);
		else if (num<=7)
			generar_random_tv(num-1,argv+1);
		else
		{
			printf("si elijes grupos tienen que ser hasta un m ximo de 6\n");
			return -1;
		}

		return 0;
	}
	else if (!strcmpi(arg1,"r2"))
	{
		generar_random_canalsur();

		return 0;
	}
	else if (!strcmpi(arg1,"r3"))
	{
		generar_random_1_100();

		return 0;
	}
	else if (!strcmpi(arg1,"exit"))
	{
		exit(0);
	}
	else if (num==7)
	{
		for (i=0;i<6;i++)
			mNumeros[i]=atoi(++argv);
		mObjetivo=atoi(argv);

		return 0;
	}

	printf("Elije una opci¢n\n");

	return -1;
}

void resolver()
{
}

void splitargs(char *linea,int *num,char **args)
{
	*num=0;
	while (*linea)
	{
		++*num;
		*args++=linea;
		for (;*linea;linea++)
			if (*linea==' ')
			{
				linea++;
				break;
			}
	}
}

void leerYresolver(int num,char **argv)
{
	int i;
	int sehaleido=0;
	char linea[80];
	char *args[7];

	mostrarinstrucciones();
	if (num)
	{
		for(i=0;i<num;i++)
			if((puts(argv[i])))
				sehaleido=!leerargs(num,argv);

	}
	while (!sehaleido)
	{
		gets(linea);
		splitargs(linea,&num,&args);
		sehaleido=!leerargs(num,args);
	}
	resolver();
}


int main(int argc,char **argv)
{
	int num;
	char **args;

   randomize();
	if (argc>1)
	{
		num=argc-1;
		args=argv+1;
	}

	for(;;)
	{
		leerYresolver(num,args);
		num=0;
		args=NULL;
	}

	return EXIT_SUCCESS;
}