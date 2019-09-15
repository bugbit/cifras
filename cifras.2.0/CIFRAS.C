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
#include <time.h>
#include <errno.h>

#ifdef __BORLANDC__
	#define	FASTCALL	_fastcall
	#define randomint(n)(random(n))
	#define getsarray(s)(gets(s))
#else
	#define	FASTCALL
	#define randomint(n)(rand() % n)
	#define randomize()(srand( time(NULL) ))
	#define	getsarray(s)(fgets_wrapper(s,sizeof(s)-1,stdin))
	
	char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp)
{
    if (fgets(buffer, buflen, fp) != 0)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[len-1] = '\0';
        return buffer;
    }
    return 0;
}
#endif

#define	MaxNodos	5

enum EOperaciones
{
	Suma=0,Multiplicacion,Resta,Division,None
};

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
enum EOperaciones 
	mOpApx[]={ Multiplicacion,Suma,None },
	mOpExc[]={ Multiplicacion,Suma,Resta,Division,None },
	*mOp;
 
char mOperacionesChar[]="+*-/";

/*
	N0 		10 				50 		100			250		300		600		999		Combinaciones: 1,2 ... 1,6	2,3	... 2,6		3,4 ... 3,6		4,5 ... 4,6		5,6	*	4 Operaciones
	N1		10+50			100		250			300		600		999
	N3		10+50+100		250		300			600		999
	N4		10+50+100+250	300		600			999
			
			1º +,-,*,/ 2º +,-,*,/	3º
			
	Array Numero+camino
	0		1			2													.....	12	(10 50 + 100 + 250 + 300 + 600	+)
	Numero	Longitud	Numero o Operación o notacion polaca (camino)
	
	Array	Combinacion
	0		1		2
	pos1	pos2	op
*/

int	mNivNumeros[4][6][12];
int mNivComb[4][3];
int mEsAprox;
int mObjetivoSol[12];

void mostrarinstrucciones()
{
	printf
	(
		"Instrucciones:\n"
		"Tipo random o tecleo los 6 nUmeros mÂ s objetivo\n"
		"r1\trandom tv2. Puede elegir los 4 grupos\n"
		"r2\trandom canal sur\n"
		"r3\tRandom 1-100\n"
		"exit\tsalir\n"

	);
}

int ajusteidxgrupo(int idx)
{
	return ((idx % 7)!=0) ? idx: idx+1;
}

/*
	Aplicamos el algoritmo para mezcla de Fisher-Yates
	n = El numero de grupos que queremos juntar
*/
void mezclargr2(int *result,int n)
{
	int t=6*n,i,r,tmp,idx1,idx2;

	for (i=t;i>=0;i--)
	{
		r=randomint(t);
		idx1=ajusteidxgrupo(i);
		idx2=ajusteidxgrupo(r);
		tmp=result[idx1];
		result[idx1]=result[idx2];
		result[idx2]=tmp;
	}
}

void mezclargr(int *result)
{
	int i,idx1,idx2,tmp,t=100+randomint(200);
	int *rptr=result;
	int *m=(int *) &mgrupos;

	for (i=0;i<4;i++,rptr += 6,m += 6)
	{
		*rptr++=6;
		memcpy(rptr,m,6*sizeof(int));
	}
	mezclargr2(result,3);
	mezclargr2(result+7*3,1);
	//memcpy(result,mgrupos,4*6*sizeof(int));
	for (i=0;i<t;i++)
	{
		idx1=randomint(3)*7+randomint(6)+1;
		idx2=randomint(3)*7+randomint(6)+1;
		tmp=result[idx1];
		result[idx1]=result[idx2];
		result[idx2]=tmp;
		idx1=3*7+randomint(6)+1;
		idx2=3*7+randomint(6)+1;
		tmp=result[idx1];
		result[idx1]=result[idx2];
		result[idx2]=tmp;
	}
}

