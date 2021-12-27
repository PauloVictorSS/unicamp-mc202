#include <stdio.h>
#include <string.h>
#include "planejar.h"

#define MAX 100

int main(){

    char comando[MAX], codigoOrigem[3];
    Data dataInicio, dataFim;
    int qtdVoos = 0;
    Voo voos[MAX];

    scanf("%s", comando);

    while(strcmp(comando, "planejar") != 0){

        if(strcmp(comando, "registrar") == 0){
            Voo novoVoo;

            scanf("%d", &novoVoo.numero);
            scanf("%s %s", novoVoo.aeoropOrigem, novoVoo.aeoropDestino);
            novoVoo.dataVoo = lerData();
            scanf("%f", &novoVoo.valor);

            voos[qtdVoos] = novoVoo;
            qtdVoos++;
        }
        else if(strcmp(comando, "alterar") == 0){
            int numeroVoo;
            float novoValor;

            scanf("%d", &numeroVoo);
            scanf("%f", &novoValor);
            alterar(voos, qtdVoos, numeroVoo, novoValor);
        }
        else if(strcmp(comando, "cancelar") == 0){
            int numeroVoo;

            scanf("%d", &numeroVoo);
            cancelar(voos, qtdVoos, numeroVoo);
        }

        scanf("%s", comando);
    }

    scanf("%s", codigoOrigem);
    dataInicio = lerData();
    dataFim = lerData();
    planejar(voos, qtdVoos, dataInicio, dataFim, codigoOrigem);

    return 0;
}