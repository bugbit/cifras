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
	#define randomint(n)(random() % n)
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

enum EOperaciones
{
	Suma=0,multiplicacion,Resta,division
};

typedef struct
{
	union
	{
		struct
		{
			unsigned num1:10;
			unsigned num2:10;
			unsigned operacion:2;
		} s;
		long n;	
	} u;
} Expr2Bin;

typedef struct
{
	unsigned char count;
	void *expr2bin;
} Expr2Bins;

typedef struct __Combinaciones__
{
	unsigned char numeros[6];
	union
	{
		struct
		{
			unsigned count:3;
			unsigned idx1:3;
			unsigned idx2:3;
			unsigned operacion:2;
		} s;
		short n;
	} u;
	struct __Combinaciones__ *next;
	Expr2Bins exprs;
} Combinaciones;

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
int mCalcAprox;

Combinaciones *mCombTop=NULL;

void mostrarinstrucciones()
{
	printf
	(
		"Instrucciones:\n"
		"Tipo random o tecleo los 6 nUmeros m s objetivo\n"
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
			printf("si elijes grupos tienen que ser hasta un m ximo de 6\n");
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
			mNumeros[i]=atoi(*(++argv));
		mObjetivo=atoi(*argv);

		return 0;
	}

	printf("Elije una opci¢n\n");

	return -1;
}

void esaprox()
{
	// comparar si el cuadrado del sumatorio de las pistas es menor que el doble del número a hallar
	
	int s=0;
	int i=0;
	
	for (;i<6;i++)
		s += mNumeros[i];
		
	mCalcAprox= s*s<2*mObjetivo;
}

void *mallocexprs(size_t size,int num)
{
	return malloc(size+num*sizeof(Expr2Bin));
}

void copythenumeros(Combinaciones *comb)
{
	int i,j,tmp;
	
	for(i=0;i<6;i++)
	{
		comb->numeros[i]=mNumeros[i];
		for (j=i-1;j>=0;i--)
		{
			if (comb->numeros[j]>comb->numeros[j+1])
			{
				tmp=comb->numeros[j];
				comb->numeros[j]=comb->numeros[j+1];
				comb->numeros[j+1]=tmp;
			}
			else
				break;
		}
	}
}

Combinaciones *firstcomb()
{
	Combinaciones *comb=(Combinaciones *) mallocexprs(sizeof(Combinaciones),1);
	
	if (comb!=NULL)
	{
		copythenumeros(comb);
		comb->u.s.count=6;
		comb->u.s.idx1=0;
		comb->u.s.idx2=1;
		comb->u.s.operacion=0;
		comb->exprs.count=0;
	}
	
	return comb;
}

void pushcomb(Combinaciones *comb)
{
	if (mCombTop!=NULL)
		comb->next=mCombTop;
	mCombTop=comb;
}

Combinaciones *popcomb()
{
	Combinaciones *comb=mCombTop;
	
	if (comb!=NULL)
		mCombTop=mCombTop->next;
	
	return comb;
}

void resetcombs()
{
	Combinaciones *comb;
	
	while ((comb=popcomb())!=NULL)
		free(comb);
}

//int op(Expr2Bin *expr,int *)

void FASTCALL resolcomb(Combinaciones *comb)
{
	Expr2Bin *expr=((Expr2Bin *)&comb->exprs.expr2bin)+comb->exprs.count;
	
	comb->exprs.count++;
	expr->u.s.num1=comb->numeros[comb->u.s.idx1];
	expr->u.s.num2=comb->numeros[comb->u.s.idx2];
}

void resolver()
{
	Combinaciones *comb,*comb1;
	
	if ((comb1=comb=firstcomb())==NULL)
	{
		puts(strerror(errno));
		
		return;
	}
	esaprox();
	//pushcomb(comb1);
	do
		resolcomb(comb);
	while ((comb=popcomb())!=NULL);
	resetcombs();
	free(comb1);
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
		getsarray(linea);
		splitargs(linea,&num,(char **)&args);
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
