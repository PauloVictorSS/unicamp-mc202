#include <stdio.h>
#include <stdlib.h>

typedef struct amigo *pAmigo;

struct amigo{
    int altura;
    pAmigo ant, prox;
};

/*
    Recebe a altura do amigo e adiciona ele no final da fila
    circular duplamente ligada
*/
pAmigo inserirAmigo(pAmigo inicio, int alturaAmigo){

    pAmigo novoAmigo;
    novoAmigo = malloc(sizeof(struct amigo));

    novoAmigo->altura = alturaAmigo;

    //Verifica se há algum elemento na fila
    if(inicio == NULL){
        novoAmigo->prox = novoAmigo;
        novoAmigo->ant = novoAmigo;
        inicio = novoAmigo;
    }
    else{
        novoAmigo->prox = inicio;
        novoAmigo->ant = inicio->ant;
        inicio->ant->prox = novoAmigo;
    }

    //O final da fila aponta para o novo amigo
    inicio->ant = novoAmigo;

    return inicio;
}

/*
    Função que recebe o início da circular fila duplamente ligada e, com base
    nos valores sorteados presente na lista recebida, coloca os amigos nas 
    listas das equipes 1 ou 2
*/
void defineEquipes(pAmigo inicio, int *equipe1, int *equipe2, int *valoresSorteados, int numeroAmigos){

    int numeroVezes, posicaoEquipe1, posicaoEquipe2;
    pAmigo amigoEscolhido, atual;

    posicaoEquipe1 = posicaoEquipe2 = 0;

    //Para cada número de dado sorteado
    for(int n = 0; n < numeroAmigos; n++){

        numeroVezes = valoresSorteados[n] - 1; 
        atual = inicio;

        //Verifica se vamos colocar na equipe 1 ou 2, alternando entre elas
        if((n % 2) == 0){

            //Percorre a fila, de trás para frente, 'numeroVezes' vezes
            for(int i = 0; i < numeroVezes; i++){
                atual = atual->ant;
            }

            amigoEscolhido = atual;

            equipe1[posicaoEquipe1] = amigoEscolhido->altura;
            posicaoEquipe1++;
        }
        else{

            //Percorre a fila do inicio ao fim 'numeroVezes' vezes
            for(int i = 0; i < numeroVezes; i++){
                atual = atual->prox;
            }
            
            amigoEscolhido = atual;

            equipe2[posicaoEquipe2] = amigoEscolhido->altura;
            posicaoEquipe2++;
        }

        //Prepara a saída do amigoEscolhido da fila
        amigoEscolhido->ant->prox = amigoEscolhido->prox;
        amigoEscolhido->prox->ant = amigoEscolhido->ant;

        //Verifica se o amigo escolhido era quem estava com o dado
        if(atual == inicio)
            inicio = inicio->prox;

        //Libera a memória onde o amigoEscolhido estava alocado
        free(amigoEscolhido);
    } 
}

/*
    Função que recebe uma lista com seu respectivo tamanho e mostra os 
    elementos dela
*/
void mostraLista(int *lista, int tamanho){

    for(int i = 0; i < tamanho ; i++)
        printf("%d ", lista[i]);
    printf("\n");
}

/*
    Função que utiliza o bubble sort para ordenar do menor para
    o maior, uma lista de inteiros
*/
void ordenacao(int *lista, int tamanho){

    for (int i = 1; i < tamanho; i++) {
        for (int j = 0; j < tamanho - 1; j++) {

            int valor = lista[j];
            int valorComp = lista[j + 1];

            if (valor > valorComp) {

                lista[j] = valorComp;
                lista[j + 1] = valor;
            }
        }
    }
}

int main(){

    int numeroAmigos, *equipe1, *equipe2, *valoresSorteados, alturaAmigo, tamanhoEquipe1, tamanhoEquipe2;
    pAmigo inicio = NULL;

    scanf("%d", &numeroAmigos);

    //Define o espaço que será alocado para cada equipe
    if((numeroAmigos % 2) == 0){
        tamanhoEquipe1 = tamanhoEquipe2 = numeroAmigos / 2;
    }
    else{
        tamanhoEquipe1 = (numeroAmigos + 1) / 2;
        tamanhoEquipe2 = tamanhoEquipe1 - 1;
    }

    //Aloca a memória para os vetores que guardarão as equipes
    equipe1 = malloc(tamanhoEquipe1 * sizeof(int));
    equipe2 = malloc(tamanhoEquipe2 * sizeof(int));

    valoresSorteados = malloc(numeroAmigos * sizeof(int));

    //Recebe as alturas do amigo para inserir na fila
    for(int i = 0; i < numeroAmigos; i++){
        scanf("%d", &alturaAmigo);
        inicio = inserirAmigo(inicio, alturaAmigo);
    }

    //Recebe os valores que foram sorteados
    for(int i = 0; i < numeroAmigos; i++){
        scanf("%d", &valoresSorteados[i]);
    }

    defineEquipes(inicio, equipe1, equipe2, valoresSorteados, numeroAmigos);

    ordenacao(equipe1, tamanhoEquipe1);
    ordenacao(equipe2, tamanhoEquipe2);
    mostraLista(equipe1, tamanhoEquipe1);
    mostraLista(equipe2, tamanhoEquipe2);

    //Libera a memória dos vetores utilizados
    free(equipe1);
    free(equipe2);
    free(valoresSorteados);

    return 0;
}