#include <stdio.h>
#define MAX 10

double calculaImposotoDoMes(double rendimentoDoMes){

    if(rendimentoDoMes < 1499.15)
        return 0;

    else if(rendimentoDoMes < 2246.75)
        return (rendimentoDoMes - 1499.15) * 0.075;

    else if(rendimentoDoMes < 2995.70)
        return 56.07 + (rendimentoDoMes - 2246.75) * 0.15;

    else if(rendimentoDoMes < 3743.19)
        return 56.07 + 112.3425 + (rendimentoDoMes- 2995.70) * 0.225;

    else 
        return 56.07 + 112.3425 + 168.18525 + (rendimentoDoMes - 3743.19) * 0.275;
}

double calculaImposotoDevidoAnual(double baseCalculo){

    if(baseCalculo < 18000.00)
        return 0;

    else if(baseCalculo < 26400.00)
        return (baseCalculo - 18000.00) * 0.075;

    else if(baseCalculo < 36000.00)
        return 630.00 + (baseCalculo - 26400.00) * 0.15;

    else if(baseCalculo < 44400.00)
        return 630.00 + 1440.00 + (baseCalculo- 36000.00) * 0.225;

    else 
        return 630.00 + 1440.00 + 1890.00 + (baseCalculo - 44400.00) * 0.275;
}


void calculaImpostosRetidos(int numeroFuncio, double rendimentosFuncio[][12], double abatimentosFuncio[], double infosFuncionarios[][15]){

    for(int i = 0; i < numeroFuncio; i++){

        double totalImpostoRetidoFonte = 0, rendimentoTotalAnual = 0, impostoDevidoAnual, valorAjuste;

        for(int j = 0; j < 12; j++){
        
            double rendimentoDoMes = rendimentosFuncio[i][j];
            double impostoDoMes;
            
            impostoDoMes = calculaImposotoDoMes(rendimentoDoMes);

            infosFuncionarios[i][j] = impostoDoMes;
            totalImpostoRetidoFonte += impostoDoMes;
            rendimentoTotalAnual += rendimentoDoMes;
        }

        impostoDevidoAnual = calculaImposotoDevidoAnual(rendimentoTotalAnual - abatimentosFuncio[i]);
        valorAjuste = impostoDevidoAnual - totalImpostoRetidoFonte;

        infosFuncionarios[i][12] = totalImpostoRetidoFonte;
        infosFuncionarios[i][13] = impostoDevidoAnual;
        infosFuncionarios[i][14] = valorAjuste;
    }
}


int main(){

    int numeroFuncio;
    double rendimentosFuncio[MAX][12];
    double abatimentosFuncio[MAX];
    double infosFuncionarios[MAX][15];

    scanf("%d", &numeroFuncio);

    for(int i = 0; i < numeroFuncio; i++)
        scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &rendimentosFuncio[i][0], &rendimentosFuncio[i][1], &rendimentosFuncio[i][2], &rendimentosFuncio[i][3], &rendimentosFuncio[i][4], &rendimentosFuncio[i][5], &rendimentosFuncio[i][6], &rendimentosFuncio[i][7], &rendimentosFuncio[i][8], &rendimentosFuncio[i][9], &rendimentosFuncio[i][10], &rendimentosFuncio[i][11]);

    for(int i = 0; i < numeroFuncio; i++)
        scanf("%lf", &abatimentosFuncio[i]);
    
    calculaImpostosRetidos(numeroFuncio, rendimentosFuncio, abatimentosFuncio, infosFuncionarios);

    printf("Jan\tFev\tMar\tAbr\tMai\tJun\tJul\tAgo\tSet\tOut\tNov\tDez\tRetido\tDevido\tAjuste\n");

    for(int i = 0; i < numeroFuncio; i++){
        for(int j = 0; j < 15; j++)
            printf("%.2lf\t", infosFuncionarios[i][j]);
        printf("\n");
    }

    return 0;
}