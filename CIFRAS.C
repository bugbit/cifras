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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __BORLANDC__
#define FASTCALL _fastcall
#define randomint(n) (random(n))
#else
#define FASTCALL
#define randomize() (srand(time(NULL)))
#define randomint(n) (rand() % n)
#endif

#define MaxNodos 6
#define MaxCaminos 6 * 5

#define OpToCamino(op) (0xFFFF - ((unsigned)op - 1))
#define CaminoToOp(c) ((enum EOperaciones)(0xFFFF - c + 1))

enum EOperaciones
{
    Suma = 1,
    Multiplicacion = 2,
    Resta = 3,
    Division = 4,
    OpMax = Division,
    None = 0
};

typedef unsigned TNumero;

typedef struct
{
    TNumero numero;
#if defined(__SMALL__) || defined(__TINY__) || defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
    unsigned char n;
#else
    unsigned n;
#endif
    TNumero camino[MaxCaminos];
} Numero;

typedef struct
{
#if defined(__SMALL__) || defined(__TINY__) || defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
    unsigned char n;
#else
    unsigned n;
#endif
    Numero numeros[6];
    const enum EOperaciones *operacionPtr, *opPtr;
    int i, j;
} Nodo;

static Nodo *nodos_tail;
static int nodos_n;
static Nodo nodos[MaxNodos];
static Numero solucion = {0, 0};
static TNumero numeros[6];
static TNumero objetivo;
static TNumero stacknums[MaxCaminos];
static int const grupos123[][6] =
    {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 1, 2, 3},
        {4, 5, 6, 7, 8, 9},
        {10, 10, 25, 50, 75, 100}};
static const enum EOperaciones operaciones1[] = {
    Multiplicacion, Suma, Resta, Division};
static const enum EOperaciones operaciones2[] = {
    Suma, Resta, Division, Multiplicacion};

static void resolver();
static void printsols();
static void generarEnunciado(const char *tipo);

#ifndef __EMSCRIPTEN__
static void help();
static void leerEnunciado(char **e);
int main(int argc, char **argv)
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
#else
static void leerJSEnunciado()
{
    int i = 0;

    for (; i < 6; i++)
        numeros[i] = EM_ASM_INT({ return parseInt(document.getElementById($0).value); }, i);
    objetivo = EM_ASM_INT(return parseInt(document.getElementById("num").value));
}

extern "C" void resuelve()
{
    leerJSEnunciado();
    EM_ASM({ document.getElementById('output').value =''; });
    resolver();
}
extern "C" void generaEnunciado(const char *tipo)
{
    int i = 0;

    randomize();
    generarEnunciado(tipo);
    for (; i < 6; i++)
        EM_ASM({document.getElementById($0).value = $1}, i, numeros[i]);
    EM_ASM({document.getElementById("num").value = $0}, objetivo);
}

#endif

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
#ifndef __EMSCRIPTEN__
        printf("%d ", numeros[i]);
#endif
        if (grupo < 4)
            n123++;
    }
    objetivo = 100 + randomint(900);
#ifndef __EMSCRIPTEN__
    printf("%d\n", objetivo);
#endif
}

