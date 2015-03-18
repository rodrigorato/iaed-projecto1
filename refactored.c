#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 41 /* Definido pelo enunciado - nome maximo de um banco */
#define MAXBANKS 1000 /* Definido pelo enunciado - maximo de bancos = 10^3 */
#define BOM 1 /* Para classificar bancos ditos bons */
#define MAU 0 /* Para classificar bancos ditos maus */
#define EMPRESTA 0 /* Legibilidade de transferencias - comando 'e' */
#define DEVOLVE 1 /* Legibilidade de transferencias - comando 'p' */
#define OUTVM 0 /* calcValues - legibilidade */
#define printTODOS 1 /* calcValues - legibilidade */
#define apanhaNEWLINE getchar() /* Para apanhar '\n's do utilizador de forma legivel */


/* ESTRUTURA - Banco */
typedef struct Banco{
	char nome[MAXNAME];
	long ref;
	int rating; /* 1 para um rating BOM e 0 para MAU - qualquer outro e invalido */
	int partners;
}bank;

/* Dados dos bancos - a utilizar em todo o programa */
int bankMat[MAXBANKS][MAXBANKS]; /* Matriz de adjacencias */
int bankInd = 0; /* Indice de bancos - conta quantos ha  */
bank bankList[MAXBANKS];
int listaHistograma[MAXBANKS]; /* Vai sendo limpa a medida que sao adicionados bancos */

/* Prototipos de funcoes */
void addBank(char nome[], int rating, long ref);
void killBank(long ref);
void reviveBank(long ref);
void emprestaDinheiro(long ref1, long ref2, int valor);
void paybackDinheiro(long ref1, long ref2, int valor);
void listData(int tipo);
void killWorst();

int indBankRef(long ref);
void transfereDinheiro(long ref1, long ref2, int valor, int modo);
int calcValues(int indiceBanco, int op);
void histogramaParceiros();
void lastStats();
int weakestLink();

/* Programa - Funcoes */

int main(){
	/* 	Usado para invocar comandos	 *
	 * 	Variaveis sao mais do que  	 *
	 * necessarias por legibilidade. */

	int rating, valor, tipo; 
	long ref1, ref2;
	char nome[MAXNAME], command;

	while(1){
		command = getchar();
		switch(command){
			case 'a':
				scanf(" %s %d %ld", nome, &rating, &ref1);
				apanhaNEWLINE;
				addBank(nome, rating, ref1);
				break;

			case 'k':
				scanf(" %ld", &ref1);
				apanhaNEWLINE;
				killBank(ref1);
				break;

			case 'r':
				scanf(" %ld", &ref1);
				apanhaNEWLINE;
				reviveBank(ref1);
				break;

			case 'e':
				scanf(" %ld %ld %d", &ref1, &ref2, &valor);
				apanhaNEWLINE;
				emprestaDinheiro(ref1, ref2, valor);
				break;

			case 'p':
				scanf(" %ld %ld %d", &ref1, &ref2, &valor);
				apanhaNEWLINE;
				paybackDinheiro(ref1, ref2, valor);
				break;

			case 'l':
				scanf(" %d", &tipo);
				apanhaNEWLINE;
				listData(tipo);
				break;

			case 'K':
				apanhaNEWLINE;
				killWorst();
				break;

			case 'x':
				apanhaNEWLINE;
				lastStats();
				return EXIT_SUCCESS;

			default:
				printf("ERRO - Comando invalido!\n");
		}
	}
	return EXIT_FAILURE;
}

/* Funcoes do 'menu' */
void addBank(char nome[], int rating, long ref){
	/* Cria um novo banco, adiciona-o a lista		*
	 * de bancos (com os dados que recebe)			*
	 * e prepara as suas entradas na matriz.		*
	 * Altera o indice de bancos (adjacInd global)  */

	int j;
	bank newBank;

	if(bankInd != MAXBANKS){
		/* No caso contrario ja nao podemos guardar mais bancos */
		strcpy(newBank.nome, nome);
		newBank.rating = rating;
		newBank.ref = ref;
		newBank.partners = 0;
		listaHistograma[bankInd] = 0;
		listaHistograma[0]++;
		bankList[bankInd] = newBank;		

		for(j = 0; j <= bankInd; j++){
			bankMat[bankInd][j] = 0;
			bankMat[j][bankInd] = 0;
		}

		bankInd++;
	}
}

void killBank(long ref){
	/* Classifica como MAU o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = MAU;
}

void reviveBank(long ref){
	/* Classifica como BOM o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = BOM;
}

void emprestaDinheiro(long ref1, long ref2, int valor){
	/* Empresta dinheiro de valor de ref1 para ref2 */
	transfereDinheiro(ref1, ref2, valor, EMPRESTA);
}

void paybackDinheiro(long ref1, long ref2, int valor){
	/* Devolve dinheiro de valor de ref1 para ref2 */
	transfereDinheiro(ref1, ref2, valor, DEVOLVE);
}

void listData(int tipo){
	/* Escreve a listagem de informacao conforme o enunciado */

	int i;
	switch(tipo){
		case 0:
			for(i = 0; i < bankInd; i++)
				printf("%ld %s %d\n", bankList[i].ref, bankList[i].nome, bankList[i].rating);
			break;

		case 1:
			for(i = 0; i < bankInd; i++){
				printf("%ld %s %d ", bankList[i].ref, bankList[i].nome, bankList[i].rating);
				calcValues(i, printTODOS); /* Escreve estatisticas de dado banco */
				printf("\n");
			}
			break;

		case 2:
			histogramaParceiros(); /* Escreve o histograma dos parceiros entre bancos */
			break;
	}
}

void killWorst(){
	long refWeakest;
	int bankWorstInd;
	refWeakest = weakestLink();
	if(refWeakest != -1){
		killBank(refWeakest);
		bankWorstInd = indBankRef(refWeakest);
		printf("*%ld %s %d ", refWeakest, bankList[bankWorstInd].nome, bankList[bankWorstInd].rating);
		calcValues(bankWorstInd, printTODOS);
		printf("\n");
	}
	lastStats();
}


/* Funcoes 'auxiliares' */
int indBankRef(long ref){
	/* Devolve o indice de dado banco que recebe	*
	 * a referencia por argumento. Caso nao exista 	*
	 * devolve o inteiro '-1'.						*/

	int i;
	for(i = 0; i < bankInd; i++)
		if(bankList[i].ref == ref)
			return i;
	return -1;
}

void transfereDinheiro(long ref1, long ref2, int valor, int modo){
	/* Empresta ou devolve valor entre os bancos de ref1 e ref2: *
	 * (ref1 <modo> ref2 o valor) em que o modo pode ser:		 *
	 * <modo> ::= EMPRESTA ou DEVOLVE 							 *
	 * Altera a matriz de adjacencias (bankMat global) 			 */	

	 int ind1, ind2;
	 ind1 = indBankRef(ref1);
	 ind2 = indBankRef(ref2);

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

void histogramaParceiros(){
	int i;
	for(i = 0; i < bankInd; i++)
		if(listaHistograma[i] != 0)
			printf("%d %d\n", i, listaHistograma[i]);
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

	int tempDivida = 0, refFinal = -1, actDivida = -1, i;
	for(i = 0; i < bankInd; i++){
		actDivida = calcValues(i, OUTVM);
		if(bankList[i].rating == BOM && actDivida >= tempDivida && actDivida != 0){
			tempDivida = actDivida;
			refFinal = bankList[i].ref;
		}
	}
	return refFinal;
}
