/*		Turno 9 - Grupo 9	*
 *							*
 *	Alice Dourado  81205	*
 *  Nuno Vieira	   81098	*
 * 	Rodrigo Rato   81500	*
 *							*/

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
int bankInd = 0; /* Indice de bancos - conta quantos ha */
bank bankList[MAXBANKS]; /* Vector de bancos */
int listaHistograma[MAXBANKS]; /* Vai sendo limpa a medida que sao adicionados bancos */

/* Prototipos de funcoes */
void addBank(char nome[], int rating, int ref);
int killBank(int ref);
void reviveBank(int ref);
void emprestaDinheiro(int ref1, int ref2, int valor);
void paybackDinheiro(int ref1, int ref2, int valor);
void listData(int tipo);
void killWorst();

int indBankRef(int ref);
void transfereDinheiro(int ref1, int ref2, int valor, int modo);
int calcValues(int indiceBanco, int op);
void lastStats();
int weakestLink();

/* Programa - Funcoes */
int main(){

	int rating, valor, tipo, ref1, ref2;
	char nome[MAXNAME], command = getchar();

	while(command != 'x'){
		switch(command){
			case 'a':
				scanf("%s %d %d", nome, &rating, &ref1);
				addBank(nome, rating, ref1);
				break;

			case 'k':
				scanf("%d", &ref1);
				killBank(ref1);
				break;

			case 'r':
				scanf("%d", &ref1);
				reviveBank(ref1);
				break;

			case 'e':
				scanf("%d %d %d", &ref1, &ref2, &valor);
				emprestaDinheiro(ref1, ref2, valor);
				break;

			case 'p':
				scanf("%d %d %d", &ref1, &ref2, &valor);
				paybackDinheiro(ref1, ref2, valor);
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
		getchar(); /* '\n' por apanhar */
		command = getchar();
	}

	lastStats();
	return 0;
}


/* Funcoes do 'menu' */
void addBank(char nome[], int rating, int ref){
	/* Adiciona novo banco a lista de bancos, incrementa  	  *
	 * o numero de bancos que temos e actualiza o histograma  *
	 * de parceiros.										  */

	if(bankInd != MAXBANKS && ref > 0){
		/* No caso contrario ja nao podemos guardar mais bancos */
		strcpy(bankList[bankInd].nome, nome);
		bankList[bankInd].rating = rating;
		bankList[bankInd].ref = ref;
		bankList[bankInd].partners = 0;

		listaHistograma[0]++;	

		bankInd++;
	}
}

int killBank(int ref){
	/* Classifica como MAU o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = MAU;
	return indice;
}

void reviveBank(int ref){
	/* Classifica como BOM o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = BOM;
}

void emprestaDinheiro(int ref1, int ref2, int valor){
	/* Empresta dinheiro de valor de ref1 para ref2 */
	transfereDinheiro(ref1, ref2, valor, EMPRESTA);
}

void paybackDinheiro(int ref1, int ref2, int valor){
	/* Devolve dinheiro de valor de ref1 para ref2 */
	transfereDinheiro(ref1, ref2, valor, DEVOLVE);
}

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
	/* Despromove/mata o banco que verifique as condicoes	  *
	 * ditas pelo enunciado. Caso nao exista nao faz nada.	  *
	 * De qualquer modo, imprime a estatistica da totalidade  *
	 * de bancos e quantos sao bons 						  */

	int refWeakest, bankWorstInd;
	refWeakest = weakestLink();
	if(refWeakest != -1){
		bankWorstInd = killBank(refWeakest);
		printf("*%d %s %d ", refWeakest, bankList[bankWorstInd].nome, bankList[bankWorstInd].rating);
		calcValues(bankWorstInd, printTODOS);
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
	 		if(!bankMat[ind1][ind2] && !bankMat[ind2][ind1]){
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
	 		if (!bankMat[ind2][ind1] && !bankMat[ind1][ind2]){
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
			for(i=0; i < bankInd; i++){
				if(bankList[i].rating == MAU)
					outVM += bankMat[i][indiceBanco];
			}
			break;

		case printTODOS:
			/* printTODOS - calculando ao mesmo tempo para diminuir complexidade */
			for(i = 0; i < bankInd; i++){
				if(bankList[i].rating == MAU){
					outVM += bankMat[i][indiceBanco];
					inVM += bankMat[indiceBanco][i];
				}

				if(bankMat[indiceBanco][i] != 0)
					inP++;
				
				if(bankMat[i][indiceBanco] != 0)
					outP++;
				
				outV += bankMat[i][indiceBanco];

				inV += bankMat[indiceBanco][i];

			}

			printf("%d %d %d %d %d %d", inP, outP, outV, outVM, inV, inVM);
			break;
	}
	return outVM;
}

void lastStats(){
	/* Imprime a estatistica da totalidade de bancos e quantos sao bons */
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
		if(bankList[i].rating == BOM){
			actDivida = calcValues(i, OUTVM);
			if(actDivida >= tempDivida && actDivida){
				tempDivida = actDivida;
				refFinal = bankList[i].ref;
			}
		}
	}
	return refFinal;
}