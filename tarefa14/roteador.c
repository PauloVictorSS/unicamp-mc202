#include <stdio.h>
#include <stdlib.h>

#define PAI(i) ((i-1)/2)
#define F_ESQ(i) (2*i+1) /*Filho esquerdo de i*/
#define F_DIR(i) (2*i+2) /*Filho direito de i*/

typedef struct {
    int id, prioridade, incremento;
} Pacote;

typedef struct {
    Pacote *v;
    int n, tamanhoMaximo;
} FP;

typedef FP * pFP;

/*
    Função que recebe um tamanho e cria uma filha de prioridades com
    um vetor desse tamanho
*/
pFP criarFilaPrio(int tam){

    pFP fPrio = malloc(sizeof(FP));
    fPrio->v = malloc(tam * sizeof(Pacote));
    fPrio->n = 0;
    fPrio->tamanhoMaximo = tam;

    return fPrio;
}

/*
    Função que recebe dois pacotes e troca eles de posição na memória
*/
void troca(Pacote *a, Pacote *b) {
    Pacote t = *a;
    *a = *b;
    *b = t;
}

/*
    Função que recebe a fila de prioridades e determinada posição de um elemento
    para 'subir' esse elemento na sua posição correta
*/
void sobeNoHeap(pFP fPrio, int k) {
    if (k > 0 && fPrio->v[PAI(k)].prioridade < fPrio->v[k].prioridade) {
        troca(&fPrio->v[k], &fPrio->v[PAI(k)]);
        sobeNoHeap(fPrio, PAI(k));
    }
}

/*
    Função que recebe a fila de prioridades e determinada posição de um elemento
    para 'descer' esse elemento na sua posição correta
*/
void desceNoHeap(pFP fPrio, int k) {
    int maior_filho;

    if (F_ESQ(k) < fPrio->n) {

        maior_filho = F_ESQ(k);
        if (F_DIR(k) < fPrio->n &&
            fPrio->v[F_ESQ(k)].prioridade < fPrio->v[F_DIR(k)].prioridade)
            maior_filho = F_DIR(k);
        if (fPrio->v[k].prioridade < fPrio->v[maior_filho].prioridade) {
            troca(&fPrio->v[k], &fPrio->v[maior_filho]);
            desceNoHeap(fPrio, maior_filho);
        }
    }
}

/*
    Função que recebe a fila de prioridades e pacote para inserí-lo
    dentro da fila na posição correta
*/
void inserePacote(pFP fPrio, Pacote pacote) {
    fPrio->v[fPrio->n] = pacote;
    fPrio->n++;
    sobeNoHeap(fPrio, fPrio->n - 1);
}

/*
    Função que recebe a fila de prioridades e retorna o valor máximo dentro
    dessa fila
*/
Pacote extraiMaximo(pFP fPrio) {

    Pacote pacote = fPrio->v[0];
    troca(&fPrio->v[0], &fPrio->v[fPrio->n - 1]);
    fPrio->n--;
    desceNoHeap(fPrio, 0);

    return pacote;
}

/*
    Função que recebe a fila de prioridades e incrementa a prioridade
    de todos os pacotes contidos nela
*/
void incrementaPrioridades(pFP fPrio){

    for(int i = (fPrio->n - 1); i >= 0; i--){

        fPrio->v[i].prioridade += fPrio->v[i].incremento;
    }
    
    for(int i = (fPrio->n - 1); i >= 0; i--){

        sobeNoHeap(fPrio, i);
    }
}

/*
    Função que recebe a fila de prioridades, o número do tick e a quantidade 
    máxima de pacotes enviados. Ele mostra os pacotes daquele determinado tick
    retirando as informações da fila recebida.
*/
void mostraTick(pFP fPrio, int pacotesEnviadosTick, int tick){

    int cont = 0;

    printf("TICK %d\n", tick);

    while((cont < pacotesEnviadosTick) && fPrio->n > 0){

        Pacote pacoteMaximo = extraiMaximo(fPrio);

        printf("%d\t%d\t%d\n", pacoteMaximo.id, pacoteMaximo.prioridade, pacoteMaximo.incremento);
        cont++;
    }

    incrementaPrioridades(fPrio);
}


int main(){

    int pacotesEnviadosTick, capacidadeFila, tick = 1;
    int id, prioridade, incremento; 
    pFP fPrio;

    scanf("%d", &pacotesEnviadosTick);
    scanf("%d", &capacidadeFila);

    fPrio = criarFilaPrio(capacidadeFila);

    while (scanf("%d %d %d", &id, &prioridade, &incremento) != EOF) {

        if((id != 0) || (prioridade != 0) || (incremento != 0)){

            if(fPrio->n < fPrio->tamanhoMaximo){
                Pacote novoPacote;

                novoPacote.id = id;
                novoPacote.prioridade = prioridade;
                novoPacote.incremento = incremento;

                inserePacote(fPrio, novoPacote);
            }
        }
        else{
            mostraTick(fPrio, pacotesEnviadosTick, tick);
            printf("\n");
            tick++;
        }
    }

    free(fPrio->v);
    free(fPrio);

    return 0;
}