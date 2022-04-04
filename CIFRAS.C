/*
    CIFRAS [tipo de generador de los numeros]
        Tipo de generador de numeros:
            r1: random tv
            r2: random 1-100
    o
    CIFRAS /?
    o
    CIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]

*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <memory.h>

#ifdef __BORLANDC__
#define FASTCALL _fastcall
#define randomint(n) (random(n))
#else
#define FASTCALL
#define randomize() (srand(time(NULL)))
#define randomint(n) (rand() % n)
#endif

#define MaxNodos 6
#define MaxCaminos 12

enum EOperaciones
{
    Suma = 1,
    Multiplicacion = 2,
    Resta = 3,
    Division = 4,
    None = 0
};

typedef struct
{
    int numero;
#if defined(SMALL) || defined(TINY) || defined(COMPACT) || defined(LARGE)
    unsigned char n;
#else
    unsigned n;
#endif
    int camino[MaxCaminos];
} Numero;

typedef struct
{
#if defined(SMALL) || defined(TINY) || defined(COMPACT) || defined(LARGE)
    unsigned char n;
#else
    unsigned n;
#endif
    Numero numeros[6];
    enum EOperaciones operacion;
    int i, j;
} Nodo;

static Nodo *nodos_tail;
static int nodos_n;
static Nodo nodos[MaxNodos];
static Numero solucion = {0, 0};
static int numeros[6];
static int objetivo;
static int grupos123[][6] =
    {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 1, 2, 3},
        {4, 5, 6, 7, 8, 9},
        {10, 10, 25, 50, 75, 100}};

static void resolver();

#ifndef __EMSCRIPTEN__
static void help();
static void generarEnunciado(const char *tipo);
static void leerEnunciado(char **e);
int main(int argc, char **argv)
#else
int main()
#endif
{
    randomize();
#ifndef __EMSCRIPTEN__
    printf("Cifras (programa \"Cifras y letras\" de la tele)\nHecho por Oscar Hernandez Bañó 2016-2022\n");
    if (argc == 1 || !strcmp(argv[1], "/?"))
    {
        help();

        return EXIT_SUCCESS;
    }
    else if (argc == 2)
        generarEnunciado(argv[1]);
    else if (argc == 8)
        leerEnunciado(&argv[1]);
    else
    {
        printf("número de argumentos incorrectos\n");

        return EXIT_FAILURE;
    }

    resolver();
#endif

    return EXIT_SUCCESS;
}

#ifndef __EMSCRIPTEN__

static void help()
{
    printf("modo de empleo:\nCIFRAS [tipo de generador de los numeros]\n");
    printf("\ttTipo de generador de numeros:\n\t\ttv: random tv\n\t\trandom: random 1-100\n");
    printf("o\nCIFRAS /? : muestra esta ayuda\n");
    printf("o\nCIFRAS [num1] [num2] [num3] [num4] [num5] [num6] [objetivo]\n");
}

#endif

static int ajusteidxgrupo(int idx)
{
    return ((idx % 7) != 0) ? idx : idx + 1;
}

/*
    Aplicamos el algoritmo para mezcla de Fisher-Yates
    n = El numero de grupos que queremos juntar
*/
static void mezclargr2(int *result, int n)
{
    int t = 6 * n, i, r, tmp, idx1, idx2;

    for (i = t; i >= 0; i--)
    {
        r = randomint(t);
        idx1 = ajusteidxgrupo(i);
        idx2 = ajusteidxgrupo(r);
        tmp = result[idx1];
        result[idx1] = result[idx2];
        result[idx2] = tmp;
    }
}

static void mezclargr(int *result)
{
    int i, idx1, idx2, tmp, t = 100 + randomint(200);
    int *rptr = result;
    int *m = (int *)&grupos123;

    for (i = 0; i < 4; i++, rptr += 6, m += 6)
    {
        *rptr++ = 6;
        memcpy(rptr, m, 6 * sizeof(int));
    }
    mezclargr2(result, 3);
    mezclargr2(result + 7 * 3, 1);
    // memcpy(result,grupos123,4*6*sizeof(int));
    for (i = 0; i < t; i++)
    {
        idx1 = randomint(3) * 7 + randomint(6) + 1;
        idx2 = randomint(3) * 7 + randomint(6) + 1;
        tmp = result[idx1];
        result[idx1] = result[idx2];
        result[idx2] = tmp;
        idx1 = 3 * 7 + randomint(6) + 1;
        idx2 = 3 * 7 + randomint(6) + 1;
        tmp = result[idx1];
        result[idx1] = result[idx2];
        result[idx2] = tmp;
    }
}

static int obtnumero(int *result, int grupo)
{
    int *rptr = result + 7 * (grupo - 1);
    int num = rptr[*rptr];

    --*rptr;

    return num;
}

static int randomgrupo(int n123, int num)
{
    int p = randomint(100);
    int p123 = (num) ? 100 * n123 / num : 0;
    int p100123 = (100 - p123) / 2;
    int g = (p <= p100123) ? randomint(3) + 1 : 4;

    return g;
}

