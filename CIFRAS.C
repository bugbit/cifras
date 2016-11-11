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

enum EOperaciones
{
	Suma=0,Multiplicacion,Resta,Division,None
};

typedef struct _Expr2Bin_
{
	int num1,num2;
	enum EOperaciones operacion;
	struct _Expr2Bin_ *next;
} Expr2Bin;

typedef struct
{
	int numeros[6];
	int count;
	int sum;
	Expr2Bin solucion;
} Solucion;

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
Expr2Bin *mSolucion;
int mObjetivoSol;
enum EOperaciones 
	mOpApx[]={ Multiplicacion,Suma,None },
	mOpExc[]={ Multiplicacion,Suma,Resta,Division,None };
char mOperacionesChar[]="+*-/";
	
int FASTCALL resolsol(Solucion *sol,int aproxcalc,enum EOperaciones *op,int idxi);

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

int FASTCALL isexpr2binbottom(Expr2Bin *expr)
{
	return !expr->num1 && expr->num1==expr->num2;
}

void FASTCALL setexpr2binbottom(Expr2Bin *expr)
{
	expr->num1=expr->num2=0;
	expr->next=NULL;
}

Expr2Bin * FASTCALL getexprfirst(Expr2Bin *expr)
{
	if (expr==NULL || isexpr2binbottom(expr) || expr->next==NULL)
		return NULL;
		
	return expr;
}

/*
	Devuelve el calculo o -1 si no se puede calcular.
	En las operaciones de resta o division se puede cambiar los operandos si es mayor
	Si es una resta y el resultado es cero devolvemos -1
	Si es una división y el resultado no es exacto devolvemos -1
	Si la operación es multiplicación o división y el segundo operando es 1, también la descarto, puesto que ya hemos probado ese caso.
 */
int FASTCALL calcexpr(Expr2Bin *expr)
{
	int num1=expr->num1;
	int num2=expr->num2;
	div_t d;
	
	if (num1<num2)
	{
		expr->num1=num2;
		expr->num2=num1;
		num1=expr->num1;
		num2=expr->num2;
	}
	switch(expr->operacion)
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
			
			return expr->num1-expr->num2;
		case Division:
			if (num2==1)
				return -1;
					
			d=div(num1,num2);
			
			return (d.rem) ? -1 : d.quot;				
	}
	
	return -1;
}

void FASTCALL freeexpr(Expr2Bin *expr)
{
	Expr2Bin *expr2=getexprfirst(expr),*e;
	
	if (expr2==NULL)
		return;
	
	do
	{
		e=expr2;
		expr2=expr2->next;
		free(e);
	} while (e!=NULL && !isexpr2binbottom(e));
}