int obtnumero(int *result,int grupo)
{
	int *rptr=result+7*(grupo-1);
	int num=rptr[*rptr];

	--*rptr;

	return num;
}

int randomgrupo(int n123,int num)
{
	int p=randomint(100);
	int p123=(num) ? 100*n123/num : 0;
	int p100123=(100-p123)/2;
	int g=(p<=p100123) ? randomint(3)+1 : 4;

	return g;
}

void generar_random_tv(int num,char **grupos)
{
	int result[4][7];
	int i;
	int grupo;

	mezclargr((int *) &result);
	for (i=0;i<6;i++)
	{
		printf("elija un grupo 1-4: ");
		grupo=(i<num) ? atoi(grupos[i]) : 0;
		if (grupo<1 || grupo>4)
			do
				scanf("%d",&grupo);
			while (grupo<1 || grupo>4);
		else
			printf("%d\n",grupo);
		mNumeros[i]=obtnumero((int *) &result,grupo);

		printf("Numero: %d\n",mNumeros[i]);
	}
	mObjetivo=100+randomint(900);
	printf("Numero objetivo: %d\n",mObjetivo);
}

void generar_random_canalsur()
{
	int result[4][7];
	int i;
	int n123=0;
	int grupo;

	mezclargr((int *) &result);
	for (i=0;i<6;i++)
	{
		grupo=randomgrupo(n123,i+1);
		mNumeros[i]=obtnumero((int *) &result,grupo);
		printf("%d ",mNumeros[i]);
		if (grupo<4)
			n123++;
	}
	mObjetivo=100+randomint(900);
	printf("%d\n",mObjetivo);
}

void generar_random_1_100()
{
	int i;

	for (i=0;i<6;i++)
	{
		mNumeros[i]=randomint(100)+1;
		printf("%d ",mNumeros[i]);
	}
	mObjetivo=100+randomint(900);
	printf("%d\n",mObjetivo);
}


int leerargs(int num,char **argv)
{
	char *arg1=*argv;
	int i;

	if (!strcmp(arg1,"r1"))
	{
		if (num==1)
			generar_random_tv(0,NULL);
		else if (num<=7)
			generar_random_tv(num-1,argv+1);
		else
		{
			printf("si elijes grupos tienen que ser hasta un mÂ ximo de 6\n");
			return -1;
		}

		return 0;
	}
	else if (!strcmp(arg1,"r2"))
	{
		generar_random_canalsur();

		return 0;
	}
	else if (!strcmp(arg1,"r3"))
	{
		generar_random_1_100();

		return 0;
	}
	else if (!strcmp(arg1,"exit"))
	{
		exit(0);
	}
	else if (num==7)
	{
		for (i=0;i<6;i++)
			mNumeros[i]=atoi(*argv++);
		mObjetivo=atoi(*argv);

		return 0;
	}

	printf("Elije una opciÂ¢n\n");

	return -1;
}
 
 void FASTCALL initniveles()
 {
 	int i;
 	int *n=mNumeros;
 	
 	memset(mNivNumeros,0,sizeof(mNivNumeros));
 	memset(mNivComb,0,sizeof(mNivComb));
 	for(i=0;i<6;i++,n++)
 	{
 		mNivNumeros[0][i][0]=*n;
 		mNivNumeros[0][i][1]=1;
 		mNivNumeros[0][i][2]=*n;
	}
	for(i=0;i<4;i++)
	{
		mNivComb[i][0]=i+1;
		mNivComb[i][1]=i+2;
	}
 }

int FASTCALL esaprox()
 {
 	// comparar si el cuadrado del sumatorio de las pistas es menor que el doble del número a hallar
 	
 	int sum;
 	int i;
 	int *n=mNumeros;
 	
 	for(sum=0,i=6;i-->0;)
 		sum += *n++;
	
	return sum*sum<2*mObjetivo;
 }/*
	Devuelve el calculo o -1 si no se puede calcular.
	En las operaciones de resta o division se puede cambiar los operandos si es mayor
	Si es una resta y el resultado es cero devolvemos -1
	Si es una división y el resultado no es exacto devolvemos -1
	Si la operación es multiplicación o división y el segundo operando es 1, también la descarto, puesto que ya hemos probado ese caso.
 */
