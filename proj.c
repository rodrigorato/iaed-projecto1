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
void list(bank bankList, int adjacMat[][MAXBANKS], int adjacInd);
void killWorst();
void ending(bank bankList, int adjacInd);

void changeRating(bank list, int referencia, int newRating);
int weakestLink(bank bankList);
int indBankRef(bank bankList, referencia);
void transactions(bank bankList, way);
void ammountMoney(bank bankList, int adjacMat[][MAXBANKS], int bankInd, int maxInd, int status);
void histPartners(bank bankList, int maxInd);





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
				reviveBank(bankList);
				break;
			case 'e':
				loanMoney(bankList);
				break;
			case 'p':
				payback(bankList);
				break;
			case 'l':
				list(bankList);
				break;
			case 'K':
				/*getchar();*/
				killWorst(bankList);
				break;
				//kill(weakest_link());
		}
	}
	ending(bankList, adjacInd);	
	return 0;
}

void changeRating(bank list, int referencia, int newRating){
	/* */
	int i;
	list[indBankRef(bankList, referencia)].rating = newRating;
}

int indBankRef(bank bankList, referencia){
	int i;
	for(i=0; bankList[i].ref != referencia; i++);
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


void list(bank bankList, int adjacMat[][MAXBANKS], int adjacInd){
	int i;
	char c = getchar();
	switch(c){
		case 0:
			for(i=0; i < adjacInd; i++)
				printf("%i %s %i\n", bankList[i].ref, bankList[i].nome, bankList[i].rating);
			break;
		case 1:
			for(i=0; i < adjacInd; i++)
				printf("%d %s %d %d %d %d %d %d %d\n",
						bankList[i].ref, bankList[i].nome, bankList[i].rating,
						ammountMoney(bankList, adjacMat, i, adjacInd, 0),
						ammountMoney(bankList, adjacMat, i, adjacInd, 1),
						ammountMoney(bankList, adjacMat, i, adjacInd, 2),
						ammountMoney(bankList, adjacMat, i, adjacInd, 3),
						ammountMoney(bankList, adjacMat, i, adjacInd, 4),
						ammountMoney(bankList, adjacMat, i, adjacInd, 5));
			break;
		case 2:
			histPartners(bankList, maxInd);
			break;
	}
}

void histPartners(bank bankList, int maxInd){
	int i, histList[maxInd] = {};
	for(i=0; i<maxInd; i++)
		histList[bankList[i].partners]++;
	for(i=0; i<maxInd; i++)
		if(histList[i] != 0)
			printf("%d %d", i, histList[i]);
}



void ammountMoney(bank bankList, int adjacMat[][MAXBANKS], int bankInd, int maxInd, int status){
	int result = 0, i = 0;
	switch(status){
		case 0:
			/* inP: numero total de bancos parceiros a quem o banco tem divida */
			for(i=0; i<maxInd; i++){
				if(adjacMat[bankInd][i] != 0)
					result++;
			}
			break;
		case 1:
			/* outP: numero total de bancos parceiros a quem o banco tem dinheiro emprestado */
			for(i=0; i<maxInd; i++){
				if(adjacMat[i][bankInd] != 0)
					result++;
			}
			break;
		case 2:
			/* outV: valor total emprestado pelo banco a outros */
			for(i=0; i<maxInd; i++)
				result += adjacMat[i][bankInd];
			break;
		case 3:
			/* outVM: usado pelo comando 'K' - valor total emprestado pelo banco a bancos 'maus' */
			for(i=0; i<maxInd; i++){
				if(bankList[i].rating == 0)
					result += adjacMat[i][bankInd];
			}
			break;
		case 4:
			/* inV: valor total emprestado ao banco por outros bancos */
			for(i=0; i<maxInd; i++)
				result += adjacMat[bankInd][i];
			break;
		case 5:
			/* inVM: valor total emprestado ao banco por bancos maus */
			for(i=0; i<maxInd; i++){
				if(bankList[i].rating == 0)
					result += adjacMat[bankInd][i];
			}
			break;
	}
	return result;
}




void ending(bank bankList, int adjacInd){
	int i, bons=0;
	for (i=0;i<adjacInd;i++)
		if (bankList[i].rating==1)
			bons++;
	printf("%d %d", adjacInd, bons);
}