Expr2Bin *FASTCALL cloneexpr(Expr2Bin *expr)
{
	Expr2Bin *exprnew,*exprn2,*e;
	
	if (expr==NULL)
		return NULL;
		
	exprnew=NULL;
	do
	{
		e=(Expr2Bin *) malloc(sizeof(Expr2Bin));
		
		if (e==NULL)
		{
			freeexpr(exprnew);
			
			return NULL;
		}
		*e=*expr;
		if (exprnew==NULL)
			exprnew=e;
		else
			exprn2->next=e;
		exprn2=e;
		expr=expr->next;
	} while (expr!=NULL && !isexpr2binbottom(exprn2));
	e->next=exprnew;
	
	return e->next;
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
int FASTCALL esenconsol(Expr2Bin *expr,int c)
{
	if (c==mObjetivo || abs(c-mObjetivoSol)>=abs(c-mObjetivo))
	{
		freeexpr(mSolucion);
		if ((mSolucion=cloneexpr(expr))==NULL)
			return -1;
			
		mObjetivoSol=c;
		
		return (c==mObjetivo) ? 1 : 0;
	}
	
	return 0;
}

void FASTCALL copynumerosasol(Solucion *sol,int count,int *numeros)
{
	int i,j,tmp,sum;
	
	sol->count=count;
	for(i=0,sum=0;i<count;i++)
	{
		sum += numeros[i];
		sol->numeros[i]=numeros[i];
		for (j=i-1;j>=0;j--)
		{
			if (sol->numeros[j]<sol->numeros[j+1])
			{
				tmp=sol->numeros[j];
				sol->numeros[j]=sol->numeros[j+1];
				sol->numeros[j+1]=tmp;
			}
			else
				break;
		}
	}
	sol->sum=sum;
}

void FASTCALL initsolucion(Solucion *sol)
{
	setexpr2binbottom(&sol->solucion);
}

int FASTCALL esaprox(Solucion *sol)
{
	// comparar si el cuadrado del sumatorio de las pistas es menor que el doble del número a hallar
	
	return sol->sum*sol->sum<2*mObjetivo;
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error, -2 si no se puede calcular
 */
int FASTCALL initsol2op(Solucion *sdest,Solucion *ssource,enum EOperaciones op,int idx1,int idx2,int *idx1new,int *idx2new)
{
	int i,c,r;
	
	sdest->sum=ssource->sum-ssource->numeros[idx1]-ssource->numeros[idx2];
	for (i=sdest->count=0;i<ssource->count;i++)
		if (i==idx1)
			*idx1new=sdest->count;
		else if (i==idx2)
			*idx2new=sdest->count;
		else
			sdest->numeros[sdest->count++]=ssource->numeros[i];
	if (idx1==idx2 && *idx1new==*idx2new)
		++*idx2new;
	sdest->solucion.num1=ssource->numeros[idx1];
	sdest->solucion.num2=ssource->numeros[idx2];
	sdest->solucion.operacion=op;
	
	/*
		zero
		1-zero-1
		1-2-zero-1
		1-2-3-zero-1
	 */
	
	if (isexpr2binbottom(&ssource->solucion))
		ssource->solucion.next=&sdest->solucion;
	sdest->solucion.next=&ssource->solucion;
	c=calcexpr(&sdest->solucion);
	
	if (c<0)
		return -2;
		
	r=esenconsol(&sdest->solucion,c);
	if (r)
		return r;
	
	sdest->numeros[sdest->count++]=c;
	sdest->sum += c;
	
	return 0;
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
int FASTCALL resolsol2op(Solucion *_sol,int aproxcalc,enum EOperaciones *op,int idx1,int idx2)
{
	Solucion sol;
	int r;
	int idx1new,idx2new;
	
	if (_sol->count<=1)
		return 0;
		
	if ((r=initsol2op(&sol,_sol,*op,idx1,idx2,&idx1new,&idx2new)))
		return (r==-2) ? 0 : r; // si no se puede calcular la solución devuelve 0 sino r
		
	return resolsol(&sol,aproxcalc,NULL,sol.count-1);
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
int FASTCALL resolsol(Solucion *sol,int aproxcalc,enum EOperaciones *_op,int idxi)
{
	int isaprox=esaprox(sol),i,j,r;
	enum EOperaciones *op=(_op!=NULL) ? _op : (isaprox) ? mOpApx : mOpExc;
	
	if (!aproxcalc && isaprox)
		return 0;
	
	if (idxi!=0)
	{
		for(;*op!=None;op++)
			for (j=0;j<idxi;j++)
				if ((r=resolsol2op(sol,aproxcalc,op,idxi,j)))
					return r;
	}
	else
		for(;*op!=None;op++)
		{
			for (i=0;i<sol->count;i++)
			{
				for (j=i+1;j<sol->count;j++)
					if ((r=resolsol2op(sol,aproxcalc,op,i,j)))
						return r;
			}
		}
	
	return 0;
}

/*
 * Devuelve 1 si encontrada solucion, 0 si no, -1 si hay error
 */
int FASTCALL resolver()
{
	Solucion solucion;
	
	initsolucion(&solucion);
	copynumerosasol(&solucion,6,(int*) &mNumeros);
	mSolucion=NULL;
	mObjetivoSol=0;
	
	return resolsol(&solucion,esaprox(&solucion),NULL,0);
}

void printsols2(Expr2Bin *sol)
{
	if (isexpr2binbottom(sol))
		return;
	printsols2(sol->next);
	printf("%i %c %i = %i\n",sol->num1,mOperacionesChar[sol->operacion],sol->num2,calcexpr(sol));
}

void printsols(Expr2Bin *sol)
{
	Expr2Bin *expr=getexprfirst(sol);
	
	printsols2(expr);
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
	if (mObjetivoSol==mObjetivo)
		printf("He encontrado la solución en %g segs\n",difftime(t2,t1));
	else
		printf("No He encontrado la solución sino una aproximación: %i en %g segs\n",mObjetivoSol,difftime(t2,t1));
	printf("\nLa solución es:\n\n");
	printsols(mSolucion);
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
