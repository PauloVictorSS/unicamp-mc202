#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct no *pNo;
typedef struct cela *pCela;
typedef struct grafo *pGrafo;

struct no{
    int v;
    pNo prox;
};

struct cela{
    int v;
    char nome[32];
    int niveisMinimos[6];
    pNo prox;
};

struct grafo{
    int n;
    pCela *adjacencia;
};

/*
    Função que cria o grafo de tamanho 'n'
*/
pGrafo criarGrafo(int n){
    pGrafo g = malloc(sizeof(struct grafo));

    g->n = n;
    g->adjacencia = malloc(n * sizeof(pCela));

    return g;
}

/*
    Função que libera o espaço utilizado para armazenar cada uma das listas
*/
void liberaLista(pNo lista) {

    if (lista != NULL) {
        liberaLista(lista->prox);
        free(lista);
    }
}

/*
    Função que libera o espaço utilizado para armazenar o grafo
*/
void liberaGrafo(pGrafo g) {
    int i;

    for (i = 0; i < g->n; i++){
        liberaLista(g->adjacencia[i]->prox);
        free(g->adjacencia[i]);
    }

    free(g->adjacencia);
    free(g);
}

/*
    Função que lê todas as celas e coloca elas na lista de adjacências do grafo
*/
void leTodasCelas(pGrafo g){

    int nivelMinimo;
    char nomeCela[32];

    for(int i = 0; i < g->n; i++){

        scanf("%s", nomeCela);

        pCela novaCela = malloc(sizeof(struct cela));

        novaCela->v = i;
        novaCela->prox = NULL;
        strcpy(novaCela->nome, nomeCela);

        for(int j = 0; j < 6; j++){

            scanf("%d", &nivelMinimo);
            novaCela->niveisMinimos[j] = nivelMinimo;
        }

        g->adjacencia[i] = novaCela;
    }
}

/*
    Função que recebe um inteiro, representando uma cela e insere ele na lista de adjacência
    referente a uma determinada cela
*/
pCela insereNaLista(pCela lista, int v) {
    pNo novo = malloc(sizeof(struct no));
    novo->v = v;
    novo->prox = lista->prox;
    lista->prox = novo;

    return lista;
}

/*
    Função que recebe duas celas e verifica se elas são 'vizinhas' (verifica se a cela 1
    pode acessar a cela 2 em apenas uma rodada)
*/
int verificaSeSaoSelasVizinhas(pCela cela1, pCela cela2){

    int cont = 0, status = 1;

    while(cont < 6 && status >= 0){

        if(cela1->niveisMinimos[cont] < cela2->niveisMinimos[cont])
            status -= cela2->niveisMinimos[cont] - cela1->niveisMinimos[cont];

        cont++;
    }

    if(status >= 0)
        return 1;

    return 0;
}

/*
    Função que recebe o grafo e gera todas as adjacências existentes nele, colocando
    essas relações em uma lista de adjacências
*/
void geraTodasAdjacencias(pGrafo g){

    for(int i = 0; i < g->n; i++){
    
        pCela cela1 = g->adjacencia[i];

        for(int j = 0; j < g->n; j++){
            if(i != j){

                pCela cela2 = g->adjacencia[j];

                if(verificaSeSaoSelasVizinhas(cela1, cela2))
                    g->adjacencia[i] = insereNaLista(g->adjacencia[i], j);
            }
        }
    }
}

/*
    Função que recebendo o nível inicial de determinado jogador, determina qual a cela em
    que ele deve começar o jogo
*/
int verificaCelaInicial(pGrafo g, int nivelInicialJogador[]){

    int i;

    for (int u = 0; u < g->n; u++){

        for(i = 0; i < 6; i++){

            if(g->adjacencia[u]->niveisMinimos[i] != nivelInicialJogador[i])
                break;
        }

        if(i == 6)
            return u;
    }

    return 0;
}

/*
    Função que lê todos os jogadores e coloca no vetor 'celasIniciais' a cela 
    inicial de cada um desses jogadores
*/
void leTodosJogadores(pGrafo g, int celasIniciais[], int numeroJogadores){

    char nomeJogador[32];
    int *nivelInicial;
    nivelInicial = malloc(6 * sizeof(int));

    for(int i = 0; i < numeroJogadores; i++){

        scanf("%s", nomeJogador);

        for(int j = 0; j < 6; j++)
            scanf("%d", &nivelInicial[j]);

        celasIniciais[i] = verificaCelaInicial(g, nivelInicial);
    }

    free(nivelInicial);
}

/*
    Função de ordenação BubbleShort para ordenar o vetor de resultados
*/
void ordenacao(int lista[], int tamanho){

    for (int i = 1; i < tamanho; i++) {
        for (int j = 0; j < tamanho - 1; j++) {

            int valor = lista[j];
            int valorComp = lista[j + 1];

            if (valor > valorComp) {

                lista[j] = valorComp;
                lista[j + 1] = valor;
            }
        }
    }
}

/*
    Função que recebe as possíveis celas de encontro final e mostra o nome de todas elas,
    ou a frase "Impossivel terminar em duas rodadas.", caso não tenha nenhuma cela
*/
void imprimeTodasCelasDeEncontro(pGrafo g, int celasDeEncontro[], int totalResultados){

    if(totalResultados > 0){
        for(int i = 0; i < totalResultados; i++)
            printf("%s\n", g->adjacencia[celasDeEncontro[i]]->nome);
    }
    else{
        printf("\nImpossivel terminar em duas rodadas.\n");
    }
}

