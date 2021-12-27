#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct no *pNo;
typedef struct hash *pHash;
typedef struct stopWord *pStopWord;

struct no {
    char palavra[60];
    int numeroRepeticoes;
    pNo prox;
};
struct hash {
    pNo *vetor;
    int tamanho;
};
struct stopWord {
    char palavra[60];
    pStopWord prox;
};

/*
    Função que lê um número 'numeroStopWords' de palavras e cooloca em uma
    fila ligada chamada 'filaStopWord'
*/
pStopWord leTodasStopWords(pStopWord filaStopWord, int numeroStopWords){

    char palavra[60];

    for(int i = 0; i < numeroStopWords; i++){

        scanf("%s", palavra);

        pStopWord novaStopWord = malloc(sizeof(struct stopWord));

        strcpy(novaStopWord->palavra, palavra);
        novaStopWord->prox = filaStopWord;

        filaStopWord = novaStopWord;
    }

    return filaStopWord;
}

/*
    Libera memória utilizada para guardar as Stop Words
*/
void liberaFilaStopWords(pStopWord stopWordAtual){

    if(stopWordAtual != NULL){

        liberaFilaStopWords(stopWordAtual->prox);
        free(stopWordAtual);
    }
}

/*
    Função de HASH que retorna um numero entre 0 e 'tamanho'
    com base na palavra recebida
*/
int hash(char palavra[], int tamanho) {
    int i, n = 0;

    for (i = 0; i < strlen(palavra); i++)
        n = (256 * n + palavra[i]) % tamanho;

    return n;
}

/*
    Função que procura 'palavra' em uma fila, onde retorna NULL
    caso não ache e o nó, caso ache
*/
pNo buscaPalavra(pNo atualNo, char palavra[]){

    if(atualNo == NULL)
        return atualNo;
    
    if(strcmp(atualNo->palavra, palavra) == 0)
        return atualNo;

    return buscaPalavra(atualNo->prox, palavra);
}

/*
    Função que insere na lista determinada palavra caso ela não
    esteja lá, e aumenta o contador de repetições dela caso ela
    esteja
*/
pNo inserirLista(pNo primeiroNo, char palavra[]){

    pNo palavraBuscada = buscaPalavra(primeiroNo, palavra);

    if(palavraBuscada != NULL){
        palavraBuscada->numeroRepeticoes++;
    }
    else{
        pNo novoNo = malloc(sizeof(struct no));

        strcpy(novoNo->palavra, palavra);
        novoNo->numeroRepeticoes = 1;
        novoNo->prox = primeiroNo;

        primeiroNo = novoNo;
    }

    return primeiroNo;
}

/*
    Função que retorna 1 caso  determinada palabra não se encontre na lista
    de StopWords, e 0 caso ela esteja
*/
int naoEhStopWord(char palavra[], pStopWord atualStopWord){

    if(atualStopWord == NULL)
        return 1;

    if(strcmp(atualStopWord->palavra, palavra) == 0)
        return 0;
    
    return naoEhStopWord(palavra, atualStopWord->prox);
}

/*
    Função que trata determinada palavra, transformando tudo em minuscula e 
    tirando simbolos e números
*/
void trataPalavra(char novaPalavra[], char palavraTratada[]){

    int cont = 0, posicao = 0;

    while(novaPalavra[cont] != '\0'){

        char algarismo = tolower(novaPalavra[cont]);

        if(islower(algarismo)){
            palavraTratada[posicao] = algarismo;
            posicao++;
        }

        cont++;
    }

    palavraTratada[posicao] = '\0';
}

/*
    Função que gerencia a nuvem de palabras, lendo todas as 
    palavras de um texto e colocando elas em determinadas posições
    de uma tabela de espalhamento com base em um HASH calculado
*/
void gerenciaNuvemPalavras(pHash t, pStopWord filaStopWord){

    char novaPalavra[60];
    char palavraTratada[60];

    while (scanf("%s", novaPalavra) != EOF) {

        trataPalavra(novaPalavra, palavraTratada);

        if(strlen(palavraTratada) > 1){
            if(naoEhStopWord(palavraTratada, filaStopWord)){
                
                int n = hash(palavraTratada, t->tamanho);
                t->vetor[n] = inserirLista(t->vetor[n], palavraTratada);
            }
        }
    }
}