static void generar_random_tv()
{
    int result[4][7];
    int i;
    int n123 = 0;
    int grupo;

    mezclargr((int *)&result);
    for (i = 0; i < 6; i++)
    {
        grupo = randomgrupo(n123, i + 1);
        numeros[i] = obtnumero((int *)&result, grupo);
        printf("%d ", numeros[i]);
        if (grupo < 4)
            n123++;
    }
    objetivo = 100 + randomint(900);
    printf("%d\n", objetivo);
}

void generar_random_1_100()
{
    int i;

    for (i = 0; i < 6; i++)
    {
        numeros[i] = randomint(100) + 1;
        printf("%d ", numeros[i]);
    }
    objetivo = 100 + randomint(900);
    printf("%d\n", objetivo);
}

static void generarEnunciado(const char *tipo)
{
    if (!strcmp(tipo, "random"))
        generar_random_1_100();
    else if (!strcmp(tipo, "tv"))
        generar_random_tv();
    else
    {
        printf("%s no es un tipo de generador de número correcto\n", tipo);
        exit(-1);
    }
}

static void leerEnunciado(char **e)
{
    int i;

    for (i = 0; i < 6; i++)
        numeros[i] = atoi(*e++);
    objetivo = atoi(*e);
}

static void reset_nodos()
{
    nodos_tail = nodos;
    nodos_n = 0;
}

static FASTCALL Nodo *new_nodo()
{
    assert(nodos_n <= MaxNodos);
    nodos_n++;

    return nodos_tail++;
}

static FASTCALL Nodo *del_nodo()
{
    assert(nodos_n > 0);

    return --nodos_tail;
}

static FASTCALL int search_numero(Nodo *n, unsigned numero)
{
    int i = 0;
    Numero *ptr = n->numeros;

    for (; i < n->n; i++, ptr++)
        if (ptr->numero >= numero)
            return i;

    return n->n;
}

static Numero *insert_numero(Nodo *n, unsigned numero)
{
    int i = search_numero(n, numero);
    Numero *ptr = &n->numeros[i];
    int diff = n->n - i;

    if (diff > 0)
        memmove(ptr + 1, ptr, diff * sizeof(*ptr));
    ptr->numero = numero;
    ptr->n = 0;
    n->n++;

    return ptr;
}

static FASTCALL void new_firstnodo()
{
    Nodo *n = new_nodo();
    int i = 6;
    int *ptr = numeros;
    Numero *n1;

    n->n = 0;
    for (; i-- > 0; ptr++)
    {
        n1 = insert_numero(n, *ptr);
        *(n1->camino) = *ptr;
        n1->n = 1;
    }

    n->operacion = Suma;
    n->i = 0;
    n->j = 1;
}

static FASTCALL int calc_op(EOperaciones op, unsigned n1, unsigned n2)
{
    switch (op)
    {
    case Suma:
        return n1 + n2;
    case Resta:
        return n1 - n2;
    case Multiplicacion:
        return (n1 > 999 || n2 > 999) ? -1 : n1 * n2;
    case Division:
        return ((n1 % n2) == 0) ? n1 / n2 : -1;
    default:
        return -1;
    }
}

static FASTCALL int calc_nodo_act()
{
    Nodo *n = nodos_tail, *n3;
    Numero *n1 = &n->numeros[n->i], *n2 = &n->numeros[n->j], *n4;
    int r = calc_op(n->operacion, n1->numero, n2->numero), num;

    if (r < 0)
        return -1;

    n3 = new_nodo();
    if (n->i > 0)
    {
        memcpy(n3->numeros, n->numeros, n->i * sizeof(*n1));
        n3->n = n->i;
    }
    num = (n->j - n->i);
    if (num > 0)
    {
        memcpy(n3->numeros + n3->n, n2 + 1, num * sizeof(*n2));
        n3->n += num;
    }
    num = n->n - n->j - 1;
    if (num > 0)
    {
        memcpy(n3->numeros + n3->n, &n->numeros[n->j + 1], num * sizeof(*n2));
        n3->n += num;
    }
    n4 = insert_numero(n3, r);
    n4->n = n1->n;
    memcpy(n4->camino, n1->camino, n1->n * sizeof(int));
    n4->camino[n4->n++] = -(int)n->operacion;
    memcpy(n4->camino + n4->n, n2->camino, n2->n * sizeof(int));
    n4->n += n2->n;

    if (r == objetivo)
    {
        solucion = *n4;
        
        return 0;
    }

    // Numero *n3=

    // if (r==objetivo)

    return 1;
}

static FASTCALL int resolver_nodo_3_6()
{
    Nodo *n = nodos_tail;
    int r = calc_op(n->operacion, n->numeros[n->i].numero, n->numeros[n->j].numero);

    return 0;
}

static FASTCALL int resolver_nodo()
{
    return resolver_nodo_3_6();
}

static void resolver()
{
    time_t t1, t2;
    int idx;

    memset(nodos, 0, sizeof(nodos));
    time(&t1);
    new_firstnodo();

    while (nodos_tail >= 0)
    {
        if (!resolver_nodo())
            return;
    }
    time(&t2);
}