int FASTCALL calcexpr(int n1,int n2,enum EOperaciones op)
{
	int num1;
	int num2;
	div_t d;
	
	if (num1<num2)
	{
		num1=n2;
		num2=n1;
	}
	else
	{
		num1=n1;
		num2=n2;
	}
	switch(op)
	{
		case Suma:
			return num1+num2;
		case Multiplicacion:
			if (num2==1)
				return -1;
					
			return num1*num2;
		case Resta:
			if (num1==num2)
				return -1;			
			
			return num1-num2;
		case Division:
			if (num2==1)
				return -1;
					
			d=div(num1,num2);
			
			return (d.rem) ? -1 : d.quot;	
	}
	
	return -1;
}

int FASTCALL copy2numeros(int *n,int *n1,int *n2,int c)
{
	// Falta operacion
	*n++=c;
	*n++=n1[1]+n2[1];
	memcpy(n,n1+2,sizeof(int)*n1[1]);
	memcpy(n+n1[1],n2+2,sizeof(int)*n2[1]);
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
int FASTCALL esenconsol(int *n1,int *n2,int c)
{
	int esencon=c==mObjetivo;
	int *no=mObjetivoSol;
	
	if (esencon || abs(c-*no)>=abs(c-*no))
	{
		copy2numeros(mObjetivoSol,n1,n2,c);
	}
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error, -2 si no se puede calcular
 */
int FASTCALL resolverNivIdxs(int n,int idx1,int idx2,int idxop)
{
	int n1=mNivNumeros[n][idx1][0];
 	int n2=mNivNumeros[n][idx2][0];
 	enum EOperaciones op=mOp[idxop];
 	int calc=calcexpr(n1,n2,op);
}
 
 /*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error, -2 si no se puede calcular
 */
int FASTCALL resolverNiv(int n)
 {
 	int n1,n2,calc;
 	enum EOperaciones op;
 	int *comb=mNivComb[n];
 	int idx1=*comb++;
 	int idx2=*comb++;
 	int idxop=*comb++;
 	
 	
 	
 	return 0;
 }
 
/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
 
 int FASTCALL resolver()
{
	int n,ret;
	
	initniveles();
	mEsAprox=esaprox();
	mOp=(mEsAprox) ? mOpApx : mOpExc;
	for(n=0;n<3;n++)
	{
		ret=resolverNiv(n);
		if (ret!=0 && ret!=-2)
			return ret;
	}
	
	return 0;
}

int obtobjetivosol()
{
	return 0;
}

void printsols()
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
	int r;
	time_t t1,t2;
	int objetivosol;

	mostrarinstrucciones();
	if (num)
	{
		for(i=0;i<num;i++)
			if((puts(argv[i])))
				sehaleido=!leerargs(num,argv);

	}
	while (!sehaleido)
	{
		getsarray(linea);
		splitargs(linea,&num,(char **)&args);
		sehaleido=!leerargs(num,args);
	}
	time(&t1);
	r=resolver();
	time(&t2);
	objetivosol=obtobjetivosol();
	if (objetivosol==mObjetivo)
		printf("He encontrado la soluciÃ³n en %g segs\n",difftime(t2,t1));
	else
		printf("No He encontrado la soluciÃ³n sino una aproximaciÃ³n: %i en %g segs\n",objetivosol,difftime(t2,t1));
	printf("\nLa soluciÃ³n es:\n\n");
	printsols();
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
	else
	{
		num=0;
		args=NULL;
	}

	for(;;)
	{
		leerYresolver(num,args);
		num=0;
		args=NULL;
	}

	return EXIT_SUCCESS;
}
