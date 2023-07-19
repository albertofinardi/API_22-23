#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ISTR 18
#define MAX_HEAP 512

typedef struct stazione_
{
    unsigned long int key;             // distanza
    struct stazione_ *p;               // padre
    struct stazione_ *l;               // left
    struct stazione_ *r;               // right
    unsigned long int *v;     // macchine
    unsigned short int size;               // numero macchine
} stazione_t;

typedef struct bst_
{
    stazione_t *root;
} bst_t;

void heap_print(unsigned long int v[], unsigned short size);
stazione_t *bst_search(stazione_t *bst, unsigned long int key);
stazione_t *bst_insert(unsigned long int key);
stazione_t *bst_min(stazione_t *x);
stazione_t *bst_successor(stazione_t *x);
void inorder_bst_print(stazione_t *x);
void bst_delete(stazione_t *x);
void swap(unsigned long int *a, unsigned long int *b);
void heap_heapify(unsigned long int v[], unsigned short size, unsigned short i);
void heap_insert(unsigned long int v[], unsigned short *size, unsigned long int num);
void heap_delete(unsigned long int v[], unsigned short *size, unsigned long int num);
void heap_sort(unsigned long int v[], unsigned short size);
void bst_search_print(unsigned long int key);

bst_t data;

void heap_print(unsigned long int v[], unsigned short size)
{
    printf("\t\tHeap (%hu): ", size);
    for (int i = 0; i < size; i++)
    {
        printf("%lu ", v[i]);
    }
    printf("\n");
}

stazione_t *bst_search(stazione_t *bst, unsigned long int key)
{
    if (bst == NULL)
    {
        return NULL;
    }
    if (key == bst->key)
        return bst;
    if (key < bst->key)
    {
        return bst_search(bst->l, key);
    }
    else
    {
        return bst_search(bst->r, key);
    }
}

stazione_t *bst_insert(unsigned long int key)
{
    stazione_t *stazione;
    stazione_t *curr, *pre;
    stazione = malloc(sizeof(stazione_t));
    if (stazione == NULL)
    {
        printf("non aggiunta\n");
        return NULL;
    }
    stazione->key = key;
    stazione->size = 0;
    pre = NULL;
    curr = data.root;
    while (curr != NULL)
    {
        pre = curr;
        if (key < curr->key)
        {
            curr = curr->l;
        }
        else
        {
            curr = curr->r;
        }
    }
    stazione->p = pre;
    if (pre == NULL)
    {
        data.root = stazione;
    }
    else if (key < pre->key)
    {
        pre->l = stazione;
    }
    else
    {
        pre->r = stazione;
    }
    printf("aggiunta\n");
    return stazione;
}

stazione_t *bst_min(stazione_t *x)
{
    while (x->l != NULL)
    {
        x = x->l;
    }
    return x;
}

stazione_t *bst_successor(stazione_t *x)
{
    stazione_t *y;
    if (x->r != NULL)
    {
        return bst_min(x->r);
    }
    y = x->p;
    while (y != NULL && x == y->r)
    {
        x = y;
        y = y->p;
    }
    return y;
}

void inorder_bst_print(stazione_t *x)
{
    if (x != NULL)
    {
        inorder_bst_print(x->l);
        printf("Stazione: %lu\n", x->key);
        heap_print(x->v, x->size);
        inorder_bst_print(x->r);
    }
}

void bst_delete(stazione_t *x)
{
    stazione_t *da_canc, *sottoa;
    if (x == NULL) // stazione non esiste
    {
        printf("non demolita\n");
        return;
    }
    if (x->l == NULL || x->r == NULL)
    {
        da_canc = x;
    }
    else
    {
        da_canc = bst_successor(x);
    }
    if (da_canc->l != NULL)
    {
        sottoa = da_canc->l;
    }
    else
    {
        sottoa = da_canc->r;
    }
    if (sottoa != NULL)
    {
        sottoa->p = da_canc->p;
    }
    if (da_canc->p == NULL)
    {
        data.root = sottoa;
    }
    else if (da_canc == da_canc->p->l)
    {
        da_canc->p->l = sottoa;
    }
    else
    {
        da_canc->p->r = sottoa;
    }
    if (da_canc != x)
    {
        x->key = da_canc->key;
        free(x->v);
        x->v = da_canc->v;
        x->size = da_canc->size;
    }
    free(da_canc);
    printf("demolita\n");
}

void swap(unsigned long int *a, unsigned long int *b)
{
    unsigned long int temp = *b;
    *b = *a;
    *a = temp;
}

void heap_heapify(unsigned long int v[], unsigned short size, unsigned short i)
{
    unsigned short larg = i;
    unsigned short left = i * 2 + 1;
    unsigned short right = i * 2 + 2;
    if (left < size && v[left] > v[larg])
    {
        larg = left;
    }
    if (right < size && v[right] > v[larg])
    {
        larg = right;
    }
    if (i != larg)
    {
        swap(&v[i], &v[larg]);
        heap_heapify(v, size, larg);
    }
}

