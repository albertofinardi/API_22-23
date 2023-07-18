#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ISTR 18
#define MAX_HEAP 512

typedef struct heap_
{
    unsigned long int v[MAX_HEAP];
    unsigned short size;
} heap_t;

typedef struct stazione_
{
    unsigned long int key;             // distanza
    heap_t *heap;        // macchine
    struct stazione_ *p; // padre
    struct stazione_ *l; // left
    struct stazione_ *r; // right
} stazione_t;

typedef struct bst_
{
    stazione_t *root;
} bst_t;

void heap_print(heap_t *heap);
stazione_t *bst_search(stazione_t *bst, unsigned long int key);
stazione_t *bst_insert(unsigned long int key);
stazione_t *bst_min(stazione_t *x);
stazione_t *bst_successor(stazione_t *x);
void inorder_bst_print(stazione_t *x);
void bst_delete(stazione_t *x);
void swap(unsigned long int *a, unsigned long int *b);
void heap_free(heap_t *heap);
void heap_heapify(heap_t *heap, unsigned short i);
void heap_insert(heap_t *heap, unsigned long int num);
void heap_delete(heap_t *heap, unsigned long int num);
void heap_sort(heap_t *heap);
void bst_search_print(unsigned long int key);

bst_t *data;

void heap_print(heap_t *heap)
{
    printf("\t\tHeap: ");
    for (int i = 0; i < heap->size; i++)
    {
        printf("%lu ", heap->v[i]);
    }
    printf("\n");
}

stazione_t *bst_search(stazione_t *bst, unsigned long int key)
{
    if (bst == NULL)
    {
        // printf("non trovata\n");
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
    heap_t *heapT = malloc(sizeof(heap_t));
    if (stazione == NULL || heapT == NULL)
    {
        printf("non aggiunta\n");
        return NULL;
    }
    stazione->key = key;
    stazione->heap = heapT;
    stazione->heap->size = 0;
    pre = NULL;
    curr = data->root;
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
        data->root = stazione;
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
        heap_print(x->heap);
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
        data->root = sottoa;
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
        x->heap = da_canc->heap;
    }
    heap_free(da_canc->heap);
    free(da_canc);
    printf("demolita\n");
}

void swap(unsigned long int *a, unsigned long int *b)
{
    unsigned long int temp = *b;
    *b = *a;
    *a = temp;
}

void heap_free(heap_t *heap)
{
    //free(heap->v);
    free(heap);
}

void heap_heapify(heap_t *heap, unsigned short i)
{
    unsigned short larg = i;
    unsigned short left = i * 2 + 1;
    unsigned short right = i * 2 + 2;
    if (left < heap->size && heap->v[left] > heap->v[larg])
    {
        larg = left;
    }
    if (right < heap->size && heap->v[right] > heap->v[larg])
    {
        larg = right;
    }
    if (i != larg)
    {
        swap(&(heap->v[i]), &(heap->v[larg]));
        heap_heapify(heap, larg);
    }
}

void heap_insert(heap_t *heap, unsigned long int num)
{
    if (heap->size == MAX_HEAP || heap == NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    if (heap->size == 0)
    {
        heap->v[0] = num;
        heap->size = 1;
    }
    else
    {
        heap->v[heap->size] = num;
        heap->size += 1;
        for (int i = heap->size / 2 - 1; i >= 0; i--)
        {
            heap_heapify(heap, i);
        }
    }
}

void heap_delete(heap_t *heap, unsigned long int num)
{
    int i;
    for (i = 0; i < heap->size; i++)
    {
        if (num == heap->v[i])
        {
            break;
        }
    }
    if (i == heap->size)
    {
        printf("non rottamata\n");
        return;
    }
    swap(&heap->v[i], &heap->v[heap->size - 1]);
    heap->size -= 1;
    for (int i = heap->size / 2 - 1; i >= 0; i--)
    {
        heap_heapify(heap, i);
    }
    printf("rottamata\n");
}

void heap_sort(heap_t *heap)
{
    for (int i = heap->size / 2 - 1; i >= 0; i--)
    {
        heap_heapify(heap, i);
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

    if (numeroAuto > MAX_HEAP || bst_search(data->root, distanza) != NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    stazione_t *stazione = bst_insert(distanza);
    for (int i = 0; i < numeroAuto; i++)
    {
        scanf("%lu ", &autonomiaTemp);
        //heap_insert(stazione->heap, autonomiaTemp);
        stazione->heap->v[i] = autonomiaTemp;
    }
    stazione->heap->size = numeroAuto;
    heap_sort(stazione->heap);
}

void demolisciStazione()
{
    unsigned long int distanza;
    // demolisci-stazione distanza
    scanf("%lu ", &distanza);
    bst_delete(bst_search(data->root, distanza));
}

void aggiungiAuto()
{
    unsigned long int distanza, autonomia;
    // aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
    scanf("%lu ", &distanza);
    scanf("%lu ", &autonomia);

    stazione_t *stazione = bst_search(data->root, distanza);
    if (stazione == NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    heap_insert(stazione->heap, autonomia);
    printf("aggiunta\n");
}

void rottamaAuto()
{
    unsigned long int distanza, autonomia;
    // rottama-auto distanza-stazione autonomia-auto-da-rottamare

    scanf("%lu ", &distanza);
    scanf("%lu ", &autonomia);

    stazione_t *stazione = bst_search(data->root, distanza);
    if (stazione == NULL)
    {
        printf("non rottamata\n");
        return;
    }
    heap_delete(stazione->heap, autonomia);

    // printf("RIMUOVI AUTO %d %d\n", distanza, autonomia);
    //  rimuovi auto alla lista di auto della stazione (se presente)
}

void pianificaPercorso()
{
    unsigned long int inizio, fine;
    // pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
    scanf("%lu ", &inizio);
    scanf("%lu ", &fine);
    // ...
}

void visualizza()
{
    printf("\n\nDATA\n");
    inorder_bst_print(data->root);
    printf("\n\n");
}

void visualizzaStazione() {
    unsigned long int dist;
    scanf("%lu ", &dist);
    printf("\n\nDATA\n");
    stazione_t *x = bst_search(data->root, dist);
    printf("Stazione: %lu\n", x->key);
    heap_print(x->heap);
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
    data = malloc(sizeof(bst_t));
    if (data == NULL)
    {
        printf("errore memoria\n");
        return 0;
    }
    parser();
    return 0;
}