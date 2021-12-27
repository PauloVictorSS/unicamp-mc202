#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INTMAX 999999999.0
#define PAI(i) ((i - 1) / 2)
#define F_ESQ(i) (2 * i + 1) /*Filho esquerdo de i*/
#define F_DIR(i) (2 * i + 2) /*Filho direito de i*/

typedef struct relacao *pRelacao;
typedef struct cidade *pCidade;
typedef struct grafo *pGrafo;

struct relacao{
    int v;
    double distancia;
    pRelacao prox;
};

struct cidade{
    int v;
    char nome[52];
    int populacao;
    double fatorCentralidade;
    pRelacao prox;
};

struct grafo{
    int n;
    pCidade *adjacencia;
};

typedef struct{
    int prioridade;
    int vertice;
} Item;

typedef struct{
    Item *v;
    int n, tamanho;
} FP;

typedef FP *pFP;

/*
    Função que cria a fila de prioridades a ser utilizada
*/
pFP criarFilaPrio(int tam){

    pFP h = malloc(sizeof(FP));
    h->v = malloc(tam * sizeof(Item));
    h->tamanho = tam;
    h->n = 0;

    return h;
}

/*
    Função que troca dois elementos de posicao no heap
*/
void troca(Item *a, Item *b){
    Item t = *a;
    *a = *b;
    *b = t;
}

/*
    Função que retorna a prioridade de determinado elemento dentro do heap
*/
int prioridade(pFP h, int posicao){

    return h->v[posicao].prioridade;
}

/*
    Função que insere um novo elemento no heap
*/
void insereNaFila(pFP h, int v, int prioridade){

    Item novoItem;
    novoItem.vertice = v;
    novoItem.prioridade = prioridade;

    h->v[h->n] = novoItem;
    h->n++;
}

/*
    Função que faz determinado elemento descer no heap
*/
void desceNoHeap(pFP h, int k){
    int menorFilho;

    if (F_ESQ(k) < h->n){

        menorFilho = F_ESQ(k);

        if ((F_DIR(k) < h->n) &&
            (h->v[F_ESQ(k)].prioridade > h->v[F_DIR(k)].prioridade)){

            menorFilho = F_DIR(k);
        }
        if (h->v[k].prioridade > h->v[menorFilho].prioridade){
            troca(&h->v[k], &h->v[menorFilho]);
            desceNoHeap(h, menorFilho);
        }
    }
}

/*
    Função que faz determinado elemento subir no heap
*/
void sobeNoHeap(pFP fPrio, int k){
    if (k > 0 && fPrio->v[PAI(k)].prioridade > fPrio->v[k].prioridade){
        troca(&fPrio->v[k], &fPrio->v[PAI(k)]);
        sobeNoHeap(fPrio, PAI(k));
    }
}

/*
    Função que diminui a prioridade de determinada posicao no heap
*/
void diminuiPrioridade(pFP h, int v, int prioridade){

    h->v[v].prioridade = prioridade;
    sobeNoHeap(h, v);
}

/*
    Função que extrai o minimo do minHeap
*/
Item extraiMinimo(pFP h){

    Item item = h->v[0];
    troca(&h->v[0], &h->v[h->n - 1]);
    h->n--;
    desceNoHeap(h, 0);

    return item;
}

/*
    Função que acha a posicao de uma cidade no heap com base em sua posicao no grafo
*/
int achaNoHeap(pFP h, int posicaoNoGrafo){

    for (int i = 0; i < h->n; i++){
        if (h->v[i].vertice == posicaoNoGrafo)
            return i;
    }

    return 0;
}

/*
    Função do 'dijkstra' que encontra os caminhos mínimos de determinada cidade para
    todas as outras
*/
double *dijkstra(pGrafo g, int s){
    double *distancias = malloc(g->n * sizeof(double));
    Item v;
    pRelacao t;
    pFP h = criarFilaPrio(g->n);

    //Setando valores iniciais e os colocando no heap
    for (int i = 0; i < g->n; i++){
        distancias[i] = INTMAX;
        insereNaFila(h, i, INTMAX);
    }

    distancias[s] = 0;
    diminuiPrioridade(h, s, 0);

    //Enquando houver elementos dentro do heap
    while (h->n != 0){
        v = extraiMinimo(h);

        if (v.prioridade != INTMAX){
            for (t = g->adjacencia[v.vertice]->prox; t != NULL; t = t->prox){
                if ((v.prioridade + t->distancia) < distancias[t->v]){

                    //Nova distância mínima para determinada cidade
                    distancias[t->v] = distancias[v.vertice] + t->distancia;
                    diminuiPrioridade(h, achaNoHeap(h, t->v), distancias[v.vertice] + t->distancia);
                }
            }
        }
    }

    free(h->v);
    free(h);

    return distancias;
}

/*
    Função cria o grafo
*/
pGrafo criarGrafo(int n){
    pGrafo g = malloc(sizeof(struct grafo));

    g->n = n;
    g->adjacencia = malloc(n * sizeof(pCidade));

    return g;
}

/*
    Função que lê todas as cidades, colocando elas na lista de adjacências do grafo
*/
int leTodasCidades(pGrafo g){

    int populacao, populacaoEstado = 0;
    char nomeCidade[52];

    for (int i = 0; i < g->n; i++){

        scanf("%s", nomeCidade);
        scanf("%d", &populacao);

        pCidade novaCidade = malloc(sizeof(struct cidade));

        novaCidade->v = i;
        novaCidade->prox = NULL;
        novaCidade->populacao = populacao;
        strcpy(novaCidade->nome, nomeCidade);

        g->adjacencia[i] = novaCidade;

        populacaoEstado += populacao;
    }

    return populacaoEstado;
}

