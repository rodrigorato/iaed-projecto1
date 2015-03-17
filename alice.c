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
void loanMoney(bank bankList);
void payback(bank bankList);
void list(bank bankList);
void killWorst();
void ending();

void changeRating(bank list, int referencia, int newRating);
int weakestLink(bank bankList);
int indBankRef(bank bankList, referencia);
void transactions(bank bankList, way);





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
				list(bankList);
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

void changeRating(bank list, int referencia, int newRating){
	/* */
	int i;
	list[indBankRef(bankList, referencia)].rating = newRating;
}

int indBankRef(bank bankList, referencia){
	int i;
	for(i=0; BankList[i].ref != referencia; i++);
	return i;
}

void transactions(bank bankList, way){/*way=0 -> loan, way=1 ->payback*/
	int ref1,ref2, money;
	scanf(" %d %d %d", &ref1, &ref2, &money);
	i1=indBankRef(bankList, ref1);
	i2=indBankRef(bankList, ref2);
	if 	(way==0){
		if (adjacMat[i2][i1]==0 && adjacMat[i1][i2]==0 ){
			bankList[i1].partners++;
			bankList[i2].partners++;
		}
		adjacMat[i2][i1] += money;		
	}
	else{
		adjacMat[i1][i2]-=money;
		if (adjacMat[i2][i1]==0 && adjacMat[i1][i2]==0 ){
			bankList[i1].partners--;
			bankList[i2].partners--;
		}	
	}
}


int addBank(bank bankList, int adjacInd, int adjacMat[][MAXBANKS]){
	/* */
	int j;
	bank newBank;
	newBank.partners=0;

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

void loanMoney(bank bankList){
	transactions(bankList,0);
}

void payback(bank bankList){
	transactions(bankList,1);
}
