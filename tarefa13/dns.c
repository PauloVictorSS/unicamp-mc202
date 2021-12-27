#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum cor {VERMELHO , PRETO};
typedef struct dominio *pDominio;
typedef struct usuario *pUsuario;

struct dominio{
    char nome[101], enderecoIP[101];
    enum cor cor;
    pDominio esq, dir;
};

struct usuario{
    int consultasRealizadas;
    char enderecoIP[101];
    enum cor cor;
    pUsuario esq, dir;
};

/*
    Duas funções que recebem determinado nós de suas árvores correspondentes
    e verificam se ele é VERMELHO ou não
*/
int ehVermelhoDominio(pDominio x) {
    if (x == NULL)
        return 0;

    return x->cor == VERMELHO;
}
int ehVermelhoUsuario(pUsuario x) {
    if (x == NULL)
        return 0;

    return x->cor == VERMELHO;
}

/*
    Duas funções que recebem determinado nós de suas árvores correspondentes
    e verificam se ele é PRETO ou não
*/
int ehPretoDominio(pDominio x) {
    if (x == NULL)
        return 1;

    return x->cor == PRETO;
}
int ehPretoUsuario(pUsuario x) {
    if (x == NULL)
        return 1;

    return x->cor == PRETO;
}

/*
    Duas funções que recebem determinado nós de suas árvores correspondentes
    e rotacionam para a ESQUERDA essa parte da árvore
*/
pDominio rotacionaParaEsquerdaDominio(pDominio raiz) {
    pDominio x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;

    return x;
}
pUsuario rotacionaParaEsquerdaUsuario(pUsuario raiz) {
    pUsuario x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;

    return x;
}

/*
    Duas funções que recebem determinado nós de suas árvores correspondentes
    e rotacionam para a DIREITA essa parte da árvore
*/
pDominio rotacionaParaDireitaDominio(pDominio raiz) {
    pDominio x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;

    return x;
}
pUsuario rotacionaParaDireitaUsuario(pUsuario raiz) {
    pUsuario x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;

    return x;
}

/*
    Duas funções que recebem determinado nós de suas árvores correspondentes
    e "sobem" o vermelho dos 'filhos' desse nó, para o próprio nó, deixando os
    filhos pretos
*/
void sobeVermelhoDominio(pDominio raiz) {
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}
void sobeVermelhoUsuario(pUsuario raiz) {
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}

/*
    Função que recebe um conjunto de informações sobre um domínio e insere ele 
    na árvore rubro-negra esquerdista dos domínios
*/
pDominio inserirDominio(pDominio raiz, char nome[], char enderecoIP[]) {

    pDominio novo;

    if (raiz == NULL) {
        novo = malloc(sizeof(struct dominio));
        novo->esq = novo->dir = NULL;

        strcpy(novo->nome, nome);
        strcpy(novo->enderecoIP, enderecoIP);

        novo->cor = VERMELHO;
        return novo;
    }

    //Verifica se o nome é maior ou menor que a da raiz, para decidir onde colocar o novo nó
    if (strcmp(raiz->nome, nome) > 0)
        raiz->esq = inserirDominio(raiz->esq, nome, enderecoIP);
    else
        raiz->dir = inserirDominio(raiz->dir, nome, enderecoIP);

    //Corrige a árvore com base nas propriedades de uma árvore Rubro-Negra
    if (ehVermelhoDominio(raiz->dir) && ehPretoDominio(raiz->esq))
        raiz = rotacionaParaEsquerdaDominio(raiz);
    if (ehVermelhoDominio(raiz->esq) && ehVermelhoDominio(raiz->esq->esq))
        raiz = rotacionaParaDireitaDominio(raiz);
    if (ehVermelhoDominio(raiz->esq) && ehVermelhoDominio(raiz->dir))
        sobeVermelhoDominio(raiz);

    return raiz;
}

/*
    Função que recebe um conjunto de informações sobre um usuário e insere ele 
    na árvore rubro-negra esquerdista dos usuários
*/
pUsuario inserirUsuario(pUsuario raiz, char enderecoIP[]){

    pUsuario novo;

    if (raiz == NULL) {
        novo = malloc(sizeof(struct usuario));
        novo->esq = novo->dir = NULL;
        novo->consultasRealizadas = 1;
        
        strcpy(novo->enderecoIP, enderecoIP);

        novo->cor = VERMELHO;
        return novo;
    }

    //Verifica se o IP é maior ou menor que a da raiz, para decidir onde colocar o novo nó
    if (strcmp(raiz->enderecoIP, enderecoIP) > 0)
        raiz->esq = inserirUsuario(raiz->esq, enderecoIP);
    else
        raiz->dir = inserirUsuario(raiz->dir, enderecoIP);

    //Corrige a árvore com base nas propriedades de uma árvore Rubro-Negra
    if (ehVermelhoUsuario(raiz->dir) && ehPretoUsuario(raiz->esq))
        raiz = rotacionaParaEsquerdaUsuario(raiz);
    if (ehVermelhoUsuario(raiz->esq) && ehVermelhoUsuario(raiz->esq->esq))
        raiz = rotacionaParaDireitaUsuario(raiz);
    if (ehVermelhoUsuario(raiz->esq) && ehVermelhoUsuario(raiz->dir))
        sobeVermelhoUsuario(raiz);

    return raiz;
}

