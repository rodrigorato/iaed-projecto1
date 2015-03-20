#include <stdio.h>
#include <string.h>

#define MAXNAME 41 /* Definido pelo enunciado - nome maximo de um banco */
#define MAXBANKS 1000 /* Definido pelo enunciado - maximo de bancos = 10^3 */
#define BOM 1 /* Para classificar bancos ditos bons */
#define MAU 0 /* Para classificar bancos ditos maus */
#define EMPRESTA 0 /* Legibilidade de transferencias - comando 'e' */
#define DEVOLVE 1 /* Legibilidade de transferencias - comando 'p' */
#define OUTVM 0 /* calcValues - legibilidade */
#define printTODOS 1 /* calcValues - legibilidade */


/* ESTRUTURA - Banco */
typedef struct Banco{
	char nome[MAXNAME];
	int ref;
	int rating; /* 1 para um rating BOM e 0 para MAU - qualquer outro e invalido */
	int partners;
}bank;

/* Dados dos bancos - a utilizar em todo o programa */
int bankMat[MAXBANKS][MAXBANKS]; /* Matriz de adjacencias */
int bankInd = 0; /* Indice de bancos - conta quantos ha  */
bank bankList[MAXBANKS];
int listaHistograma[MAXBANKS]; /* Vai sendo limpa a medida que sao adicionados bancos */


/* Prototipos de funcoes */
void listData(int tipo);
void killWorst();

int indBankRef(int ref);
void transfereDinheiro(int ref1, int ref2, int valor, int modo);
int calcValues(int indiceBanco, int op);
void lastStats();
int weakestLink();

/* Programa - Funcoes */

int main(){

	int valor, tipo, temp, ref1, ref2;
	char command = getchar();


	while(command != 'x'){
		switch(command){
			case 'a':
				scanf("%s%d%d", bankList[bankInd].nome, &bankList[bankInd].rating, &bankList[bankInd].ref);
				bankList[bankInd].partners = 0;
				listaHistograma[bankInd] = 0;
				listaHistograma[0]++;
				bankInd++;
				break;

			case 'k':
				scanf("%d", &ref1);
				temp = indBankRef(ref1);
				if(temp != -1)
					bankList[temp].rating = MAU;
				break;

			case 'r':
				scanf("%d", &ref1);
				temp = indBankRef(ref1);
				if(temp != -1)
					bankList[temp].rating = BOM;
				break;

			case 'e':
				scanf("%d%d%d", &ref1, &ref2, &valor);
				transfereDinheiro(ref1, ref2, valor, EMPRESTA);
				break;

			case 'p':
				scanf("%d%d%d", &ref1, &ref2, &valor);
				transfereDinheiro(ref1, ref2, valor, DEVOLVE);
				break;

			case 'l':
				scanf("%d", &tipo);
				listData(tipo);
				break;

			case 'K':
				killWorst();
				break;

			default:
				printf("ERRO - Comando invalido!\n");
		}
		getchar(); /* gets the remaining '\n' */
		command = getchar();
	}

	lastStats();
	return 0;
}

/* Funcoes do 'menu' */



void listData(int tipo){
	/* Escreve a listagem de informacao conforme o enunciado */

	int i;
	switch(tipo){
		case 0:
			for(i = 0; i < bankInd; i++)
				printf("%d %s %d\n", bankList[i].ref, bankList[i].nome, bankList[i].rating);
			break;

		case 1:
			for(i = 0; i < bankInd; i++){
				printf("%d %s %d ", bankList[i].ref, bankList[i].nome, bankList[i].rating);
				calcValues(i, printTODOS); /* Escreve estatisticas de dado banco */
				printf("\n");
			}
			break;

		case 2:
			for(i = 0; i < bankInd; i++)
				if(listaHistograma[i] != 0)
					printf("%d %d\n", i, listaHistograma[i]);
			break;
	}
}

void killWorst(){
	int indWeakest = weakestLink();
	if(indWeakest != -1){
		bankList[indWeakest].rating = MAU;
		printf("*%d %s %d ", bankList[indWeakest].ref, bankList[indWeakest].nome, bankList[indWeakest].rating);
		calcValues(indWeakest, printTODOS);
		printf("\n");
	}
	lastStats();
}


