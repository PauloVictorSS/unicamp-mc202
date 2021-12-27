#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pessoa *pPessoa;
typedef struct fila *pFila;

struct pessoa{
    char nome[25];
    double valor;
    pPessoa recrutador, recrutandoEsq, recrutandoDir;
};

struct fila{
    pPessoa pessoa;
    pFila prox;
};

/*
    Função que busca dentro da árvore quem é o recrutador, com base na
    raiz da aŕvore recebida e no nome do recrutador 
*/
pPessoa buscaPessoaRecrutadora(pPessoa pessoaTopo, char nomeRecrutador[]){

    pPessoa pessoaProcurada;

    //Verifica se chegamos ao final da árvore ou se achamos o recrutador
    if (pessoaTopo == NULL || (strcmp(pessoaTopo->nome, nomeRecrutador) == 0))
        return pessoaTopo;

    //Busca o recrutador no filho esquerdo da árvore
    pessoaProcurada = buscaPessoaRecrutadora(pessoaTopo->recrutandoEsq, nomeRecrutador);

    //Verifica se encontramos o recrutador
    if (pessoaProcurada != NULL)
        return pessoaProcurada;
        
    //Busca o recrutador no filho direito da árvore
    return buscaPessoaRecrutadora(pessoaTopo->recrutandoDir, nomeRecrutador);
}

/*
    Função que recebe a pessoa que recrutou a nova pessoa, o nome dessa 
    nova pessoa e seu investimento inicial. Retorna um ponteiro que 
    aponta para essa nova pessoa dentro da árvore.
*/
pPessoa criaNovoRecrutando(pPessoa pessoaRecrutadora, char nomeRecrutando[], double investimentoInicial){

    pPessoa novoRecrutando = malloc(sizeof(struct pessoa));

    //Seta os valores desse novo recrutando
    strcpy(novoRecrutando->nome, nomeRecrutando);
    novoRecrutando->valor = investimentoInicial;
    novoRecrutando->recrutador = pessoaRecrutadora;
    novoRecrutando->recrutandoEsq = NULL;
    novoRecrutando->recrutandoDir = NULL;

    return novoRecrutando;
}

/*
    Função que recebe o recrutador e o novo recrutando para
    atualizar os valores monetários de cada um, com base na
    bonificação
*/
void atualizaFinancasRecrutadorRecrutando(pPessoa pessoaRecrutadora, pPessoa novoRecrutando){

    double bonificacao = novoRecrutando->valor / 10;

    novoRecrutando->valor += bonificacao;
    pessoaRecrutadora->valor -= bonificacao;
}

/*
    Função recursiva que recebe a pessoa que acabou de ser recrutada e uma
    taxa de pagamento. Ela atualiza os valores dos recrutadores em até
    6 níveis acima do recrutando recebido.
*/
void remuneracaoParaRecrutadores(pPessoa recrutando, double taxa){

    pPessoa pessoaRecrutadora, recrutandoEsq, recrutandoDir;
    double remuneracaoEsquerda, remuneracaoDireita, remuneracao;

    pessoaRecrutadora = recrutando->recrutador;

    //Verifica se não tem mais recrutador ou se chegamos nos seis níveis
    if(pessoaRecrutadora == NULL || taxa == 0)
        return;    

    //Veifica se não é a primeira vez que a função é chamada
    if(taxa != 0.06){

        //A remuneração vai vir somente do recrutando que recebeu remuneração
        remuneracao = recrutando->valor * taxa;

        //Atualizando os valores
        pessoaRecrutadora->valor += remuneracao;
        recrutando->valor -= remuneracao;
    }
    else{

        //Achando quem são os recrutando do recrutador
        recrutandoEsq = pessoaRecrutadora->recrutandoEsq;
        recrutandoDir = pessoaRecrutadora->recrutandoDir;

        //Calculando quando cada recrutando vai pagar
        remuneracaoEsquerda = recrutandoEsq->valor * taxa;
        remuneracaoDireita = recrutandoDir->valor * taxa;

        //A remuneração vai vir tanto do recrutando esquerdo como do direito (Já que um deles acabou de chegar)
        pessoaRecrutadora->valor += remuneracaoEsquerda + remuneracaoDireita;
        recrutandoEsq->valor -= remuneracaoEsquerda;
        recrutandoDir->valor -= remuneracaoDireita;
    }

    //Chama a função novamente, passando o recrutador e diminuindo a taxa
    remuneracaoParaRecrutadores(pessoaRecrutadora, (taxa - 0.01));
}

/*
    Funçaõ que recebe a fila e retorna a última posição dela
*/
pFila buscaUltimoDaFila(pFila fila){

    pFila atual;

    for(atual = fila; atual->prox != NULL; atual = atual->prox);

    return atual;
}

