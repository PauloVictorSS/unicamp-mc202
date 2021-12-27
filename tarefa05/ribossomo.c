#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int verificaPosicaoEmVetor(char elemento, char vetor[]){

    for(int posicao = 0; posicao < 13; posicao++)
        if(vetor[posicao] == elemento)
            return posicao;

    return 0;
}


void calculaRNATranscrito(char basesDNA[], char basesRNA[], char sequenciaDNA[], char rnaTranscrito[],  int tamanhoBaseDNA, int listaQuantidadeTranscricoes[], int listaIDs[]){

    for(int i = 0; i < tamanhoBaseDNA; i++){

        char elemento = sequenciaDNA[i];

        int posicao = verificaPosicaoEmVetor(elemento, basesDNA);
        rnaTranscrito[i] = basesRNA[posicao];

        listaIDs[i] = listaQuantidadeTranscricoes[posicao];
        listaQuantidadeTranscricoes[posicao]++;
    }
}

void mostraDiagramaDeTranscricao(char sequenciaDNA[], char rnaTranscrito[],  int tamanhoBaseDNA, int listaIDs[]){

    printf("ID:");
    for(int i = 0; i < tamanhoBaseDNA; i++){

        printf("\t%d", listaIDs[i]);
    }

    printf("\nDNA:");
    for(int i = 0; i < tamanhoBaseDNA; i++){

        printf("\t%c", sequenciaDNA[i]);
    }

    printf("\n");
    for(int i = 0; i < tamanhoBaseDNA; i++){

        printf("\t|");
    }

    printf("\nRNA:");
    for(int i = 0; i < tamanhoBaseDNA; i++){

        printf("\t%c", rnaTranscrito[i]);
    }
    printf("\n");
}


int main(){

    char basesDNA[14], basesRNA[14], *sequenciaDNA, *rnaTranscrito;
    int tamanhoBaseDNA, *listaQuantidadeTranscricoes, *listaIDs;

    scanf("%s", basesDNA);
    scanf("%s", basesRNA);
    scanf("%d", &tamanhoBaseDNA);

    sequenciaDNA = malloc((tamanhoBaseDNA + 1) * sizeof(char));
    rnaTranscrito = malloc((tamanhoBaseDNA + 1) * sizeof(char));
    listaQuantidadeTranscricoes = malloc(tamanhoBaseDNA * sizeof(int));
    listaIDs = malloc(tamanhoBaseDNA * sizeof(int));

    if(sequenciaDNA == NULL || rnaTranscrito == NULL){
        printf("Não há memória suficiente\n");
        exit(1);
    }

    scanf("%s", sequenciaDNA);
    for(int i = 0; i < tamanhoBaseDNA; i++){

        listaQuantidadeTranscricoes[i] = 0;
    }

    calculaRNATranscrito(basesDNA, basesRNA, sequenciaDNA, rnaTranscrito, tamanhoBaseDNA, listaQuantidadeTranscricoes, listaIDs);
    mostraDiagramaDeTranscricao(sequenciaDNA, rnaTranscrito, tamanhoBaseDNA, listaIDs);

    free(sequenciaDNA);
    free(rnaTranscrito);
    free(listaQuantidadeTranscricoes);
    free(listaIDs);

    return 0;
}