#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 2147483647
#define MAX_ISTR 18
#define MAX_HEAP 512
#define MAX_ARRAY 128

typedef struct stazione_
{
    int key;             // distanza
    struct stazione_ *p;               // padre
    struct stazione_ *l;               // left
    struct stazione_ *r;               // right
    int *v;              // macchine
    int size;           // numero macchine
    // struct QueueNode *adj;
    struct stazione_ *adjPrec;
    char bfsColor; // BFS
    int bfsDist; // BFS
} stazione_t;

typedef struct QueueNode {
    stazione_t* stazione;
    struct QueueNode* next;
    struct QueueNode* nextAdj;
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

void listPrint(queue_node_t *node){
    queue_node_t *tmp = node;
    for(; tmp != NULL; tmp = tmp->next){
        printf("\t%d\n", tmp->stazione->key);
    }
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
    stazione->bfsColor = 'w';
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
    // TODO: Modifica, se sono black passa direttamente al successivo oltre
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
    // TODO: Modifica, se sono black passa direttamente al precedente oltre
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
        nodo->bfsColor = 'g';
        nodo->bfsDist = 0;
    } else {
        nodo->bfsColor = 'w';
        nodo->bfsDist = INT_MAX;
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

// TODO: Prova ad usare array al posto di lista
// Fai arrai dinamico di X elementi, tieni traccia di size e maxsize (all'inizio X)
// usi size come valore della dimensione dell'array, al posto di free metti size = 0
// se size > maxSize fai una realloc (singola o di un multiplo di X)
// X di 128?
// sui 400 nel caso 100 a fine
//prova a vedere quanti hanno color 'b' negli adiacenti (non so magari puoi toglierli)
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
        // andata CONTROLLA
        //queue_node_t *list = NULL;
        for(stazione_t* staz = bst_successor(root); staz != NULL && root->key + root->v[0] >= staz->key; staz = bst_successor(staz)){
            if(staz->bfsColor != 'b') { // se e' black e' gia' stata visitata ed esiste percorso piu' breve
                /*queue_node_t* new = malloc(sizeof(queue_node_t));
                new->stazione = staz;
                new->nextAdj = list;
                list = new;*/
                if(data.adjSize < data.adjMax) {
                    data.adj[data.adjSize] = staz;
                    data.adjSize++;
                } else {
                    data.adjMax += MAX_ARRAY;
                    data.adj = realloc(data.adj, data.adjMax*sizeof(stazione_t*));
                }
            }
        }
        //return list;
        // while di tutti i successivi, fino a quando superi distanza raggiungibile con autonomia
        // crei la lista e la ritorni, aggiungi in testa
    } else {
        // ritorno CONTROLLA
        //queue_node_t *list = NULL;
        for(stazione_t* staz = bst_predecessor(root); staz != NULL && root->key - root->v[0] <= staz->key; staz = bst_predecessor(staz)){
            if(staz->bfsColor != 'b') {
            /*queue_node_t* new = malloc(sizeof(queue_node_t));
            new->stazione = staz;
            new->nextAdj = list;
            list = new;*/
            if(data.adjSize < data.adjMax) {
                    data.adj[data.adjSize] = staz;
                    data.adjSize++;
                } else {
                    data.adjMax += MAX_ARRAY;
                    data.adj = realloc(data.adj, data.adjMax*sizeof(stazione_t*));
                }
            }
        }

        //return list;
        // while di tutti i precedenti, fino a quando superi distanza raggiungibile con autonomia
        // crei la lista e la ritorni, aggiungi in testa
    }
}

void bfs(stazione_t *partenza, stazione_t *arrivo, int direzione){
    color_bst(data.root, partenza->key);
    //queue_t* Q = createQueue();
    enqueue(data.Q, partenza);
    int exit = 0;
    while(data.Q->front != NULL && !exit) { 
        stazione_t *u = dequeue(data.Q);
        // per ogni v che e' u.adj
        if(u->key == arrivo->key) {
            exit = 1;
        }
        if(arrivo->bfsDist > u->bfsDist && !exit) { // se arrivo ha gia' dist minore, inutile calcolare gli adiacenti (controlla <= se va bene)
            adjacents(u, direzione);
            for(int i = 0; data.adjSize != -1 && i < data.adjSize; i++){
                stazione_t *stazione = data.adj[i];
                if(stazione->bfsDist > u->bfsDist) {
                    stazione->bfsDist = u->bfsDist + 1;
                    if(stazione->adjPrec == NULL) {
                        stazione->adjPrec = u;
                    }
                    if(stazione->adjPrec->key > u->key){
                        stazione->adjPrec = u; 
                    } // in teoria, se partendo da partenza nel ritorno stampo adjPrec, ho percorso

                    if (stazione->bfsColor == 'w') {
                        stazione->bfsColor = 'g';
                        enqueue(data.Q, stazione);
                    }
                }
                u->bfsColor = 'b';
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
    // aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
    if(scanf("%d ", &distanza)) {};
    // crea stazione
    if(scanf("%d ", &numeroAuto)) {};

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
        if(scanf("%d ", &autonomiaTemp)) {};
        //heap_insert(stazione->heap, autonomiaTemp);
        stazione->v[i] = autonomiaTemp;
    }
    heap_sort(stazione->v, stazione->size);
    }
}

void demolisciStazione()
{
    int distanza;
    // demolisci-stazione distanza
    if(scanf("%d ", &distanza)) {};
    bst_delete(bst_search(data.root, distanza));
}

void aggiungiAuto()
{
    int distanza, autonomia;
    // aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
    if(scanf("%d ", &distanza)) {};
    if(scanf("%d ", &autonomia)) {};

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

    if(scanf("%d ", &distanza)) {};
    if(scanf("%d ", &autonomia)) {};

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
    if(scanf("%d ", &inizio)) {};
    if(scanf("%d ", &fine)) {};
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
    //deleteQueue(data.Q);
    demolisciAlbero();
    free(data.adj);
    free(data.Q);
    return 0;
}