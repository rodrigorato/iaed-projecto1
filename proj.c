#include <stdio.h>

#define MAXNAME 41 /*	Um nome tem no maximo 40 letras *
					* 41 pois o char '\0' ocupa 1 pos.  */

#define MAXBANKS 1000 /* 10^3 */


typedef struct Banco{
	char nome[MAXNAME];
	int ref;
	short int rating;/*1-bom, 0-mau, qualquer outro valor-invalido*/
	int partners;
}bank;



int addBank();
void killBank();
void reviveBank();
void loanMoney();
void payback();
void list();
void killWorst();
void ending();





int main(){	
	int adjacMat[MAXBANKS][MAXBANKS], adjacInd=0;
	bank bankList[MAXBANKS];
	char c;
	while((c = getchar()) != 'x'){
		switch(c){
			case 'a':
				addBank(bankList, adjacInd, adjacMat);
				break;
			case 'k':
				killBank();
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
				killWorst();
				break;
				//kill(weakest_link());
		}
	}
	ending();
	
	return 0;
}
int addBank(bank bankList, int adjacInd, int adjacMat[][MAXBANKS]){
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