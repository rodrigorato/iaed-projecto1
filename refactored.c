#include <stdio.h>
#include <stdlib.h>

#define MAXNAME 41 /* Definido pelo enunciado - nome maximo de um banco */
#define MAXBANKS 1000 /* Definido pelo enunciado - maximo de bancos = 10^3 */
#define BOM 1 /* Para classificar bancos ditos bons */
#define MAU 0 /* Para classificar bancos ditos maus */

/* ESTRUTURA - Banco */
typedef struct Banco{
	char nome[MAXNAME];
	int ref;
	int rating; /* 1 para um rating BOM e 0 para MAU - qualquer outro e invalido */
	int partners;
}bank;

int main(){
	/* Usado ao longo do programa */
	char command; /* Regista o comando a efectuar */
	int adjacMat[MAXBANKS][MAXBANKS]; /* Matriz de adjacencias */
	int adjacInd = 0; /* Indice de bancos - conta quantos ha  */
	bank bankList[MAXBANKS];

	/* 	Usado para invocar comandos	 *
	 * 	Variaveis sao mais do que  	 *
	 * necessarias por legibilidade. */
	int rating, ref1, ref2, valor, tipo; 
	char nome[MAXNAME];

	while(1){
		command = getchar();
		switch(command){
			case 'a':
				scanf(" %s %d %d", nome, &rating, &ref1);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("a:\nNome: %s\nRating: %d\nReferencia: %d\n", nome, rating, ref1);
				break;

			case 'k':
				scanf(" %d", &ref1);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("k:\nReferencia: %d\n", ref1);
				break;

			case 'r':
				scanf(" %d", &ref1);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("r:\nReferencia: %d\n", ref1);
				break;

			case 'e':
				scanf(" %d %d %d", &ref1, &ref2, &valor);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("e:\nRef1: %d\nRef2: %d\nValor: %d\n", ref1, ref2, valor);
				break;

			case 'p':
				scanf(" %d %d %d", &ref1, &ref2, &valor);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("p:\nRef1: %d\nRef2: %d\nValor: %d\n", ref1, ref2, valor);
				break;

			case 'l':
				scanf(" %d", &tipo);
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("l:\nTipo: %d\n", tipo);
				break;

			case 'K':
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("K!\n");
				break;

			case 'x':
				getchar(); /* Apanha um '\n' que ficou por apanhar */
				printf("eXit!\n");
				return EXIT_SUCCESS;

			default:
				printf("ERRO - Comando invalido!\n");
		}
	}
	return EXIT_FAILURE;
}