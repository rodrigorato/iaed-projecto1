#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 41 /* Definido pelo enunciado - nome maximo de um banco */
#define MAXBANKS 1000 /* Definido pelo enunciado - maximo de bancos = 10^3 */
#define BOM 1 /* Para classificar bancos ditos bons */
#define MAU 0 /* Para classificar bancos ditos maus */
#define apanhaNEWLINE getchar() /* Para apanhar '\n's do utilizador de forma legivel */


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

/* Prototipos de funcoes */
void addBank(char nome[], int rating, int ref);
void killBank(int ref);
void reviveBank(int ref);

int indBankRef(int ref);



/* Programa - Funcoes */

int main(){
	/* 	Usado para invocar comandos	 *
	 * 	Variaveis sao mais do que  	 *
	 * necessarias por legibilidade. */

	int rating, ref1, ref2, valor, tipo; 
	char nome[MAXNAME], command;

	while(1){
		command = getchar();
		switch(command){
			case 'a':
				scanf(" %s %d %d", nome, &rating, &ref1);
				apanhaNEWLINE;
				addBank(nome, rating, ref1);
				break;

			case 'k':
				scanf(" %d", &ref1);
				apanhaNEWLINE;
				killBank(ref1);
				break;

			case 'r':
				scanf(" %d", &ref1);
				apanhaNEWLINE;
				reviveBank(ref1);
				break;

			case 'e':
				scanf(" %d %d %d", &ref1, &ref2, &valor);
				apanhaNEWLINE;
				printf("e:\nRef1: %d\nRef2: %d\nValor: %d\n", ref1, ref2, valor);
				break;

			case 'p':
				scanf(" %d %d %d", &ref1, &ref2, &valor);
				apanhaNEWLINE;
				printf("p:\nRef1: %d\nRef2: %d\nValor: %d\n", ref1, ref2, valor);
				break;

			case 'l':
				scanf(" %d", &tipo);
				apanhaNEWLINE;
				printf("l:\nTipo: %d\n", tipo);
				break;

			case 'K':
				apanhaNEWLINE;
				printf("K!\n");
				break;

			case 'x':
				apanhaNEWLINE;
				printf("eXit!\n");
				return EXIT_SUCCESS;

			default:
				printf("ERRO - Comando invalido!\n");
		}
	}
	return EXIT_FAILURE;
}

/* Funcoes do 'menu' */
void addBank(char nome[], int rating, int ref){
	/* Cria um novo banco, adiciona-o a lista		*
	 * de bancos (com os dados que recebe)			*
	 * e prepara as suas entradas na matriz.		*
	 * Altera o indice de bancos (adjacInd global)  */

	int j;
	bank newBank;

	strcpy(newBank.nome, nome);
	newBank.rating = rating;
	newBank.ref = ref;
	bankList[bankInd] = newBank;

	for(j = 0; j <= bankInd; j++){
		bankMat[bankInd][j] = 0;
		bankMat[j][bankInd] = 0;
	}

	bankInd++;
}

void killBank(int ref){
	/* Classifica como MAU o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = MAU;
}

void reviveBank(int ref){
	/* Classifica como BOM o banco cuja referencia recebe */
	int indice = indBankRef(ref);
	bankList[indice].rating = BOM;
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


/* PASTA ZONE








*/