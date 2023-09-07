#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 2147483647
#define MAX_ISTR 18
#define MAX_HEAP 512
#define MAX_ARRAY 512

typedef struct stazione_
{
    int key;             // distanza
    struct stazione_ *p;               // padre
    struct stazione_ *l;               // left
    struct stazione_ *r;               // right
    int *v;              // macchine
    int size;           // numero macchine
    struct stazione_ *adjPrec;
    char color; // BFS
    int dist; // BFS
} stazione_t;

typedef struct QueueNode {
    stazione_t* stazione;
    struct QueueNode* next;
} queue_node_t;

typedef struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
} queue_t;

typedef struct bst_
{
    stazione_t *root;
    queue_t *Q;
    stazione_t **adj;
    int adjSize;
    int adjMax;
} bst_t;

void getchar_scanf_int(int *number)
{
    char ch = getchar_unlocked();
    if (ch == ' ') {
        ch = getchar_unlocked();
    }
    *number = 0;
    while (ch >= '0' && ch <= '9')
    {
        *number = *number * 10 + (ch - '0');
        ch = getchar_unlocked();
    }
}

queue_node_t* createQueueNode(stazione_t* node) {
    queue_node_t* newNode = malloc(sizeof(queue_node_t));
    newNode->stazione = node;
    newNode->next = NULL;
    return newNode;
}

// Funzione per creare una coda vuota
queue_t* createQueue() {
    queue_t* queue = malloc(sizeof(queue_t));
    queue->front = queue->rear = NULL;
    return queue;
}

