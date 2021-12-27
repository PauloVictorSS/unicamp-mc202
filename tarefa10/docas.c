#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct doca *pDoca;
typedef struct navio *pNavio;
typedef struct container *pContainer;

struct doca{
    int espacoDisponivel;
    pContainer pilhaContainers;
};

struct navio{
    char nome[25], objetivo[25], tipoMercadoria[25];
    int acoesRestantes;
    pNavio prox;
};

struct container{
    char tipoMercadoria[25];
    pContainer prox;
};

/*
    Cria o conjunto de docas, com base em um número de docas e na 
    capacidade de containers por doca. Cada doca é colocada em uma
    posição de um vetor
*/
void criaAsDocas(pDoca docas[], int numeroDocas, int capacidadePorDoca){

    for(int i = 0; i < numeroDocas; i++){

        //Aloca espaço na memória para a doca
        pDoca novaDoca = malloc(sizeof(struct doca));

        //Coloca as informações da doca
        novaDoca->espacoDisponivel = capacidadePorDoca;
        novaDoca->pilhaContainers = NULL;

        //Adiciona a doca no vetor
        docas[i] = novaDoca;
    }
}

/*
    Função que cria a fila de navios
*/
pNavio criaFilaDeNavios(pNavio filaDeNavios){

    char nomeNavio[25], objetivo[25], tipoMercadoria[25];
    int acoesRestantes;

    pNavio fimFilaNavios = NULL;

    //Recebe os dados do navio
    while (scanf("%s %s %s %d", nomeNavio, objetivo, tipoMercadoria, &acoesRestantes) != EOF) {

        pNavio novoNavio = malloc(sizeof(struct navio));

        strcpy(novoNavio->nome, nomeNavio);
        strcpy(novoNavio->objetivo, objetivo);
        strcpy(novoNavio->tipoMercadoria, tipoMercadoria);
        novoNavio->acoesRestantes = acoesRestantes;

        novoNavio->prox = NULL;

        //Caso não seja o primeiro navio, ele vai para o fim
        if(filaDeNavios != NULL)
            fimFilaNavios->prox = novoNavio;
        else
            filaDeNavios = novoNavio;

        //O fim da fila aponta para o novo navio
        fimFilaNavios = novoNavio;
    }

    return filaDeNavios;
}

/*
    Recebendo o conjunto de docas, o número de docas e o tipo de mercadoria esperada,
    retorna a posição da doca caso o container do topo da pilha em questão for do
    mesmo tipo de mercadoria esperada, e retorna -1 caso contrário 
*/
int docaOferecendoMercadoriaDesejada(pDoca docas[], int numeroDocas, char tipoMercadoriaEsperada[]){

    //Passa por todas as docas
    for(int i = 0; i < numeroDocas; i++){

        pContainer containerDeCima = docas[i]->pilhaContainers;

        //Verifica se o container de cima existe, caso exista, verifica se está com o tipo de mercadoria esperada
        if(containerDeCima != NULL){
            if(strcmp(containerDeCima->tipoMercadoria, tipoMercadoriaEsperada) == 0){ 
                return i;
            }
        }
    }

    return -1;
}

/*
    Recebendo o conjunto de docas e  o número de docas, retorna a posiçao da
    primeira doca que tiver espaço livre, caso nao tenha nenhum, retorna -1
*/
int docaOferecendoEspacoVazio(pDoca docas[], int numeroDocas){

    //Passa por todas as docas
    for(int i = 0; i < numeroDocas; i++){

        pDoca doca = docas[i];

        //Verifica se a doca tem espaço disponível
        if(doca->espacoDisponivel > 0)
            return i;
    }

    return -1;
}

/*
    Acha o final da fila de navios
*/
pNavio achaFimDaFila(pNavio filaDeNavios){

    pNavio atualNavio;

    for(atualNavio = filaDeNavios; atualNavio->prox != NULL; atualNavio = atualNavio->prox);

    return atualNavio;
}

/*
    Recebe um navio e determinada pilha de containers, simula uma adicao
    de containers ao navio até o tipo de mercadoria do navio ser incompativel
    com o que se tem na pilha, ou ainda até o navio se encher. Retorna a 
    quantidade de containers colocados no navio
*/
void carregaNavio(pNavio navio, pDoca doca){

    pContainer atualContainer = doca->pilhaContainers;

    //Verifica se ainda há containers na pilha
    if(atualContainer != NULL){

        //Verifica se o tipo de mercadoria do container em questão é igual ao do navio, e se o navio não está cheio
        if((strcmp(atualContainer->tipoMercadoria, navio->tipoMercadoria) == 0) && (navio->acoesRestantes > 0)){

            navio->acoesRestantes--;
            doca->espacoDisponivel++;
            doca->pilhaContainers = atualContainer->prox;

            //Chama a função, passando o próximo container
            carregaNavio(navio, doca);
            free(atualContainer); //Libera o espaço na memória do container
        }
    }
}