/*
    Função que recebe duas posições na lista de adjacências e verifica se 'u' está
    ligado em 'v' (no caso, se 'u' acessa 'v')
*/
int temLigacao(pGrafo g, int u, int v) {

    for (pNo t = g->adjacencia[u]->prox; t != NULL; t = t->prox)
        if (t->v == v)
            return 1;

    return 0;
}

/*
    Função que recebe uma lista e retorna '1' caso 'w' esteja contido nessa lista
    e '0' caso contrário
*/
int estaContidoNaLista(int lista[], int w, int numeroMaxCelas){

    for(int j = 0; j < numeroMaxCelas; j++){

        if(lista[j] == -1)
            return 0;

        if(lista[j] == w)
            return 1;
    }

    return 0;
}

/*
    Função que recebe o número de celas e inicia o vetor de celas de encontro, que será
    o vetor com o resultado final do problema
*/
int* iniciaCelasDeEncontro(int numeroCelas){

    int *celasDeEncontro;
    celasDeEncontro =  malloc(numeroCelas * sizeof(int));

    for(int i = 0; i < numeroCelas; i++)
        celasDeEncontro[i] = -1;

    return celasDeEncontro;
}

/*
    Função que recebe as celas iniciais de cada jogar e calcula, colocando em uma matriz, 
    a relação de possíveis celas finais, em até duas rodadas, de cada jogador, com base
    na cela inicial de cada um
*/
void calculaRelacaoCelasFinaisPorJogador(pGrafo g, int celasIniciais[], int *relacaoCelasFinaisPorJogador[], int numeroJogadores){

    int i, v, w;

    for(i = 0; i < numeroJogadores; i++){
        int cont = 0;
        int u = celasIniciais[i]; //Cela inicial do 'i' jogador

        //Iniciando o vetor de celas para esse jogador
        relacaoCelasFinaisPorJogador[i] = malloc(g->n * sizeof(int *));
        for(int j = 0; j < g->n; j++)
            relacaoCelasFinaisPorJogador[i][j] = -1;

        //Colocando nesse vetor as celas que podem ser acessadas em uma rodada
        for (pNo t = g->adjacencia[u]->prox; t != NULL; t = t->prox){
            relacaoCelasFinaisPorJogador[i][cont] = t->v;
            cont++;
        }

        //Está rodando rodas as celas possíveis, onde 'v' é a posição de uma
        for (v = 0; v < g->n; v++) {

            if (temLigacao(g, u, v)) {

                //Está rodando rodas as celas possíveis, onde 'w' é a posição de uma
                for (w = 0; w < g->n; w++) {
                    if ((temLigacao(g, v, w)) && (!estaContidoNaLista(relacaoCelasFinaisPorJogador[i], w, g->n))){

                        //Colocando nesse vetor as celas que podem ser acessadas na segunda rodada
                        relacaoCelasFinaisPorJogador[i][cont] = w;
                        cont++;
                    }
                }
            }
        }
    }
}

/*
    Função que recebe a relação de possíveis celas finais de cada jogador e, 
    comparando essa infomação, acha as celas em comum a todos os jogadores,
    returnando a quantidade dessas celas comuns
*/
int calculaTodasCelasDeEncontro(int *relacaoCelasFinaisPorJogador[], int numeroJogadores, int numeroMaxCelas, int celasDeEncontro[]){

    int i, j, k;
    int totalResultados = 0;

    //Para cada jogador na posição 'i' temos as celas finais para ele na posição 'j'
    for(i = 0; i < numeroJogadores; i++){
        for(j = 0; j < numeroMaxCelas; j++){

            if(!estaContidoNaLista(celasDeEncontro, relacaoCelasFinaisPorJogador[i][j], totalResultados)){

                //Verifica se a cela na posição '[i][j]' é uma cela final para todos os jogadores
                for(k = 0; k < numeroJogadores; k++){
            
                    if(!estaContidoNaLista(relacaoCelasFinaisPorJogador[k], relacaoCelasFinaisPorJogador[i][j], numeroMaxCelas))
                        break;
                }

                if(k == numeroJogadores){
                    celasDeEncontro[totalResultados] = relacaoCelasFinaisPorJogador[i][j];
                    totalResultados++;
                }
            }
        }
    }

    return totalResultados;
}

int main(){

    pGrafo grafo;
    int **relacaoCelasFinaisPorJogador;
    int numeroCelas, numeroJogadores, *celasIniciais, *celasDeEncontro, totalResultados;

    scanf("%d", &numeroCelas);

    grafo = criarGrafo(numeroCelas);

    leTodasCelas(grafo);
    geraTodasAdjacencias(grafo);

    scanf("%d", &numeroJogadores);

    celasIniciais = malloc(numeroJogadores * sizeof(int));
    relacaoCelasFinaisPorJogador = malloc(numeroJogadores * sizeof(int *));

    leTodosJogadores(grafo, celasIniciais, numeroJogadores);

    //Funções principais que acham todas as celas de encontro possíveis
    celasDeEncontro = iniciaCelasDeEncontro(numeroCelas);
    calculaRelacaoCelasFinaisPorJogador(grafo, celasIniciais, relacaoCelasFinaisPorJogador, numeroJogadores);
    totalResultados = calculaTodasCelasDeEncontro(relacaoCelasFinaisPorJogador, numeroJogadores, grafo->n, celasDeEncontro);

    ordenacao(celasDeEncontro, totalResultados);
    imprimeTodasCelasDeEncontro(grafo, celasDeEncontro, totalResultados);

    //Chamadas referentes a liberação da memória utilizada
    for(int i = 0; i < numeroJogadores; i++)
        free(relacaoCelasFinaisPorJogador[i]);

    free(relacaoCelasFinaisPorJogador);
    free(celasDeEncontro);
    free(celasIniciais);
    liberaGrafo(grafo);

    return 0;
}