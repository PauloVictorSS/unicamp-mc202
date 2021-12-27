#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct bloco{
    int tipo;
} Bloco;

int **calcularAltitudes(int m, int n, int seed){

    int **altitudes;

    //Aloca a matriz de altitudes
    altitudes = malloc(m * sizeof(int *));
    for(int i = 0; i < m; i++)
        altitudes[i] = malloc(n * sizeof(int));

    //Calcula a altitude máxima de cada coordenada
    for(int x = 0; x < m; x++)
        for(int z = 0; z < n; z++){

            int altitude = (seed * (202 + x + z) + 12345 + x + z) % 256;
            altitudes[x][z] = altitude;
        }

    return altitudes;
}

Bloco ***criarMundo(int m, int n, int **altitudes, int seed){

    Bloco ***mundo;

    //Aloca o vetor de matrizes de blocos para representar o mundo
    mundo = malloc(m * sizeof(Bloco **));
    for(int i = 0; i < m; i++)
        mundo[i] = malloc(n * sizeof(Bloco *));
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            mundo[i][j] = malloc(256 * sizeof(Bloco));

    //Determina o tipo de cada bloco
    for(int x = 0; x < m; x++){
        for(int z = 0; z < n; z++){
            for(int y = 0; y < 256; y++){

                int tipoBloco, altitudeDaCoordenadaXZ;

                altitudeDaCoordenadaXZ = altitudes[x][z];

                if(y > altitudeDaCoordenadaXZ)
                    tipoBloco = 21;
                else
                    tipoBloco = (seed * (202 + x + y + z) + x + y + z) % 33;

                mundo[x][z][y].tipo = tipoBloco;
            }
        }
    }

    return mundo;
}

double explorarMundo(Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco, int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos){

    double tempoTotal;
    *qtdDiamante = *qtdOuro = *qtdFerro = *qtdBlocos = 0;

    //Calcula a quantidade de cada tipo de bloco
    for(int x = 0; x < m; x++){
        for(int z = 0; z < n; z++){
            for(int y = 0; y < 256; y++){

                Bloco bloco = mundo[x][z][y];
                int tipoBloco = bloco.tipo;

                if(tipoBloco < 21){
                    (*qtdBlocos)++;

                    if(tipoBloco == 0)
                        (*qtdDiamante)++;
                    else if(tipoBloco < 3)
                        (*qtdOuro)++;
                    else if(tipoBloco < 6)
                        (*qtdFerro)++;
                }
            }
        }
    }

    tempoTotal = tempoPorBloco * (*qtdBlocos);
    return tempoTotal;
}

int main(){

    int m, n, seed, **altitudes;
    int qtdDiamante, qtdOuro, qtdFerro, qtdBlocos;
    double tempoPorBloco, tempoTotal;
    Bloco ***mundo;

    scanf("%d %d", &m, &n);
    scanf("%d", &seed);
    scanf("%lf", &tempoPorBloco);

    altitudes = calcularAltitudes(m, n, seed);
    mundo = criarMundo(m, n, altitudes, seed);
    tempoTotal = explorarMundo(mundo, m, n, altitudes, tempoPorBloco, &qtdDiamante, &qtdOuro, &qtdFerro, &qtdBlocos);

    printf("Total de Blocos: %d\n", qtdBlocos);
    printf("Tempo total: %.2lfs\n", tempoTotal);
    printf("Diamantes: %d\n", qtdDiamante);
    printf("Ouros: %d\n", qtdOuro);
    printf("Ferros: %d\n", qtdFerro);

    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            free(mundo[i][j]);
    for(int i = 0; i < m; i++)
            free(mundo[i]);
    free(mundo);

    for(int i = 0; i < m; i++)
        free(altitudes[i]);
    free(altitudes);

    return 0;
}