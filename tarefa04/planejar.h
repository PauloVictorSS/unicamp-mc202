typedef struct data {
    int dia, mes, ano;
} Data;

typedef struct voo {
    int numero;
    char aeoropOrigem[4], aeoropDestino[4];
    Data dataVoo;
    float valor;
} Voo;

Data lerData();

/*
    A função recebe duas datas e retorna:
    -> 1; Caso a primeira data seja a menor;
    -> 2; Caso a segunda data seja a menor;
    -> 0; Caso as datas sejam iguais;
*/
int comparaData(Data data1, Data data2);

/*
    A função recebe duas datas e returna
    a diferença em dias entre elas
*/
int diferencaData(Data data1, Data data2);

/*
    A função recebe os voos, o número total de
    voos e deleta da lista o voo que tiver o número
    'numeroVoo', recebido por parâmetro
*/
void cancelar(Voo voos[], int qtdVoos, int numeroVoo);

/*
    A função recebe os voos, o número total de
    voos e altera o valor do voo para 'novoValor', 
    recebido por parâmetro, que tiver o número
    'numeroVoo', também recebido por parâmetro
*/
void alterar(Voo voos[], int qtdVoos, int numeroVoo, float novoValor);

float verificaViabilidadeDeParVoos(Voo voo1, Voo voo2);

void planejar(Voo voos[], int qtdVoos, Data dataInicio, Data dataFim, char codigoOrigem[]);