/*
    Função que libera o espaço usado na utilização das relações dentre cidades
*/
void liberaRelacao(pRelacao lista){

    if (lista != NULL){
        liberaRelacao(lista->prox);
        free(lista);
    }
}

/*
    Função que libera o espaço usado na utilização do grafo
*/
void liberaGrafo(pGrafo g){

    for (int i = 0; i < g->n; i++){
        liberaRelacao(g->adjacencia[i]->prox);
        free(g->adjacencia[i]);
    }

    free(g->adjacencia);
    free(g);
}

/*
    Função que insere uma nova relação entre duas cidades, para representar uma ligação 
    no grafo
*/
pCidade insereNovaRelacao(pCidade lista, int v, double distancia){
    pRelacao novaRelacao = malloc(sizeof(struct relacao));

    novaRelacao->v = v;
    novaRelacao->prox = lista->prox;
    novaRelacao->distancia = distancia;

    lista->prox = novaRelacao;
    return lista;
}

/*
    Função que acha na lista de adjacências a posição de determinada cidade com base
    no seu nome 
*/
int achaPosicaoNaListaAdjacencia(pGrafo g, char nome[]){

    for (int i = 0; i < g->n; i++){

        if (strcmp(g->adjacencia[i]->nome, nome) == 0)
            return i;
    }

    return 0;
}

/*
    Função que gera as adjacêncidas entre as cidades, colocando as distâncias entre elas 
*/
void geraTodasAdjacencias(pGrafo g){

    char nomeCidade1[52], nomeCidade2[52];
    double distancia;

    while (scanf("%s %s %lf", nomeCidade1, nomeCidade2, &distancia) != EOF){

        int posicao1 = achaPosicaoNaListaAdjacencia(g, nomeCidade1);
        int posicao2 = achaPosicaoNaListaAdjacencia(g, nomeCidade2);

        //verifica se não são a mesma cidade
        if (posicao1 != posicao2){
            g->adjacencia[posicao1] = insereNovaRelacao(g->adjacencia[posicao1], posicao2, distancia);
            g->adjacencia[posicao2] = insereNovaRelacao(g->adjacencia[posicao2], posicao1, distancia);
        }
        else{
            g->adjacencia[posicao1] = insereNovaRelacao(g->adjacencia[posicao1], posicao2, 0);
        }
    }
}

/*
    Função que define os fatores de centralidade de cada cidade
*/
void defineTodosFatoresDeCentralidade(pGrafo g, int populacaoEstado){

    for(int i = 0; i < g->n; i++){

        double *distanciasMinimas, numerador, denominador;

        distanciasMinimas = dijkstra(g, i); //Encontra as distâncias mínimas para cada cidade
        numerador = denominador = 0;

        for(int j = 0; j < g->n; j++){

            //verifica se a cidade pode ou não ser alcançada
            if(distanciasMinimas[j] < INTMAX){

                numerador += distanciasMinimas[j] * g->adjacencia[j]->populacao;
                denominador += g->adjacencia[j]->populacao;
            }
        }

        //Verifica se a cidade alcança pelo menos metade do estado
        if(denominador * 2 >= populacaoEstado)
            g->adjacencia[i]->fatorCentralidade = numerador / denominador;
        else
            g->adjacencia[i]->fatorCentralidade = -1;

        free(distanciasMinimas);
    }
}

/*
    Função que mostra os fatores de centralidade de cada cidade com potêncial de ser capital
*/
void mostraTodosFatoresDeCentralidade(pGrafo g){

    for(int i = 0; i < g->n; i++){
        
        if(g->adjacencia[i]->fatorCentralidade != -1)
            printf("%s %.2lf\n", g->adjacencia[i]->nome, g->adjacencia[i]->fatorCentralidade);
    }
}

/*
    Função que ordena o grafo usando o BubbleSort. Primeiro, com relação ao fator de
    centralidade, depois, com relação ao nome de cada cidade.
*/
void ordenacao(pGrafo g){

    pCidade aux;

    for (int i = 1; i < g->n; i++) {
        for (int j = 0; j < g->n - 1; j++) {

            double valor = g->adjacencia[j]->fatorCentralidade;
            double valorComp = g->adjacencia[j + 1]->fatorCentralidade;

            if (valor > valorComp) {

                aux = g->adjacencia[j];
                g->adjacencia[j] = g->adjacencia[j + 1];
                g->adjacencia[j + 1] = aux;
            }
            else if (valor == valorComp){

                if (strcmp(g->adjacencia[j]->nome, g->adjacencia[j + 1]->nome) > 0) {
                    aux = g->adjacencia[j];
                    g->adjacencia[j] = g->adjacencia[j + 1];
                    g->adjacencia[j + 1] = aux;
                }
            }
        }
    }
}

int main(){

    pGrafo grafo;
    int numerocidades, populacaoEstado;

    scanf("%d", &numerocidades);

    grafo = criarGrafo(numerocidades);

    populacaoEstado = leTodasCidades(grafo);
    geraTodasAdjacencias(grafo);

    defineTodosFatoresDeCentralidade(grafo, populacaoEstado);
    ordenacao(grafo);
    mostraTodosFatoresDeCentralidade(grafo);

    liberaGrafo(grafo);

    return 0;
}