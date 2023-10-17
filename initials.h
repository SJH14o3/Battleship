/*this function will set both player board arrays to a meaningful number, read comments down below for more information.
also if mode = 1 it mean it is a fresh game, and it has not loaded from a file, else it mean it is a loaded game from file.
mode is require because in fresh game it will reset every part of player boards to 0.*/
void initialBoard(int mode, int FOCP1[][11], int FOCP2[][11], int size) {
	int i, j; //counters.
	//this loop will give the first row of board arrays their index. it will be used for printing.
	for (j = 0; j < size; j++) FOCP1[0][j] = FOCP2[0][j] = j;
	for (i = 1; i < size; i++) FOCP1[i][0] = FOCP2[i][0] = i; //same as the last loop but for columns.
	//if condition is met, this loop will make other parts of arrays zero which is water.
	if (mode == 1) for (i = 1; i < size; i++) for (j = 1; j < size; j++)  FOCP1[i][j] = FOCP2[i][j] = 0;
}

//will make f1ships and f2ships arrays -1 before start of the game, it is required for game. also get an initial amount for some other variables.
void initialShips(int playerShips[][101], int ho[], int remainShips[]) {
	for (int i = 0; i < 102; i++) playerShips[0][i] = playerShips[1][i] = -1;
	ho[0] = ho[1] = 1;
	remainShips[0] = remainShips[1] = 0;
}

/*during file input mode, bots won't obey it, because the content of player map 2 is loaded,
since at the end of the text file there is the repair amounts, bot map must be reset.*/
void initialBotFileMode(int size, int FOCP2[][11], int remainShips[], int playerShips[][101], int ho[]) {
	int i, j;
	for (i = 1; i < size; i++) for (j = 1; j < size; j++) FOCP2[i][j] = 0;
	remainShips[1] = 0;
	ho[1] = 1;
	for (i = 1; i < 101; i++) playerShips[1][i] = -1;
}

//during a fresh game, two functions need to be called. this function do it.
void initiateGame(int FOCP1[][11], int FOCP2[][11], int playerShips[][101], int ho[], int remainShips[], int size) {
	initialBoard(1, FOCP1, FOCP2, size);
	initialShips(playerShips, ho, remainShips);
}