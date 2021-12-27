#include <stdio.h>

int descobrePosicao(int fila[], int numParticip, int num){

    int posicao;

    for(int cont = 0; cont < numParticip; cont++){

        if(fila[cont] == num){
            posicao = cont;
            break;
        }
    }

    return posicao;
}


void rearranjar(int fila[], int numParticip, int instrucao, int num1, int num2){

    int posicao1, posicao2;
    posicao1 = descobrePosicao(fila, numParticip, num1);
    posicao2 = descobrePosicao(fila, numParticip, num2);

    if(instrucao == 1){

        int tamanhoListaAuxiliar, cont2;
        tamanhoListaAuxiliar = posicao2 - posicao1 + 1;
        cont2 = 1;

        int listaAuxiliar[10000];

        for(int cont1 = posicao1; cont1 <= posicao2; cont1++){
            listaAuxiliar[tamanhoListaAuxiliar - cont2] = fila[cont1];
            cont2++;
        }

        cont2 = 0;

        for(int cont1 = posicao1; cont1 <= posicao2; cont1++){
            fila[cont1] = listaAuxiliar[cont2];
            cont2++;
        }

    }
    else{

        for(int cont = posicao1; cont < posicao2; cont++){
            fila[cont] = fila[cont + 1];
        }

        fila[posicao2] = num1;
    }
}

int main(){

    int numParticip;
    scanf("%d", &numParticip);
    int fila[10000];

    for(int cont = 0; cont < numParticip; cont++){
        fila[cont] = cont + 1;
    }

    int numInstrucao;
    int instrucao, num1, num2;
    scanf("%d", &numInstrucao);

    for(int cont = 0; cont < numInstrucao; cont++){

        scanf("%d %d %d", &instrucao, &num1, &num2);
        rearranjar(fila, numParticip, instrucao, num1, num2);
    }

    for(int cont = 0; cont < numParticip; cont++){
        printf("%d ", fila[cont]);
    }
    printf("\n");

    return 0;
}