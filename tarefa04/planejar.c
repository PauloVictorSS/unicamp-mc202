#include <stdio.h>
#include <string.h>
#include "planejar.h"

Data lerData(){
    Data data;

    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);

    return data;
}

int comparaData(Data data1, Data data2){

    if(data1.ano > data2.ano)
        return 2;
    else{
        if(data1.ano < data2.ano)
            return 1;

        //Os anos são iguais
        else{
            if(data1.mes > data2.mes)
                return 2;
            else{
                if(data1.mes < data2.mes)
                    return 1;
                
                //Os mêses são iguais
                else{
                    if(data1.dia < data2.dia)
                        return 1;
                    else if(data1.dia > data2.dia)
                        return 2;
                    else
                        return 0;
                }
            }
        }
    }
}

int diferencaData(Data data1, Data data2){

    Data inicio, fim;
    int menorData, diferencaDias = 0;
    int difAnos = 0;
    int iTotalDiasAteInicioAno, fTotalDiasAteInicioAno;

    menorData = comparaData(data1, data2);

    //Total de dias que cada mês tem
    int totalDiasPorMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //Verifica qual data é a de inicio e qual é a de fim
    if(menorData == 1){
        inicio = data1;
        fim = data2;
    }
    else if(menorData == 2){
        inicio = data2;
        fim = data1;
    }
    else
        return diferencaDias;

    //Calcula a quantidade de dias que há entre a data até o inicio do ano
    iTotalDiasAteInicioAno = inicio.dia;
    for (int i = inicio.mes - 1; i > 0; --i)
        iTotalDiasAteInicioAno += totalDiasPorMes[i];

    //Calcula a quantidade de dias que há entre a data até o inicio do ano
    fTotalDiasAteInicioAno = fim.dia;
	for (int i = fim.mes - 1; i > 0; --i)
		fTotalDiasAteInicioAno += totalDiasPorMes[i];

    //Verifica se há diferença entre os anos das datas
    if(inicio.ano < fim.ano)
        difAnos = 365;

    //Calcula a diferença de dias entre as datas
    diferencaDias = fTotalDiasAteInicioAno - iTotalDiasAteInicioAno + difAnos;
    return diferencaDias;
}

void cancelar(Voo voos[], int qtdVoos, int numeroVoo){

    for(int cont = 0; cont < qtdVoos; cont++)
        if(voos[cont].numero == numeroVoo)
            voos[cont].numero = -1;
}

void alterar(Voo voos[], int qtdVoos, int numeroVoo, float novoValor){
    
    for(int cont = 0; cont < qtdVoos; cont++)
        if(voos[cont].numero == numeroVoo)
            voos[cont].valor = novoValor;
}

float verificaViabilidadeDeParVoos(Voo voo1, Voo voo2){

    Data dataVoo1 = voo1.dataVoo; 
    Data dataVoo2 = voo2.dataVoo;
    float valor1 = voo1.valor; 
    float valor2 = voo2.valor;  
    char aeoropOrigem1[4], aeoropDestino1[4];
    char aeoropOrigem2[4], aeoropDestino2[4];

    strcpy(aeoropOrigem1, voo1.aeoropOrigem);
    strcpy(aeoropDestino1, voo1.aeoropDestino);
    strcpy(aeoropOrigem2, voo2.aeoropOrigem);
    strcpy(aeoropDestino2, voo2.aeoropDestino);

    int diferenca = diferencaData(dataVoo1, dataVoo2);

    if(diferenca >= 3)
        if((strcmp(aeoropDestino1, aeoropOrigem2) == 0) && (strcmp(aeoropDestino2, aeoropOrigem1) == 0))
            return valor1 + valor2;
    
    return -1.0;
}

void planejar(Voo voos[], int qtdVoos, Data dataInicio, Data dataFim, char codigoOrigem[]){

    int qtdVoosPossiveis = 0;
    Voo voosPossiveis[100];
    float valorMinimo = 300000.0;
    int parDeVooEscolhido[2];

    for(int i = 0; i < qtdVoos; i++){

        Voo voo = voos[i];
        int numero = voo.numero;
        
        if(numero >= 0){

            Data dataVoo = voo.dataVoo;

            if(((comparaData(dataInicio, dataVoo) == 1) || (comparaData(dataInicio, dataVoo) == 0)) && ((comparaData(dataFim, dataVoo) == 2) || (comparaData(dataFim, dataVoo) == 0))){
                voosPossiveis[qtdVoosPossiveis] = voo;
                qtdVoosPossiveis++;
            }
        }
    }

    for(int i = 0; i < qtdVoosPossiveis; i++){

        Voo voo1 = voosPossiveis[i];

        for(int j = 0; j < qtdVoosPossiveis; j++){
            
            if(i != j){

                Voo voo2 = voosPossiveis[j];
                float resultado = verificaViabilidadeDeParVoos(voo1, voo2);

                if((resultado >= 0) && (resultado < valorMinimo)){
                    valorMinimo = resultado;

                    if(comparaData(voo1.dataVoo, voo2.dataVoo) == 1){
                        parDeVooEscolhido[0] = voo1.numero;
                        parDeVooEscolhido[1] = voo2.numero;
                    }
                    else{
                        parDeVooEscolhido[0] = voo2.numero;
                        parDeVooEscolhido[1] = voo1.numero;
                    }
                }
            }
        }
    }

    printf("%d\n", parDeVooEscolhido[0]);
    printf("%d\n", parDeVooEscolhido[1]);
}