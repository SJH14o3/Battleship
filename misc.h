//some functions declaration.
void saveRepairLoc(int row, int col);
void drawIntoGameText(char p1[], char p2[]);
void drawRepairError(int mode);
void repairPart(int FOCP[][11], int pNUM, char name[], int mode, int playerShips[][101], int size, int side, ALLEGRO_DISPLAY *display);
int showExitDialog(ALLEGRO_DISPLAY *display);
/*in manual mode, during placing a ship in map, if player place a ship which has interfrence with another ship, this error will be shown.
it is here since one function in this header use it.*/
void printErrorInterfrence() {
	printRed();
	printf("\aThis ship has Interfrence with another ship, try again\n");
	al_rest(1.5);
}

//since both scanf and gets are used, this function will prevent gets from getting enter after an scanf function.
void getEnter() {
	char enter[2];
	gets_s(enter, 2);
}

//this function is like a coinflip, 50% chance to return 1, 50% chance to return 2;
int Randomizer() {
	int i;
	i = rand();
	i %= 2;
	if (i) return(2);
	else return(1);
}

/*this function will check for interference of the given ship with other ships. it will return -1 if there is.
if mode = 1: print error, else won't print error. since for setting bot ships also this function is called.*/
int checkForInterference(int len, int wid, int col, int row, int FOCP[][11], int mode) {
	int i, j;
	for (i = row; i < wid + row; i++) {
		for (j = col; j < len + col; j++) {
			if (FOCP[i][j] > 0) {
				if (mode) printErrorInterfrence();
				return(0);
			}
		}
	}
	return(1);
}

//this function will give the amount of a ship parts to "if alive" mechanism.
void markMechanism(int pNum, int len, int wid, int playerShips[][101], int ho[]) {
	playerShips[pNum - 1][ho[pNum - 1]] = (len * wid);
	ho[pNum - 1]++;
}

//this function will set a ship in a location that has been given.
void setShipLoc(int FOCP[][11], int pNum, int len, int wid, int row, int col, int playerShips[][101], int ho[]) {
	int i, j;
	for (i = col; i < col + wid; i++) {
		for (j = row; j < row + len; j++) {
			FOCP[i][j] = ho[pNum - 1];
		}
	}
	markMechanism(pNum, len, wid, playerShips, ho);
}

/*this function will scan three details for ships, if width is bigger than lenght it will swap them,
because it has been programmed in a way that lenght must be bigger, it won't matter for vertical ships though.
it used in manual and project way input mode.*/
void getShipDetails(int *len, int *wid, int *amount) {
	printWhite();
	scanf_s("%d %d %d", &(*wid), &(*len), &(*amount));
	if ((*wid) > (*len)) swap(&(*len), &(*wid));
}

/*this function will found location of a dead ship parts that it has been informed to be dead,
so it will make that ships parts to have another character in printing which determine is dead.*/
void kill(int side, int kill, int FOCP[][11], int remainShips[], int size)
{
	int j, k;
	remainShips[side - 1]--;
	for (j = 1; j <= size; j++) { //this loop will look for the dead ship and actually kill it!
		for (k = 1; k <= size; k++) {
			if (FOCP[j][k] == (kill * -1)) FOCP[j][k] = -101;
		}
	}
	playBEsound();
}

//this function will add a secondary number to an string which has a number saved in them. it will be used to draw the text of that string.
void addNumToStr(char tar[], int in) {
	int j = strlen(tar);
	int ic = in;
	int i;
	for (i = 0; ic != 0; i++) {
		ic /= 10;
	}
	i++;
	tar[j] = ' ';
	j++;
	tar[j] = ' ';
	tar[j + i] = 0;
	for (; in != 0; i--) {
		tar[j + i - 1] = (in % 10) + 48;
		in /= 10;
	}
}

//since there was a error with _itoa function, i made my own.
void intToA(char str[], int in) {
	if (in == 0) {
		str[0] = '0';
		str[1] = 0;
	}
	else {
		int ic = in;
		int i;
		for (i = -1; ic != 0; i++) {
			ic /= 10;
		}
		str[i + 1] = 0;
		for (; in != 0; i--) {
			str[i] = (in % 10) + 48;
			in /= 10;
		}
	}
}

//when everything for repairing is valid, this function will finally repair a spot.
void repairShip(int col, int row, int FOCP[][11], int pNUM, int mode, int playerShips[][101]) {
	int temp;
	FOCP[row][col] *= -1;
	temp = FOCP[row][col];
	playerShips[pNUM - 1][temp]++;
	if (mode == 3) saveRepairLoc(row, col);
	playRepairSound();
}

//before a game starts this function will do needed things.
void Start(int mode, char p1[], char p2[], ALLEGRO_SAMPLE **music) {
	playGameMusic(&*music);
	clearscreen();
	printWhite();
	if (mode != 1) printf("Done! Go back to game window\n");
	drawIntoGameText(p1, p2);
}

//this function will count how many ship spots are damaged in a player map.
int countDamagedShipsSpots(int FOCP[][11], int size) {
	int i, j, count = 0;
	for (i = 1; i < size; i++) {
		for (j = 1; j < size; j++) {
			if (FOCP[i][j] < 0 && FOCP[i][j] > -101) count++;
		}
	}
	return(count);
}

//this function is called for repairing. it is used to manipulate repair mechanism.
void repair(int FOCP[][11], int pNUM, char name[], int mode, int playerShips[][101], int remainRepairs[], int size, int side, ALLEGRO_DISPLAY *display) {
	if (remainRepairs[pNUM - 1] == 0) drawRepairError(1);
	else {
		int count = countDamagedShipsSpots(FOCP, size);
		if (count == 0) drawRepairError(2);
		else repairPart(FOCP, pNUM, name, mode, playerShips, size, side, display);
		remainRepairs[pNUM - 1]--;
	}
}

//this function will check if player target a damaged or dead ship.
int checkForShootError(int row, int col, int FOCP[][11]) {
	if (FOCP[col][row] < 0) return(1);
	else return(0);
}

//when a game is done, the user will be asked to play again or exit the game.
int action(int *sw2, ALLEGRO_DISPLAY *display) {
	(*sw2) = 0;
	int sw = showExitDialog(display);
	if (sw == 1) return(0);
	else return(2);
}