/*
    Recebe um navio e determinada doca, simula o descarregamento de containers
    do navio para a doca, enquanto houver espaço na doca e o navio estiver com
    carga. Retorna o número de containers deixados na doca
*/
int descarregaNavio(pNavio navio, pDoca doca){

    int containersDescarregados = 0;
    char tipoMercadoria[25];

    //Guarta o tipo de mercadoria que será colocado na doca
    strcpy(tipoMercadoria, navio->tipoMercadoria);

    //'Coloca' os containers enquanto houver espaço na doca e enquanto o navio não estiver vazio
    while((containersDescarregados < doca->espacoDisponivel) && (navio->acoesRestantes > containersDescarregados)){

        //Aloca memória para o novo container
        pContainer novoContainer = malloc(sizeof(struct container));
        strcpy(novoContainer->tipoMercadoria, tipoMercadoria);

        //Coloca o container no topo da pilha
        novoContainer->prox = doca->pilhaContainers;
        doca->pilhaContainers = novoContainer;

        //Aumentando a quantidade de containers descarregados
        containersDescarregados++;
    }

    return containersDescarregados;
}

/*
    Libera a memoria alocada para a fila de navios
*/
void liberaRestanteMemoria(pNavio filaDeNavios){

    pNavio atualNavio = filaDeNavios;

    if(atualNavio != NULL){
        liberaRestanteMemoria(atualNavio->prox);
        free(atualNavio);
    }
}

/*
    Função que faz o papel de gerenciar o fluxo dos navios, verifica qual
    o objetivo de cada navio e, dependendo do objetivo, chama determinado 
    conjunto de funções. Mostrando as movimentações que acontecerem
*/
void gerenciaFluxoDeNavios(pNavio filaDeNavios, pDoca docas[], int numeroDocas, int capacidadePorDoca, int numeroNaviosRestantes){

    int numeroTentativasFalhasSeguidas = 0;
    pNavio ultimoDaFila = achaFimDaFila(filaDeNavios);

    //Enquanto houver operação não realizada para fazer
    while(numeroNaviosRestantes > numeroTentativasFalhasSeguidas){

        int posicaoDoca, numeroAcoesFeitas = 0;
        pNavio atualNavio = filaDeNavios;

        //Verifica qual objetivo do atual navio
        if((strcmp(atualNavio->objetivo, "carrega") == 0)){

            //Recebe a posição da doca com a mercadoria desejada, -1 caso não tenha
            posicaoDoca = docaOferecendoMercadoriaDesejada(docas, numeroDocas, atualNavio->tipoMercadoria);
            
            if(posicaoDoca != -1){
                numeroAcoesFeitas = atualNavio->acoesRestantes;
                carregaNavio(atualNavio, docas[posicaoDoca]); 

                //Calcula o número de ações feitas
                numeroAcoesFeitas -= atualNavio->acoesRestantes;
            }
        }
        else{

            //Recebe a posicao da doca com espaço vazio, -1 caso não tenha
            posicaoDoca = docaOferecendoEspacoVazio(docas, numeroDocas);
            
            if(posicaoDoca != -1){
                numeroAcoesFeitas = descarregaNavio(atualNavio, docas[posicaoDoca]);

                //Atualiza os dados das docas e do navio
                docas[posicaoDoca]->espacoDisponivel -= numeroAcoesFeitas;
                atualNavio->acoesRestantes -= numeroAcoesFeitas;
            }
        }

        //Verifica se alguma ação foi feita pelo navio
        if(numeroAcoesFeitas > 0){
            printf("%s\t%s\t%s\tdoca: %d\tconteineres: %d\n", atualNavio->nome, atualNavio->objetivo, atualNavio->tipoMercadoria, posicaoDoca, numeroAcoesFeitas);
            numeroTentativasFalhasSeguidas = 0;
        }
        else{
            numeroTentativasFalhasSeguidas++;
        }

        filaDeNavios = atualNavio->prox;

        //Verifica se o navio ainda tem coisa para fazer
        if(atualNavio->acoesRestantes > 0){
            ultimoDaFila->prox = atualNavio;
            ultimoDaFila = atualNavio;
            atualNavio->prox = NULL;
        }
        else{
            numeroNaviosRestantes--;
            free(atualNavio);
        }
    }

    //Verifica se é impossível esvaziar a fila
    if((numeroNaviosRestantes == numeroTentativasFalhasSeguidas) && (numeroNaviosRestantes != 0)){
        liberaRestanteMemoria(filaDeNavios);
        printf("ALERTA: impossivel esvaziar fila, restam %d navios.", numeroNaviosRestantes);
    }
}

int main(){

    int numeroDocas, capacidadePorDoca, numeroNavios;
    pNavio filaDeNavios = NULL;
    pDoca *docas;

    scanf("%d", &numeroDocas);
    scanf("%d", &capacidadePorDoca);
    scanf("%d", &numeroNavios);

    docas = malloc(numeroDocas * sizeof(pDoca));
    criaAsDocas(docas, numeroDocas, capacidadePorDoca);
    filaDeNavios = criaFilaDeNavios(filaDeNavios);

    gerenciaFluxoDeNavios(filaDeNavios, docas, numeroDocas, capacidadePorDoca, numeroNavios);

    for(int i = 0; i < numeroDocas; i++){
        free(docas[i]);
    }
    free(docas);
    return 0;
}