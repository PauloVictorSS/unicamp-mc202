#include <stdio.h>
#include <string.h>
#define MAX 100


void ordenacaoQuickSort(char nomesAlunos[][MAX], int numeroTotalAlunos){
/*
    Algoritmo de ordenação QuickSort, que recebe uma
    'lista de Strings' e ordena ela
*/

    for(int i = 0; i < numeroTotalAlunos; i++){
        for(int j = 0; j < (numeroTotalAlunos - 1); j++){

            char nome[MAX];
            char nomeComparacao[MAX];

            strcpy(nome, nomesAlunos[j]);
            strcpy(nomeComparacao, nomesAlunos[j+1]);

            if (strcmp(nome, nomeComparacao) > 0){

                strcpy(nomesAlunos[j+1], nome);
                strcpy(nomesAlunos[j], nomeComparacao);
            }
        }
    }
}

int achaHomonimos(char nomesAlunos[][MAX], char saidaDeNomes[][MAX], int numeroTotalAlunos){

    int cont = 0, posicao = 0;

    //Primeiro 'for' para passar por todos os nomes
    for(int i = 0; i < numeroTotalAlunos; i++){

        posicao = 0;
        char nomeCompleto1[MAX], primeiroNome1[MAX];
        strcpy(nomeCompleto1, nomesAlunos[i]);

        //Pega o primeiro nome do nome completo
        while(nomeCompleto1[posicao] != '_'){
            primeiroNome1[posicao] = nomeCompleto1[posicao];
            posicao++;
        }
        primeiroNome1[posicao] = '\0';

        //Segundo 'for' para passar por todos os nomes
        for(int j = 0; j < numeroTotalAlunos; j++){
            if (i != j){
                posicao = 0;
                char nomeCompleto2[MAX], primeiroNome2[MAX];
                strcpy(nomeCompleto2, nomesAlunos[j]);

                //Pega o primeiro nome do nome completo
                while(nomeCompleto2[posicao] != '_'){
                    primeiroNome2[posicao] = nomeCompleto2[posicao];
                    posicao++;
                }
                primeiroNome2[posicao] = '\0';

                //Compara os primeiros nomes dos dois nomes completos
                if(strcmp(primeiroNome1, primeiroNome2) == 0){
                    strcpy(saidaDeNomes[cont], nomeCompleto1);
                    cont++;
                    break;
                }
            }
        }
    }

    return cont;
}

int achaParentes(char nomesAlunos[][MAX], char saidaDeNomes[][MAX], int numeroTotalAlunos){

    int cont = 0, posicao, indice;

    //Primeiro 'for' para passar por todos os nomes
    for(int i = 0; i < numeroTotalAlunos; i++){

        char nomeCompleto1[MAX], ultimoNome1[MAX];
        strcpy(nomeCompleto1, nomesAlunos[i]);

        posicao = strlen(nomeCompleto1) - 1;
        indice = 0;

        //Pega a posição do '_' no nome
        while(nomeCompleto1[posicao] != '_')
            posicao--;

        posicao++;

        //Separa o último nome do nome completo
        while(posicao < strlen(nomeCompleto1)){
            ultimoNome1[indice] = nomeCompleto1[posicao];
            posicao++;
            indice++;
        }
        ultimoNome1[indice] = '\0';

        //Segundo 'for' para passar por todos os nomes
        for(int j = 0; j < numeroTotalAlunos; j++){
            if (i != j){
                posicao = 0;
                char nomeCompleto2[MAX], ultimoNome2[MAX];
                strcpy(nomeCompleto2, nomesAlunos[j]);

                posicao = strlen(nomeCompleto2) - 1;
                indice = 0;

                //Pega a posição do '_' no nome
                while(nomeCompleto2[posicao] != '_')
                    posicao--;

                posicao++;

                //Separa o último nome do nome completo
                while(posicao < strlen(nomeCompleto2)){
                    ultimoNome2[indice] = nomeCompleto2[posicao];
                    posicao++;
                    indice++;
                }
                ultimoNome2[indice] = '\0';

                //Compara os primeiros nomes dos dois nomes completos
                if(strcmp(ultimoNome1, ultimoNome2) == 0){
                    strcpy(saidaDeNomes[cont], nomeCompleto1);
                    cont++;
                    break;
                }
            }
        }
    }

    return cont;
}

int main(){

    char nomesAlunos[MAX][MAX], saidaDeNomes[MAX][MAX], opcaoSelecionada[MAX];
    int numeroTotalAlunos, totalResultados;

    scanf("%d", &numeroTotalAlunos);
    scanf("%s", opcaoSelecionada);

    for (int cont = 0; cont < numeroTotalAlunos; cont++)
        scanf("%s", nomesAlunos[cont]);

    //Verifica qual opção foi selecionada
    if(strcmp(opcaoSelecionada, "homonimos") == 0){
        ordenacaoQuickSort(nomesAlunos, numeroTotalAlunos);
        totalResultados = achaHomonimos(nomesAlunos, saidaDeNomes, numeroTotalAlunos);
    }
    else
        totalResultados = achaParentes(nomesAlunos, saidaDeNomes, numeroTotalAlunos);

    //Imprime os resutlados
    for (int cont = 0; cont < totalResultados; cont++)
        printf("%s\n", saidaDeNomes[cont]);

    return 0;
}