// Inserisce un nodo alla fine della coda
void enqueue(queue_t* queue, stazione_t* node) {
    queue_node_t* newNode = createQueueNode(node);
    if (queue->front == NULL) { // coda vuota
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Rimuove e restituisce il nodo in testa alla coda
stazione_t* dequeue(queue_t* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    queue_node_t* temp = queue->front;
    stazione_t* node = queue->front->stazione;
    queue->front = queue->front->next;
    free(temp);
    return node;
}

void deleteQueue(queue_t *queue){
    stazione_t *staz = dequeue(queue);
    while(staz != NULL){
        staz = dequeue(queue);
    }
    queue->front = queue->rear = NULL;
    //free(queue);
}

stazione_t *bst_search(stazione_t *bst, int key);
stazione_t *bst_insert(int key);
stazione_t *bst_min(stazione_t *x);
stazione_t *bst_successor(stazione_t *x);
void bst_delete(stazione_t *x);
void swap(int *a, int *b);
void heap_heapify(int v[], int size, int i);
void heap_insert(int v[], int *size, int num);
void heap_delete(int v[], int *size, int num);
void heap_sort(int v[], int size);

bst_t data;

stazione_t *bst_search(stazione_t *bst, int key)
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

stazione_t *bst_insert(int key)
{
    stazione_t *pre = NULL;
    stazione_t *curr = data.root;
    while (curr != NULL)
    {
        pre = curr;
        if (key < curr->key)
        {
            curr = curr->l;
        }
        else if (key == curr->key) {
            printf("non aggiunta\n");
            return NULL;
        } else
        {
            curr = curr->r;
        }
    }
    stazione_t *stazione = malloc(sizeof(stazione_t));
    stazione->key = key;
    stazione->size = 0;
    stazione->p = pre;
    stazione->l = NULL;
    stazione->r = NULL;
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
    stazione->color = 'w';
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

stazione_t *bst_max(stazione_t *x){
    while (x->r != NULL)
    {
        x = x->r;
    }
    return x;
}

stazione_t *bst_successor(stazione_t *x)
{
    stazione_t *y = NULL;
    if(x == NULL) {
        return NULL;
    }
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

stazione_t *bst_predecessor(stazione_t *x){
    stazione_t *y = NULL;
    if(x == NULL) {
        return NULL;
    }
    if (x->l != NULL)
    {
        return bst_max(x->l);
    }
    y = x->p;
    while (y != NULL && x == y->l)
    {
        x = y;
        y = y->p;
    }
    return y;
}

void bst_delete(stazione_t *x)
{
    stazione_t *da_canc, *sottoa = NULL;
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

void swap(int *a, int *b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}

void heap_heapify(int v[], int size, int i)
{
    int larg = i;
    int left = i * 2 + 1;
    int right = i * 2 + 2;
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

void heap_insert(int v[], int *size, int num)
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
    } /*else if (num < v[0]) {
        v[*size] = num;
        *size += 1;
    } */
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

void heap_delete(int v[], int *size, int num)
{
    int i = 0;
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

void heap_sort(int v[], int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heap_heapify(v, size, i);
    }
}

void color_bst(stazione_t *nodo, int key){
    if(nodo == NULL) {
        return;
    }
    if(nodo->key == key){
        nodo->color = 'g';
        nodo->dist = 0;
    } else {
        nodo->color = 'w';
        nodo->dist = INT_MAX;
    }
    nodo->adjPrec = NULL;
    color_bst(nodo->l, key);
    color_bst(nodo->r, key);
}

void stampaPercorso(stazione_t *tmp, stazione_t *arrivo){
    if(tmp->adjPrec != NULL) {
        stampaPercorso(tmp->adjPrec, arrivo);
    } 
    if(tmp == arrivo) {
        printf("%d\n", (tmp->key));
    } else {
        printf("%d ", (tmp->key));
    }
}

void adjacents(stazione_t* root, int direzione) {
    if(root == NULL){
        data.adjSize = -1;
        return;
    }
    if(root->size == 0) {
        data.adjSize = -1;
        return;
    }
    data.adjSize = 0;
    if(direzione){
        for(stazione_t* staz = bst_successor(root); staz != NULL && root->key + root->v[0] >= staz->key; staz = bst_successor(staz)){
            if(staz->color != 'b' && ( staz->color != 'g' || (staz->dist >= root->dist + 1))) {
                if(data.adjSize < data.adjMax) {
                    data.adj[data.adjSize] = staz;
                    data.adjSize++;
                } else {
                    data.adjMax += MAX_ARRAY;
                    data.adj = realloc(data.adj, data.adjMax*sizeof(stazione_t*));
                }
            }
        }
    } else {
        for(stazione_t* staz = bst_predecessor(root); staz != NULL && root->key - root->v[0] <= staz->key; staz = bst_predecessor(staz)){
            if(staz->color != 'b' && ( staz->color != 'g' || (staz->dist >= root->dist + 1))) {
            if(data.adjSize < data.adjMax) {
                    data.adj[data.adjSize] = staz;
                    data.adjSize++;
                } else {
                    data.adjMax += MAX_ARRAY;
                    data.adj = realloc(data.adj, data.adjMax*sizeof(stazione_t*));
                }
            }
        }
    }
}

void bfs(stazione_t *partenza, stazione_t *arrivo, int direzione){
    color_bst(data.root, partenza->key);
    enqueue(data.Q, partenza);
    int exit = 0;
    while(data.Q->front != NULL && !exit) { 
        stazione_t *u = dequeue(data.Q);
        if(u->key == arrivo->key) {
            exit = 1;
        }
        if(arrivo->dist > u->dist && !exit) {
            adjacents(u, direzione);
            for(int i = 0; data.adjSize != -1 && i < data.adjSize; i++){
                stazione_t *stazione = data.adj[i];
                if(stazione->dist > u->dist) {
                    stazione->dist = u->dist + 1;
                    if(stazione->adjPrec == NULL) {
                        stazione->adjPrec = u;
                    }
                    if(stazione->adjPrec->key > u->key){
                        stazione->adjPrec = u; 
                    }
                    if (stazione->color == 'w') {
                        stazione->color = 'g';
                        enqueue(data.Q, stazione);
                    }
                }
                u->color = 'b';
            }
        }
    }
    deleteQueue(data.Q);
    if(arrivo->adjPrec == NULL) {
        printf("nessun percorso\n");
        return;
    }
    stampaPercorso(arrivo, arrivo);
}

void aggiungiStazione()
{
    int distanza, autonomiaTemp;
    int numeroAuto;
    getchar_scanf_int(&distanza);
    getchar_scanf_int(&numeroAuto);
    if (numeroAuto > MAX_HEAP)
    {
        printf("non aggiunta\n");
        return;
    }
    stazione_t *stazione = bst_insert(distanza);
    if(stazione != NULL) {
    stazione->size = numeroAuto;
    stazione->v = malloc(sizeof(int) * MAX_HEAP);
    for (int i = 0; i < numeroAuto; i++)
    {
        getchar_scanf_int(&autonomiaTemp);
        stazione->v[i] = autonomiaTemp;
    }
    heap_sort(stazione->v, stazione->size);
    }
}

void demolisciStazione()
{
    int distanza;
    getchar_scanf_int(&distanza);
    bst_delete(bst_search(data.root, distanza));
}

void aggiungiAuto()
{
    int distanza, autonomia;
    getchar_scanf_int(&distanza);
    getchar_scanf_int(&autonomia);

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
    int distanza, autonomia;
    // rottama-auto distanza-stazione autonomia-auto-da-rottamare
    getchar_scanf_int(&distanza);
    getchar_scanf_int(&autonomia);

    stazione_t *stazione = bst_search(data.root, distanza);
    if (stazione == NULL)
    {
        printf("non rottamata\n");
        return;
    }
    heap_delete(stazione->v, &(stazione->size), autonomia);
}

void demolisciNodo(stazione_t *x){
    if(x == NULL){
        return;
    }
    demolisciNodo(x->l);
    demolisciNodo(x->r);
    free(x->v);
    free(x);
}

void demolisciAlbero() {
    demolisciNodo(data.root);
}

void pianificaPercorso()
{
    int inizio, fine;
    // pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
    getchar_scanf_int(&inizio);
    getchar_scanf_int(&fine);
    stazione_t *partenza = bst_search(data.root, inizio);
    if(partenza == NULL) {
        printf("nessun percorso\n");
        return;
    }
    stazione_t *arrivo = bst_search(data.root, fine);
    if(arrivo == NULL) {
        printf("nessun percorso\n");
        return;
    }
    bfs(partenza, arrivo, inizio < fine);
}

void parser()
{
    char istr[MAX_ISTR + 1];
    char c;
    int i = 0;

    while (1) {
        c = getc_unlocked(stdin);

        if (c == ' ' || c == '\n' || c == EOF) {
            istr[i] = '\0'; // Termina la stringa
            i = 0;

            if (strcmp(istr, "aggiungi-stazione") == 0) {
                aggiungiStazione();
            } else if (strcmp(istr, "demolisci-stazione") == 0) {
                demolisciStazione();
            } else if (strcmp(istr, "aggiungi-auto") == 0) {
                aggiungiAuto();
            } else if (strcmp(istr, "rottama-auto") == 0) {
                rottamaAuto();
            } else if (strcmp(istr, "pianifica-percorso") == 0) {
                pianificaPercorso();
            }

            if (c == EOF) {
                break;
            }
        } else {
            istr[i++] = c;
        }
    }
}

int main(int argc, char *argv[])
{
    data.root = NULL;
    data.Q = createQueue();
    data.adj = malloc(sizeof(stazione_t*) * MAX_ARRAY);
    data.adjMax = MAX_ARRAY;
    data.adjSize = 0;
    parser();
    return 0;
}