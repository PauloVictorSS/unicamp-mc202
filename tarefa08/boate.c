#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pessoa *pPessoa;

struct pessoa{
    int documento;
    char profissao[28];
    char estadoVacinacao[15];
    bool lanterna;
    pPessoa prox;
};

/*
    Cria uma lista vazia
*/
pPessoa criaFilaVazia(){
    return NULL;
}

/*
    Retorna True se a pessoa for famosa, e False 
    caso contrário
*/
bool pessoaEhFamosa(char profissao[]){

    return ((strcmp(profissao, "filho-politico") == 0) || 
            (strcmp(profissao, "ex-bbb") == 0) ||
            (strcmp(profissao, "youtuber-financas") == 0) ||
            (strcmp(profissao, "coach") == 0) ||
            (strcmp(profissao, "herdeiro") == 0));
}

/*
    Retorna True se a pessoa for um funcionário, e 
    False caso contrário
*/
bool pessoaEhFuncionario(char profissao[]){

    return (strcmp(profissao, "funcionario") == 0);
}

/*
    Retorna True se a pessoa for um fiscal, e False
    caso contrário
*/
bool pessoaEhFiscal(char profissao[]){

    return (strcmp(profissao, "fiscal") == 0);
}

/*
    Retorna True se a pessoa for vacinada, e False
    caso contrário
*/
bool pessoaEhVacinada(char estadoVacinacao[]){

    return (strcmp(estadoVacinacao, "vacinado") == 0);
}

/*
    Procura e retorna a pessoa que está com a
    lanterna na fila
*/
pPessoa procuraQuemTemLanterna(pPessoa fila){

    pPessoa atual;

    for(atual = fila; atual->lanterna != true; atual = atual->prox);

    return atual;
}

/*
    Descobre e retorna a última pessoa da fila
*/
pPessoa descobreUltimaPessoa(pPessoa fila){

    pPessoa atual;

    for(atual = fila; atual->prox != NULL; atual = atual->prox);

    return atual;
}

/*
    Remove as pessoas não-vacinadas logo atrás do fiscal,
    retornando a pessoa que estiver vacinada
*/
pPessoa removePessoasNaoVacinadas(pPessoa atual){

    pPessoa pessoaVacinada = atual;

    if(atual != NULL)
        if(!pessoaEhVacinada(atual->estadoVacinacao)){
            pessoaVacinada = removePessoasNaoVacinadas(atual->prox);
            free(atual);
        }
        
    return pessoaVacinada;
}

/*
    Adiciona as pessoas na fila com base em determinada
    posição, com base em algumas regras
*/
pPessoa adicionaPessoasNaFila(pPessoa fila){

    int documento;
    char profissao[28], estadoVacinacao[15];

    while (scanf("%d %s %s", &documento, profissao, estadoVacinacao) != EOF) {

        pPessoa novaPessoa = malloc(sizeof(struct pessoa));
        novaPessoa->documento = documento;
        strcpy(novaPessoa->profissao, profissao);
        strcpy(novaPessoa->estadoVacinacao, estadoVacinacao);

        //Verifica se já tem algúem na fila
        if(fila != NULL){

            pPessoa pessoaComLanterna = procuraQuemTemLanterna(fila);
        
            pessoaComLanterna->lanterna = false;

            if(pessoaEhFamosa(profissao)){

                //Coloca a pessoa no início da fila
                novaPessoa->prox = fila;
                fila = novaPessoa;
            }
            else if(pessoaEhFiscal(pessoaComLanterna->profissao) && !pessoaEhVacinada(estadoVacinacao)){

                pessoaComLanterna->lanterna = true;
                continue;
            }
            else if(pessoaEhFuncionario(profissao)){

                pPessoa ultimaPessoa = descobreUltimaPessoa(fila);

                //Coloca a pessoa no final da fila
                novaPessoa->prox = NULL;
                ultimaPessoa->prox = novaPessoa;

            }
            else if(pessoaEhFiscal(profissao)){

                //Retira todas as pessoas logo atrás do fiscal que não vacinaram
                pPessoa pessoaVacinada = removePessoasNaoVacinadas(pessoaComLanterna->prox);

                novaPessoa->prox = pessoaVacinada;
                pessoaComLanterna->prox = novaPessoa;
            }
            else{

                //Coloca a pessoa atrás de quem tem lanterna
                novaPessoa->prox = pessoaComLanterna->prox;
                pessoaComLanterna->prox = novaPessoa;
            }
        }
        else{

            novaPessoa->prox = fila;
            fila = novaPessoa;
        }

        novaPessoa->lanterna = true;
    }

    return fila;
}

/*
    Mostra a fila
*/
void mostraFilaFinal(pPessoa fila){

    pPessoa atual = fila;

    if(atual != NULL){
        printf("%d %s %s\n", atual->documento, atual->profissao, atual->estadoVacinacao);
        mostraFilaFinal(atual->prox);
    }
}

/*
    Desaloca a memória utlizada para a fila
*/
void desalocaMemoriaDaFila(pPessoa fila){

    if(fila != NULL){
        desalocaMemoriaDaFila(fila->prox);
        free(fila);
    }
}

int main(){

    pPessoa fila;

    fila = criaFilaVazia();
    fila = adicionaPessoasNaFila(fila);
    mostraFilaFinal(fila);
    desalocaMemoriaDaFila(fila);
}