void heap_insert(unsigned long int v[], unsigned short *size, unsigned long int num)
{
    if (*size == MAX_HEAP)
    {
        printf("non aggiunta\n");
        return;
    }
    if (size == 0)
    {
        v[0] = num;
        *size = 1;
    }
    else
    {
        v[*size] = num;
        *size += 1;
        for (int i = *size / 2 - 1; i >= 0; i--)
        {
            heap_heapify(v, *size, i);
        }
    }
}

void heap_delete(unsigned long int v[], unsigned short *size, unsigned long int num)
{
    int i;
    for (i = 0; i < *size; i++)
    {
        if (num == v[i])
        {
            break;
        }
    }
    if (i == *size)
    {
        printf("non rottamata\n");
        return;
    }
    swap(&v[i], &v[*size - 1]);
    *size -= 1;
    for (int i = *size / 2 - 1; i >= 0; i--)
    {
        heap_heapify(v, *size, i);
    }
    printf("rottamata\n");
}

void heap_sort(unsigned long int v[], unsigned short size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heap_heapify(v, size, i);
    }
}

void aggiungiStazione()
{
    unsigned long distanza, autonomiaTemp;
    unsigned short numeroAuto;
    // aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
    scanf("%lu ", &distanza);
    // crea stazione
    scanf("%hu ", &numeroAuto);

    if (numeroAuto > MAX_HEAP || bst_search(data.root, distanza) != NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    stazione_t *stazione = bst_insert(distanza);
    stazione->size = numeroAuto;
    stazione->v = malloc(sizeof(unsigned long int) * MAX_HEAP);
    for (int i = 0; i < numeroAuto; i++)
    {
        scanf("%lu ", &autonomiaTemp);
        //heap_insert(stazione->heap, autonomiaTemp);
        stazione->v[i] = autonomiaTemp;
    }
    heap_sort(stazione->v, stazione->size);
}

void demolisciStazione()
{
    unsigned long int distanza;
    // demolisci-stazione distanza
    scanf("%lu ", &distanza);
    bst_delete(bst_search(data.root, distanza));
}

void aggiungiAuto()
{
    unsigned long int distanza, autonomia;
    // aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
    scanf("%lu ", &distanza);
    scanf("%lu ", &autonomia);

    stazione_t *stazione = bst_search(data.root, distanza);
    if (stazione == NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    heap_insert(stazione->v, &(stazione->size), autonomia);
    printf("aggiunta\n");
}

void rottamaAuto()
{
    unsigned long int distanza, autonomia;
    // rottama-auto distanza-stazione autonomia-auto-da-rottamare

    scanf("%lu ", &distanza);
    scanf("%lu ", &autonomia);

    stazione_t *stazione = bst_search(data.root, distanza);
    if (stazione == NULL)
    {
        printf("non rottamata\n");
        return;
    }
    heap_delete(stazione->v, &(stazione->size), autonomia);

    // printf("RIMUOVI AUTO %d %d\n", distanza, autonomia);
    //  rimuovi auto alla lista di auto della stazione (se presente)
}

void pianificaPercorso()
{
    unsigned long int inizio, fine;
    // pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
    scanf("%lu ", &inizio);
    scanf("%lu ", &fine);
    printf("P\n");
    // ...
}

void visualizza()
{
    printf("\n\nDATA\n");
    inorder_bst_print(data.root);
    printf("\n\n");
}

void visualizzaStazione() {
    unsigned long int dist;
    scanf("%lu ", &dist);
    printf("\n\nDATA\n");
    stazione_t *x = bst_search(data.root, dist);
    printf("Stazione: %lu\n", x->key);
    heap_print(x->v, x->size);
}

void parser()
{
    char istr[MAX_ISTR + 1];
    while (scanf("%s ", istr) != EOF)
    {
        if (!strcmp(istr, "aggiungi-stazione"))
        {
            aggiungiStazione();
        }
        else if (!strcmp(istr, "demolisci-stazione"))
        {
            demolisciStazione();
        }
        else if (!strcmp(istr, "aggiungi-auto"))
        {
            aggiungiAuto();
        }
        else if (!strcmp(istr, "rottama-auto"))
        {
            rottamaAuto();
        }
        else if (!strcmp(istr, "pianifica-percorso"))
        {
            pianificaPercorso();
        }
        else if (!strcmp(istr, "visualizza"))
        {
            visualizza();
        }
        else if (!strcmp(istr, "visualizza-staz"))
        {
            visualizzaStazione();
        }
    }
}

int main(int argc, char *argv[])
{
    data.root = NULL;
    parser();
    return 0;
}