void generar_random_1_100()
{
    int i;

    for (i = 0; i < 6; i++)
    {
        numeros[i] = randomint(100) + 1;
#ifndef __EMSCRIPTEN__
        printf("%d ", numeros[i]);
#endif
    }
    objetivo = 100 + randomint(900);
#ifndef __EMSCRIPTEN__
    printf("%d\n", objetivo);
#endif
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

#ifndef __EMSCRIPTEN__

static void leerEnunciado(char **e)
{
    int i;

    for (i = 0; i < 6; i++)
        numeros[i] = atoi(*e++);
    objetivo = atoi(*e);
}

#endif

static void reset_nodos()
{
    memset(nodos, 0, sizeof(nodos));
    nodos_tail = NULL;
    nodos_n = 0;
}

static Nodo *new_nodo()
{
#ifndef NDEBUG
    assert(nodos_n <= MaxNodos - 1);
#endif
    nodos_n++;

    return (nodos_tail) ? ++nodos_tail : (nodos_tail = nodos);
}

static Nodo *del_nodo()
{
#ifndef NDEBUG
    assert(nodos_n > 0);
#endif
    nodos_n--;
    memset(nodos_tail, 0, sizeof(*nodos_tail));

    return --nodos_tail;
}

static FASTCALL int search_numero(Nodo *n, unsigned numero)
{
    int i = 0;
    Numero *ptr = n->numeros;

    for (; i < n->n; i++, ptr++)
        if (ptr->numero <= numero)
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
    ptr->n = 1;
    *(ptr->camino) = numero;
    n->n++;

    return ptr;
}

static FASTCALL void initcounters_nodo(Nodo *n)
{
    n->i = 0;
    n->j = 1;
    n->operacionPtr =
        n->opPtr =
            (n->n > 2 && n->numeros[0].numero * n->numeros[1].numero < objetivo)
                ? operaciones1
                : operaciones2;
}

static FASTCALL void new_firstnodo()
{
    Nodo *n = new_nodo();
    int i = 6;
    TNumero *ptr = numeros;
    Numero *n1;

    n->n = 0;
    for (; i-- > 0; ptr++)
    {
        n1 = insert_numero(n, *ptr);
        *(n1->camino) = *ptr;
        n1->n = 1;
    }

    initcounters_nodo(n);
}

static FASTCALL TNumero calc_op(enum EOperaciones op, unsigned n1, unsigned n2)
{
    TNumero r;

    switch (op)
    {
    case Suma:
        return n1 + n2;
    case Resta:
        return (n1 > n2) ? n1 - n2 : 0xFFFF;
    case Multiplicacion:
        r = n1 * n2;

        return r >= n1 || r > n2 ? r : 0xFFFF;
    case Division:
        return ((n1 % n2) == 0) ? n1 / n2 : 0xFFFF;
    default:
        return 0xFFFF;
    }
}

static FASTCALL TNumero diff_nums(TNumero n1, TNumero n2)
{
    return (n1 < n2) ? n2 - n1 : n1 - n2;
}

static FASTCALL int esaprox(TNumero n)
{
    TNumero r1 = diff_nums(n, objetivo), r2 = diff_nums(solucion.numero, objetivo);

    return (r1 < r2);
}

static FASTCALL int add_nums_camino(enum EOperaciones op, Numero *n1, Numero *n2, Numero *nr, int addonlysol)
{
    TNumero *c;
    int l1, l2, n;
    int encon = nr->numero == objetivo;
    int aprox = !encon && esaprox(nr->numero);

    if (addonlysol && !encon && !aprox)
        return 1;

    c = nr->camino;
    l1 = n1->n;
    l2 = n2->n;
    n = l1 + l2 + 1;

#ifndef NDEBUG
    assert(n <= MaxCaminos && l1 > 0 && l2 > 0);
#endif
    nr->n = n;
    memcpy(c, n1->camino, l1 * sizeof(TNumero));
    c += l1;
    memcpy(c, n2->camino, l2 * sizeof(TNumero));
    c += l2;
    *c = OpToCamino(op);

    if (encon)
    {
        solucion = *nr;

        return 0;
    }

    if (aprox)
        solucion = *nr;

    return 1;
}

static FASTCALL void addcounter_nodo(Nodo *n)
{
    if (++(n->j) >= n->n)
    {
        if (++(n->i) > n->n - 2)
        {
            n->i = 0;
            n->j = 1;
            if (++(n->opPtr) >= &(n->operacionPtr[4]))
                n->opPtr = NULL;
        }
        else
        {
            n->j = n->i + 1;
            assert(n->j < n->n);
        }
    }
}

static FASTCALL int calc_nodo_act_2n()
{
    Nodo *n = nodos_tail, *n3 = new_nodo();
    Numero *n1 = n->numeros, *n2 = n->numeros + 1, *n4 = n3->numeros;
    const enum EOperaciones *op = n->opPtr, *opFin = n->operacionPtr + 4;
    TNumero r;

    n3->n = 1;
    for (; op < opFin; op++)
    {
        r = calc_op(*op, n1->numero, n2->numero);

        if (r == 0xFFFF)
            continue;
        n4->numero = r;
        if (!add_nums_camino(*op, n1, n2, n4, 1))
            return 0;
    }

    del_nodo();
    del_nodo();

    return 1;
}

static FASTCALL int calc_nodo_act_mas_2n()
{
    Nodo *n = nodos_tail, *n3;
    Numero *n1, *n2, *n4, *n5;
    int num, i, j;
    TNumero r;
    enum EOperaciones op;

    i = n->i;
    j = n->j;
    op = *(n->opPtr);
    n1 = &n->numeros[i];
    n2 = &n->numeros[j];

    addcounter_nodo(n);

    r = calc_op(op, n1->numero, n2->numero);

    if (r == 0xFFFF)
        return -1;

    n3 = new_nodo();
    n5 = n3->numeros;
    n3->n = 0;
    num = i;
    if (num > 0)
    {
        memcpy(n5, n->numeros, num * sizeof(*n1));
        n5 += num;
        n3->n += num;
    }
    num = (j - i - 1);
    if (num > 0)
    {
        memcpy(n5, n1 + 1, num * sizeof(*n2));
        n5 += num;
        n3->n += num;
    }
    num = n->n - j - 1;
    if (num > 0)
    {
        memcpy(n5, n2 + 1, num * sizeof(*n2));
        n5 += num;
        n3->n += num;
    }
    n4 = insert_numero(n3, r);
    initcounters_nodo(n3);

    if (!add_nums_camino(op, n1, n2, n4, 0))
        return 0;

    return 1;
}

static FASTCALL int resolver_nodo()
{
    if (!nodos_tail->opPtr)
    {
        del_nodo();

        return -1;
    }

    switch (nodos_tail->n)
    {
    case 1:
        return -1;
    case 2:
        return calc_nodo_act_2n();
    default:
        return calc_nodo_act_mas_2n();
    }
}

static void resolver()
{
    time_t t1, t2;
    int idx;

    memset(nodos, 0, sizeof(nodos));
    time(&t1);
    reset_nodos();
    new_firstnodo();

    while (nodos_n > 0)
    {
        if (!resolver_nodo())
            break;
    }
    time(&t2);
    if (solucion.numero == objetivo)
        printf("He encontrado la solución en %g segs\n", difftime(t2, t1));
    else
        printf("No He encontrado la solución sino una aproximación: %i en %g segs\n", solucion.numero, difftime(t2, t1));
    printf("\nLa solución es:\n\n");
    printsols();
}

static void printsols()
{
    TNumero *stack = stacknums - 1;
    TNumero *c = solucion.camino, n1, n2, nr;
    int n = solucion.n;
    char op;
    TNumero opMax = OpToCamino(OpMax);

    for (; n-- > 0; c++)
    {
        if (*c < opMax)
            *++stack = *c;
        else
        {
            n2 = *stack--;
            n1 = *stack--;
            switch (CaminoToOp(*c))
            {
            case Suma:
                nr = n1 + n2;
                op = '+';
                break;
            case Resta:

                nr = n1 - n2;
                op = '-';
                break;
            case Multiplicacion:

                nr = n1 * n2;
                op = '*';
                break;
            case Division:

                nr = n1 / n2;
                op = '/';
                break;
            default:
                break;
            }
            *++stack = nr;
            printf("%d %c %d = %d\n", n1, op, n2, nr);
        }
    }
}