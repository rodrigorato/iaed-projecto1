
/*** MUDANÇAS ***/

vvoid ammountMoney(bank bankList, int adjacMat[][MAXBANKS], int bankInd, int maxInd, int status);


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
						);
			break;
		case 2:

			break;
	}

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
			/* outVM: valor total emprestado pelo banco a bancos 'maus' */
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

