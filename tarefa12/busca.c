#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct no *pNo;

struct no{
    char nomeLoja[25];
    int codigoProduto;
    double valor;
    pNo esq, dir;
};

/*
    Função que recebe determinadas informações de um anúncio e adiciona na 
    árvore binária de busca
*/
pNo adicionaAnuncio(pNo raiz, char nomeLoja[], int codigoProduto, double valorProduto) {

    pNo novoAnuncio;

    if (raiz == NULL) {
        novoAnuncio = malloc(sizeof(struct no));
        novoAnuncio->esq = novoAnuncio->dir = NULL;
        novoAnuncio->codigoProduto = codigoProduto;
        novoAnuncio->valor = valorProduto;

        strcpy(novoAnuncio->nomeLoja, nomeLoja);

        return novoAnuncio;
    }

    if (valorProduto < raiz->valor)
        raiz->esq = adicionaAnuncio(raiz->esq, nomeLoja, codigoProduto, valorProduto);
    else
        raiz->dir = adicionaAnuncio(raiz->dir, nomeLoja, codigoProduto, valorProduto);

    return raiz;
}

/*
    Função que recebe determinado numero de anúncios para serem cadastrados,
    lê a informação deles e adiciona na árvore binária de busca
*/
pNo cadastraAnuncios(pNo raiz, int numeroAnuncios){

    char nomeLoja[25];
    int codigoProduto;
    double valorProduto;

    for(int i = 0; i < numeroAnuncios; i++){

        scanf("%s %d %lf ", nomeLoja, &codigoProduto, &valorProduto);

        raiz = adicionaAnuncio(raiz, nomeLoja, codigoProduto, valorProduto);
    }

    return raiz;
}

/*
    Função que recebe determinado nó dentro da árvore e adiciona esse nó e seus 
    filhos em um vetor caso eles sejam do produto procurado
*/
int adicionaBoasOfertasNaLista(pNo anuncio, pNo ofertasDisponiveis[], int posicao, int produtoProcurado){

    if(anuncio == NULL)
        return posicao;
    else {

        int posicaoAdicionar = posicao;

        //Verifica se o anúncio em questão é do produto ou não
        if(anuncio->codigoProduto == produtoProcurado){
            ofertasDisponiveis[posicaoAdicionar] = anuncio;
            posicaoAdicionar++;
        }

        //Verifica se o anúncio tem filhos válidos para talvez serem adicionados ao vetor
        if(anuncio->esq != NULL)
            posicaoAdicionar = adicionaBoasOfertasNaLista(anuncio->esq, ofertasDisponiveis, posicaoAdicionar, produtoProcurado);
        if(anuncio->dir != NULL)
            posicaoAdicionar = adicionaBoasOfertasNaLista(anuncio->dir, ofertasDisponiveis, posicaoAdicionar, produtoProcurado);

        return posicaoAdicionar;
    }
}

/*
    Função que recebe o vetor de ofertas disponíveis e ordena com base
    no nome das lojas
*/
void ordenaOfertasDisponiveis(pNo ofertasDisponiveis[], int posicaoFinal){

    for (int i = 1; i < posicaoFinal; i++) {
        for (int j = 0; j < posicaoFinal - 1; j++) {

            pNo oferta = ofertasDisponiveis[j];
            pNo ofertaComp = ofertasDisponiveis[j + 1];

            char nome[25], nomeComp[25];

            strcpy(nome, oferta->nomeLoja);
            strcpy(nomeComp, ofertaComp->nomeLoja);

            if (strcmp(nome, nomeComp) > 0) {

                ofertasDisponiveis[j] = ofertaComp;
                ofertasDisponiveis[j + 1] = oferta;
            }
        }
    }
}

/*
    Função que recebe o vetor de ofertas disponíveis e as mostra
*/
void mostraOfertasDisponiveis(pNo ofertasDisponiveis[], int posicaoFinal){

    for(int i = 0; i < posicaoFinal; i++){

        pNo atualOferta = ofertasDisponiveis[i];

        printf("\n%s %d %.2lf", atualOferta->nomeLoja, atualOferta->codigoProduto, atualOferta->valor);
    }
}

/*
    Função que recebe determinado produto e o valor dele, para procurar e colocar 
    em um vetor, os produtos que estão estão na mesma faixa de preço (todos contidos 
    na árvore recebida).
*/
void procuraOfertas(pNo raiz, int produtoProcurado, double valorProcurado, int tag){

    double maximoFaixaPreco = valorProcurado * 1.1;
    int posicaoFinal, temOferta;
    posicaoFinal = temOferta = 0;

    pNo ofertasDisponiveis[5];
    pNo atualAnuncio = raiz;

    if(tag != 0)
        printf("\n");

    printf("OFERTAS para %d:", produtoProcurado);

    while(atualAnuncio != NULL){

        //Verifica se o valor do anúncio atual está na faixa de preço
        if((maximoFaixaPreco >= atualAnuncio->valor)){

            //Verifica se o anúncio atual 
            if(atualAnuncio->codigoProduto == produtoProcurado){
                temOferta = 1;
                ofertasDisponiveis[posicaoFinal] = atualAnuncio;
                posicaoFinal++;
            }

            int inicio = posicaoFinal;

            //Se o anúncio atual está na faixa de preço, seus filhos esquerdos também estão
            //Chama a função que vai adicionar os filhos esquerdos caso sejam do mesmo produto
            posicaoFinal = adicionaBoasOfertasNaLista(atualAnuncio->esq, ofertasDisponiveis, posicaoFinal, produtoProcurado);

            if(inicio != posicaoFinal)
                temOferta = 1;
            
            atualAnuncio = atualAnuncio->dir;
        }
        else{
            atualAnuncio = atualAnuncio->esq;
        }
    }

    if(temOferta){

        ordenaOfertasDisponiveis(ofertasDisponiveis, posicaoFinal);
        mostraOfertasDisponiveis(ofertasDisponiveis, posicaoFinal);
    }
    else{
        printf("\nnenhuma boa oferta foi encontrada");
    }
}

/*
    Função que libera memória utilizada para alocar os anúncios
    que estavam na árvore
*/
void liberaMemoria(pNo anuncio){

    if (anuncio == NULL)
        return;

    liberaMemoria(anuncio->esq);
    liberaMemoria(anuncio->dir);

    free(anuncio);
}

int main(){

    int numeroAnuncios, numeroConsultas, produtoProcurado;
    double valorProcurado;
    pNo raiz = NULL;

    scanf("%d", &numeroAnuncios);

    raiz = cadastraAnuncios(raiz, numeroAnuncios);

    scanf("%d", &numeroConsultas);

    for(int i = 0; i < numeroConsultas; i++){

        scanf("%d %lf", &produtoProcurado, &valorProcurado);

        procuraOfertas(raiz, produtoProcurado, valorProcurado, i);
        printf("\n");
    }

    liberaMemoria(raiz);
    return 0;
}