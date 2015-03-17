#include <stdio.h>

#define MAXNAME 41 /*	Um nome tem no maximo 40 letras *
					* 41 pois o char '\0' ocupa 1 pos.  */

#define MAXBANKS 1000 /* 10^3 */


typedef struct Banco{
	char nome[MAXNAME];
	unsigned int ref;
	short int rating;/*1-bom, 0-mau, qualquer outro valor-invalido*/
	unsigned int partners;
}bank;



void addBank();
void killBank();
void reviveBank();
void loanMoney();
void payback();
void list();
void killWorst();
void ending();

void initMatrix(unsigned int matriz[][MAXBANKS]);



int main(){
	bank banklist[MAXBANKS];
	unsigned int adjac[MAXBANKS][MAXBANKS];
	initMatrix(adjac);
	char c;
	while((c = getchar()) != 'x'){
		switch(c){
			case 'a':
				addBank();
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