/*
    Função que recebe a fila e determiada pessoa da piramide
    e retorna a nova fila com a pessoa inserida na ultima 
    posição
*/
pFila enfileirar(pFila fila, pPessoa pessoa){

    pFila novaPessoa = malloc(sizeof(struct fila));

    //Seta os dados da pessoa
    novaPessoa->pessoa = pessoa;
    novaPessoa->prox = NULL;

    //Verifica se a fila é vazia
    if(fila != NULL){
        pFila ultimoDaFila = buscaUltimoDaFila(fila);
        ultimoDaFila->prox = novaPessoa;
    }
    else
        fila = novaPessoa;
    
    return fila;
}

/*
    Função que recebe determinada pessoa da pirâmide e retorna o 
    nível onde ela estava dentro da pirâmide
*/
int descobreNivelDaPiramide(pPessoa pessoa){

    if(pessoa == NULL)
        return 0;

    return descobreNivelDaPiramide(pessoa->recrutador) + 1;
}

/*
    Função que recebe a raiz da pirâmide e uma flag dizendo se 
    é a primeira vez que é chamada ou não. Mostra a situação
    dela, passando pelos níveis dela, utilizando uma fila
*/
void mostraAtualEstadoDaPiramide(pPessoa pessoaTopo, int primeiraVez){

    int ultimoNivel = 0;
    pFila fila;
    fila = NULL;
    fila = enfileirar(fila, pessoaTopo);

    //Enquanto houver valores na fila
    while(fila != NULL) {

        pFila atualFila = fila;
        pessoaTopo = atualFila->pessoa;
        fila = fila->prox;

        //Verifica se a pessoa que vamos mostrar os dados existe
        if (pessoaTopo != NULL) {

            //Enfileira os filhos da atual pessoa
            fila = enfileirar(fila, pessoaTopo->recrutandoEsq);
            fila = enfileirar(fila, pessoaTopo->recrutandoDir);

            //Descobre em qual nivel a atual pessoa está na pirâmide
            int nivel = descobreNivelDaPiramide(pessoaTopo);

            //Verifica se estamos no mesmo nível que a última vez
            if(nivel != ultimoNivel){

                if(primeiraVez)
                    printf("\n");

                printf("Nivel %d: [%s %.2lf]",nivel, pessoaTopo->nome, pessoaTopo->valor); 
                ultimoNivel = nivel;
            }
            else
                printf(" [%s %.2lf]", pessoaTopo->nome, pessoaTopo->valor); 
        }

        //Libera o espaço da fila, que ja foi utilizado  
        free(atualFila);
    }
    
    printf("\n");
}

/*
    Função que gerencia o esquema da pirâmide, recebe a raiz 
    da pirâmide e adiciona as pessoas lidas nela, atualizando
    o valor monetário de cada uma e, quando necessário, pagando
    a remuneração para os recrutadores
*/
void gerenciarEsquemaPiramide(pPessoa raiz){

    double investimentoInicial;
    char nomeRecrutador[25], nomeRecrutando[25];

    //Enquanto houver pessoas para serem adicionadas
    while (scanf("%s recruta %s %lf", nomeRecrutador, nomeRecrutando, &investimentoInicial) != EOF) {

        //Acha a pessoa que recrutou essa nova pessoa
        pPessoa pessoaRecrutadora = buscaPessoaRecrutadora(raiz, nomeRecrutador);
        pPessoa novoRecrutando = criaNovoRecrutando(pessoaRecrutadora, nomeRecrutando, investimentoInicial);

        atualizaFinancasRecrutadorRecrutando(pessoaRecrutadora, novoRecrutando);

        //Verifica se a nova pessoa vai para a esquerda ou para a direita do recrutador
        if(pessoaRecrutadora->recrutandoEsq == NULL){
            pessoaRecrutadora->recrutandoEsq = novoRecrutando;
        }
        else{
            pessoaRecrutadora->recrutandoDir = novoRecrutando;

            //O recrutador acabou de recrutar duas pessoas
            remuneracaoParaRecrutadores(novoRecrutando, 0.06);
        }

        //Mostra o estado atual da pirâmide
        mostraAtualEstadoDaPiramide(raiz, 1);
    }
}

/*
    Função que libera memória utilizada para alocar as pessoas
    qu estavam na pirâmide
*/
void liberaMemoria(pPessoa pessoa){

    if (pessoa == NULL)
        return;

    liberaMemoria(pessoa->recrutandoEsq);
    liberaMemoria(pessoa->recrutandoDir);

    free(pessoa);
}

int main(){

    double investimentoInicial;
    char nomeRecrutando[25];

    scanf("Topo %s %lf", nomeRecrutando, &investimentoInicial);
    pPessoa pessoaTopo = criaNovoRecrutando(NULL, nomeRecrutando, investimentoInicial);
    mostraAtualEstadoDaPiramide(pessoaTopo, 0);

    gerenciarEsquemaPiramide(pessoaTopo);
    liberaMemoria(pessoaTopo);

    return 0;
}