/*
    Função que recebe um vetor de 'pNo' e mostra as informações
    de cada um dos nós
*/
void mostraResultados(pNo vetor[], int tamanho){

    for(int i = 0; i < tamanho && i < 50; i++)
        printf("%s %d\n", vetor[i]->palavra, vetor[i]->numeroRepeticoes);
}

/*
    Funções que libera a memoria utililizada na tabela de espalhamento
*/
void liberaFila(pNo atualNo){

    if (atualNo == NULL)
        return;

    liberaFila(atualNo->prox);
    free(atualNo);
}
void liberaTabelaEspalhamento(pHash t){

    for(int i = 0; i < t->tamanho; i++)
        liberaFila(t->vetor[i]);

    free(t->vetor);
    free(t);
}

/*
    Função transfere os valores não nulos da tabela de espalhamento
    e coloca eles dentro de um vetor de nós
*/
int transfereResultados(pHash t, pNo vetorResultado[]){

    int cont = 0;
    pNo *vetor = t->vetor;

    for(int i = 0; i < t->tamanho; i++)
        for(pNo atual = vetor[i]; atual != NULL; atual = atual->prox){

            vetorResultado[cont] = atual;
            cont++;
        }
    
    return cont;
}

/*
    Função ordena determinado vetor com o método MergeSort
*/
void merge(pNo vetor[], int comeco, int meio, int fim) {
    pNo *vetAux;
    vetAux = malloc((fim - comeco + 1) * sizeof(struct no));
    int cont1 = comeco, cont2 = meio+1, contAux = 0;

    while(cont1 <= meio && cont2 <= fim){

        if(vetor[cont1]->numeroRepeticoes > vetor[cont2]->numeroRepeticoes) {
            vetAux[contAux] = vetor[cont1];
            cont1++;
        }
        else if(vetor[cont1]->numeroRepeticoes < vetor[cont2]->numeroRepeticoes) {
            vetAux[contAux] = vetor[cont2];
            cont2++;
        }
        else{
            if(strcmp(vetor[cont1]->palavra, vetor[cont2]->palavra) < 0) {
                vetAux[contAux] = vetor[cont1];
                cont1++;
            }
            else{
                vetAux[contAux] = vetor[cont2];
                cont2++;
            }
        }
        contAux++;
    }

    while(cont1 <= meio){
        vetAux[contAux] = vetor[cont1];
        contAux++;
        cont1++;
    }

    while(cont2 <= fim) {
        vetAux[contAux] = vetor[cont2];
        contAux++;
        cont2++;
    }

    for(contAux = comeco; contAux <= fim; contAux++){
        vetor[contAux] = vetAux[contAux - comeco];
    }
    
    free(vetAux);
}

/*
    Base da função MergeSort
*/
void mergeSort(pNo vetor[], int comeco, int fim){
    if (comeco < fim) {
        int meio = (fim+comeco)/2;

        mergeSort(vetor, comeco, meio);
        mergeSort(vetor, meio+1, fim);
        merge(vetor, comeco, meio, fim);
    }
}

int main(){

    pNo *vetorResultado;
    pStopWord filaStopWord = NULL;
    int numeroPalavrasDistintas, numeroStopWords;

    pHash tabelaEspalhamento = malloc(sizeof(struct hash));

    scanf("%d", &numeroPalavrasDistintas);
    scanf("%d", &numeroStopWords);

    tabelaEspalhamento->tamanho = numeroPalavrasDistintas;
    tabelaEspalhamento->vetor = calloc(numeroPalavrasDistintas, sizeof(struct no));

    vetorResultado = calloc(numeroPalavrasDistintas, sizeof(struct no));

    filaStopWord = leTodasStopWords(filaStopWord, numeroStopWords);

    gerenciaNuvemPalavras(tabelaEspalhamento, filaStopWord);

    //Transfere os resultados para um novo vetor, ordena ele e mostra os resultados
    int totalResultados = transfereResultados(tabelaEspalhamento, vetorResultado);
    mergeSort(vetorResultado, 0, totalResultados - 1);
    mostraResultados(vetorResultado, totalResultados);

    //Libera toda memória utilizada
    free(vetorResultado);
    liberaFilaStopWords(filaStopWord);
    liberaTabelaEspalhamento(tabelaEspalhamento);

    return 0;
}