#include <stdio.h>

#define MAXNAME 41 /*	Um nome tem no maximo 40 letras *
					* 41 pois o char '\0' ocupa 1 pos.  */

#define MAXBANKS 1000 /* 10^3 */


/* ESTRUTURAS */
typedef struct Banco{
	char nome[MAXNAME];
	int ref;
	short int rating;/*1-bom, 0-mau, qualquer outro valor-invalido*/
	int partners;
}bank;

/* PROTOTIPOS */
int addBank(bank bankList, int adjacInd, int adjacMat[][MAXBANKS]);
void killBank(bank bankList);
void reviveBank(bank bankList);
void loanMoney();
void payback();
void list();
void killWorst();
void ending();

void changeRating(bank list, int referencia, int newRating);
int weakestLink(bank bankList);





int main(){	
	int adjacMat[MAXBANKS][MAXBANKS], adjacInd=0;
	bank bankList[MAXBANKS];
	char c;
	while((c = getchar()) != 'x'){
		switch(c){
			case 'a':
				adjacInd = addBank(bankList, adjacInd, adjacMat);
				break;
			case 'k':
				killBank(bankList);
				break;
			case 'r':
				reviveBank();
				break;
			case 'e':
				loanMoney();
				break;
			case 'p':
				payback();
				break;
			case 'l':
				list();
				break;
			case 'K':
				getchar();
				killWorst(bankList);
				break;
				//kill(weakest_link());
		}
	}
	ending();
	
	return 0;
}

void kill(bank list, int referencia){
	/* */
	int i;
	for (i=0; list[i].ref != referencia; i++);
	list[i].rating = 0;
}



int addBank(bank bankList, int adjacInd, int adjacMat[][MAXBANKS]){
	/* */
	int j;
	bank newBank;

	scanf(" %s %d %hd", newBank.nome, &newBank.ref, &newBank.rating);
	bankList[adjacInd] = newBank;

	for(j=0;j<=adjacInd;j++){
		adjacMat[adjacInd][j]=0;
		adjacMat[j][adjacInd]=0;
	}

	return (++adjacInd);
}

void killBank(bank bankList){
	int ref;
	scanf(" %d", &ref);
	changeRating(bankList, ref, 0);
}

/*
void killWorst(bank bankList){
	cenas
}*/

void reviveBank(bank bankList){
	int ref;
	scanf(" %d", &ref);
	changeRating(bankList, ref, 1);
}