/* Funcoes 'auxiliares' */
int indBankRef(int ref){
	/* Devolve o indice de dado banco que recebe	*
	 * a referencia por argumento. Caso nao exista 	*
	 * devolve o inteiro '-1'.						*/

	int i;
	for(i = 0; i < bankInd; i++)
		if(bankList[i].ref == ref)
			return i;
	return -1;
}

void transfereDinheiro(int ref1, int ref2, int valor, int modo){
	/* Empresta ou devolve valor entre os bancos de ref1 e ref2: *
	 * (ref1 <modo> ref2 o valor) em que o modo pode ser:		 *
	 * <modo> ::= EMPRESTA ou DEVOLVE 							 *
	 * Altera a matriz de adjacencias (bankMat global) 			 */	

	 int ind1, ind2;
	 ind1 = indBankRef(ref1);
	 ind2 = indBankRef(ref2);

	 if(valor > 0 && ind1 != -1 && ind2 != -1){
	 	if(modo == EMPRESTA){
	 		if(bankMat[ind1][ind2] == 0 && bankMat[ind2][ind1] == 0){
	 			listaHistograma[bankList[ind1].partners]--;
	 			listaHistograma[bankList[ind2].partners]--;

	 			bankList[ind1].partners++;
	 			bankList[ind2].partners++;

	 			listaHistograma[bankList[ind1].partners]++;
	 			listaHistograma[bankList[ind2].partners]++;
		 	}

	 		bankMat[ind2][ind1] += valor;
		 }
		 else{
	 		/* modo == DEVOLVE */
	 		bankMat[ind1][ind2] -= valor;
	 		if (bankMat[ind2][ind1] == 0 && bankMat[ind1][ind2] == 0){
	 			listaHistograma[bankList[ind1].partners]--;
	 			listaHistograma[bankList[ind2].partners]--;

	 			bankList[ind1].partners--;
	 			bankList[ind2].partners--;

	 			listaHistograma[bankList[ind1].partners]++;
	 			listaHistograma[bankList[ind2].partners]++;
			}
		}
	}
}

int calcValues(int indiceBanco, int op){
	/* Calcula estatisticas de dado banco, explicado em comentarios posteriores */
	int i, inP = 0, outP = 0, outV = 0, outVM = 0, inV = 0, inVM = 0;
	switch(op){
		case OUTVM:
			/* OUTVM: usado pelo comando 'K' - valor total emprestado pelo banco a bancos 'maus' */
			for(i=0; i<bankInd; i++){
				if(bankList[i].rating == MAU)
					outVM += bankMat[i][indiceBanco];
			}
			break;

		case printTODOS:
			/* printTODOS - calculando ao mesmo tempo para diminuir complexidade */
			for(i = 0; i < bankInd; i++){
				if(bankMat[indiceBanco][i] != 0)
					inP++;
				
				if(bankMat[i][indiceBanco] != 0)
					outP++;
				
				outV += bankMat[i][indiceBanco];

				if(bankList[i].rating == MAU)
					outVM += bankMat[i][indiceBanco];

				inV += bankMat[indiceBanco][i];

				if(bankList[i].rating == MAU)
					inVM += bankMat[indiceBanco][i];
			}

			printf("%d %d %d %d %d %d", inP, outP, outV, outVM, inV, inVM);
			break;
			
	}
	return outVM;
}

void lastStats(){
	int i, bons = 0;
	for(i = 0; i < bankInd; i++)
		if(bankList[i].rating == BOM)
			bons++;
	printf("%d %d\n", bankInd, bons);
}

int weakestLink(){
	/* Devolve a referencia do pior banco *
	 *          -1 caso nao o haja	      */

	int tempDivida = 0, indFinal = -1, actDivida = -1, i;
	for(i = 0; i < bankInd; i++){
		actDivida = calcValues(i, OUTVM);
		if(bankList[i].rating == BOM && actDivida >= tempDivida && actDivida != 0){
			tempDivida = actDivida;
			indFinal = i;
		}
	}
	return indFinal;
}