/*
    Função que busca determinado domínio com base em seu nome, retornando o domínio 
    correspondende, ou NULL caso contrário
*/
pDominio buscaDominio(pDominio dominioAtual, char nomeDominio[]){

    if (dominioAtual == NULL || strcmp(dominioAtual->nome, nomeDominio) == 0)
        return dominioAtual;

    if (strcmp(dominioAtual->nome, nomeDominio) > 0)
        return buscaDominio(dominioAtual->esq, nomeDominio);
    else
        return buscaDominio(dominioAtual->dir, nomeDominio);
}

/*
    Função que busca determinado usuario com base em seu endereço IP, retornando o
    usuario correspondende, ou NULL caso contrário
*/
pUsuario buscarUsuario(pUsuario usuarioAtual, char enderecoIPUsuario[]){

    if (usuarioAtual == NULL || strcmp(usuarioAtual->enderecoIP, enderecoIPUsuario) == 0)
        return usuarioAtual;

    if (strcmp(usuarioAtual->enderecoIP, enderecoIPUsuario) > 0)
        return buscarUsuario(usuarioAtual->esq, enderecoIPUsuario);
    else
        return buscarUsuario(usuarioAtual->dir, enderecoIPUsuario);
}

// Função que libera memória usada para alocar os domínios na árvore
void liberarMemoriaDominios(pDominio dominio){

    if (dominio == NULL)
        return;

    liberarMemoriaDominios(dominio->esq);
    liberarMemoriaDominios(dominio->dir);

    free(dominio);
}
// Função que libera memória usada para alocar os usuários na árvore
void liberarMemoriaUsuarios(pUsuario usuario){

    if (usuario == NULL)
        return;

    liberarMemoriaUsuarios(usuario->esq);
    liberarMemoriaUsuarios(usuario->dir);

    free(usuario);
}

/*
    Função que realiza a consulta do domínio, com base no nome do domínio, 
    mostrando o resultado da busca.
*/
void realizarConsulta(pDominio raizDominios, char nomeDominio[], char enderecoIPUsuario[]){

    pDominio dominioEncontrado;

    dominioEncontrado = buscaDominio(raizDominios, nomeDominio);

    if(dominioEncontrado == NULL)
        printf("NOTFOUND %s FROM %s\n", nomeDominio, enderecoIPUsuario);
    else
        printf("ACCEPTED %s FROM %s RESPOND %s\n", nomeDominio, enderecoIPUsuario, dominioEncontrado->enderecoIP);
}

int main(){

    int maxConsultasPermitidas, numeroDominios, numeroConsultas;
    char nomeDominio[101], enderecoIPDominio[101], enderecoIPUsuario[101];
    pDominio raizDominios = NULL;
    pUsuario raizUsuarios = NULL;

    scanf("%d", &maxConsultasPermitidas);
    scanf("%d", &numeroDominios);

    //Lê todos os domínios a serem utilizados
    for(int i = 0; i < numeroDominios; i++){

        scanf("%s %s", nomeDominio, enderecoIPDominio);
        raizDominios = inserirDominio(raizDominios, nomeDominio, enderecoIPDominio);
    }

    scanf("%d", &numeroConsultas);

    //Realiza todas as consultas
    for(int i = 0; i < numeroConsultas; i++){

        scanf("%*s %s %*s %s", nomeDominio, enderecoIPUsuario);

        pUsuario usuario = buscarUsuario(raizUsuarios, enderecoIPUsuario);

        //Verifica se o usuário já existe
        if(usuario == NULL){

            raizUsuarios = inserirUsuario(raizUsuarios, enderecoIPUsuario);
            realizarConsulta(raizDominios, nomeDominio, enderecoIPUsuario);
        }
        else{

            //Verifica se o usuário ainda pode fazer consultas
            if(usuario->consultasRealizadas < maxConsultasPermitidas){

                realizarConsulta(raizDominios, nomeDominio, enderecoIPUsuario);
                usuario->consultasRealizadas++;
            }
            else
                printf("FORBIDDEN %s FROM %s\n", nomeDominio, enderecoIPUsuario);
        }
    }

    liberarMemoriaDominios(raizDominios);
    liberarMemoriaUsuarios(raizUsuarios);

    return 0;
}