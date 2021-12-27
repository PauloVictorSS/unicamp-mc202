#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    Recebe uma matriz, junto com a posição de determinado elemento,
    e retorna true caso tenha um elemento repetido na mesma linha
    ou coluna que esse elemento. Caso contrário, retorna false.
*/
bool temNumeroRepetidoLinhaColuna(int **quebraCabeca, int i, int j, int max){

    //Verifica se não tem um número igual na mesma linha
    for(int c = 1; c < max; c++){
        if(c != j){
            if(quebraCabeca[i][j] == quebraCabeca[i][c])
                return true;
        }
    }

    //Verifica se não tem um número igual na mesma coluna
    for(int l = 1; l < max; l++){
        if(l != i){
            if(quebraCabeca[i][j] == quebraCabeca[l][j])
                return true;
        }
    }

    return false;
}

/*
    Função que recebe o quebra-cabeça e a posição de determinado número,
    dessa forma, determina se as quatro pistas estão 'vendo' o número
    certo de edifícios. Caso estejam, retorna true, caso contrário,
    retorna false.
*/
bool pistasVeemNumeroCorretoEdificios(int **quebraCabeca, int i, int j, int max){

    int pistaEsquerda = quebraCabeca[i][0];
    int pistaCima = quebraCabeca[0][j];
    int pistaDireita = quebraCabeca[i][max];
    int pistaBaixo = quebraCabeca[max][j];
    
    int maiorEdificil, estaVendo, cont;
    int totalElementos = max - 1;


    //Verifica se a pista da ESQUERDA 'vê' o número correto de edifícios
    maiorEdificil = estaVendo = cont = 0;
    for(int c = 1; c < max; c++){
        if(quebraCabeca[i][c] != 0){
            cont++;

            if(quebraCabeca[i][c] > maiorEdificil){
                maiorEdificil = quebraCabeca[i][c];
                estaVendo++;
            }
        }
    }
    if(((estaVendo > pistaEsquerda) && (cont < totalElementos)) || 
        ((cont == totalElementos) && (estaVendo != pistaEsquerda)))
        return false;


    //Verifica se a pista de CIMA 'vê' o número correto de edifícios
    maiorEdificil = estaVendo = cont = 0;
    for(int l = 1; l < max; l++){
        if(quebraCabeca[l][j] != 0){
            cont++;

            if(quebraCabeca[l][j] > maiorEdificil){
                maiorEdificil = quebraCabeca[l][j];
                estaVendo++;
            }
        }
    }
    if(((estaVendo > pistaCima) && (cont < totalElementos)) || 
        ((cont == totalElementos) && (estaVendo != pistaCima)))
        return false;

    //Verifica se estamos no final de determinada linha
    if(j == (max - 1)){

        //Verifica se a pista da DIREITA 'vê' o número correto de edifícios
        maiorEdificil = estaVendo = cont = 0;
        for(int c = (max - 1); c > 0; c--){
            if(quebraCabeca[i][c] != 0){
                cont++;

                if(quebraCabeca[i][c] > maiorEdificil){
                    maiorEdificil = quebraCabeca[i][c];
                    estaVendo++;
                }
            }
        }
        if(estaVendo != pistaDireita)
            return false;
    }

    //Verifica se estamos no final de determinada coluna
    if(i == (max - 1)){

        //Verifica se a pista de BAIXO 'vê' o número correto de edifícios
        maiorEdificil = estaVendo = cont = 0;
        for(int l = (max - 1); l > 0; l--){
            if(quebraCabeca[l][j] != 0){
                cont++;

                if(quebraCabeca[l][j] > maiorEdificil){
                    maiorEdificil = quebraCabeca[l][j];
                    estaVendo++;
                }
            }
        }
        if(estaVendo != pistaBaixo)
            return false;
    }

    return true;
}


/*
    Função recursiva que utiliza uma estratégia de backtranking para 
    achar a solução para o 'quebra-cabeça' passado. A função tenta 
    diversas possibilidades e só retorna true quando a possibilidade
    deu certo, caso contrário, retorna false.
*/
bool resolveQuebraCabeca(int tamanhoGrade, int tamanhoQuebraCabeca, int **quebraCabeca, int i, int j){

    bool resultado;

    if(i > tamanhoGrade)
        return true;

    //Verifica se algum dos números de 'p' até 'tamanhoGrade' valem na posição [i][j]
    for(int p = 1; p <= tamanhoGrade ; p++){

        quebraCabeca[i][j] = p;

        //Conjunto de verificações para ver se o elemento 'p' é válido no quebra-cabeça ou não
        if(!temNumeroRepetidoLinhaColuna(quebraCabeca, i, j, (tamanhoQuebraCabeca - 1))){
            if(pistasVeemNumeroCorretoEdificios(quebraCabeca, i, j, (tamanhoQuebraCabeca - 1))){

                //Verifica se chegamos no final da linha ou não
                if(j == tamanhoGrade)
                    resultado = resolveQuebraCabeca(tamanhoGrade, tamanhoQuebraCabeca, quebraCabeca, i + 1, 1);
                else
                    resultado = resolveQuebraCabeca(tamanhoGrade, tamanhoQuebraCabeca, quebraCabeca, i, j + 1);

                if(resultado)
                    return resultado;
            }
        }
    }
    quebraCabeca[i][j] = 0;

    return false;
}


int main(){

    int tamanhoGrade, tamanhoQuebraCabeca, **quebraCabeca;

    scanf("%d", &tamanhoGrade);
    tamanhoQuebraCabeca = tamanhoGrade + 2;

    //Aloca memória para a matriz do quebra-cabeça
    quebraCabeca = malloc(tamanhoQuebraCabeca * sizeof(int *));
    for(int i = 0; i < tamanhoQuebraCabeca; i++)
        quebraCabeca[i] = malloc(tamanhoQuebraCabeca * sizeof(int *));
    
    //Lê os valores da matriz do quebra-cabeça
    for(int i = 0; i < tamanhoQuebraCabeca; i++)
        for(int j = 0; j < tamanhoQuebraCabeca; j++)
            scanf("%d", &quebraCabeca[i][j]);

    resolveQuebraCabeca(tamanhoGrade, tamanhoQuebraCabeca, quebraCabeca, 1, 1);

    //Imprime a resolução do quebra-cabeça
    for(int i = 1; i < (tamanhoQuebraCabeca - 1); i++){
        for(int j = 1; j < (tamanhoQuebraCabeca - 1); j++)
            printf("%d ", quebraCabeca[i][j]);
        printf("\n");
    }

    //Libera a memória alocada da matriz
    for(int i = 0; i < tamanhoQuebraCabeca; i++)
        free(quebraCabeca[i]);
    free(quebraCabeca);

    return 0;
}