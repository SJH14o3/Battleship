//this function will print column numbers map. mode 1:debug, 0: others.
void printFirstColumn(int FOCP[][11], int mode, int size) {
	int i;
	setTextColor(6, 0); //column numbers are yellow.
	printf("   "); //this spaces will allign the column numbers with their correspond column.
	for (i = 1; i < size; i++) {
		printf("%d  ", FOCP[0][i]);  //this loop will print column numbers
		//space are printed for debugPrintMap function. unlike other print maps, distance between two column number is 4 in debugPrintMap.
		if (mode) printf("  ");
	}
	printf("\n");
}

//this function will print row #i number.
void printRowNumber(int FOCP[][11], int i) {
	if (i != 10) printf(" "); //printing an space for one digit column to match 10 size which is two digits. 
	setTextColor(6, 0); //row numbers are yellow aswell.
	printf("%d ", FOCP[i][0]);  //printing row number.
}

//this function print player name for functions that print map.
void printName(char name[]) {
	setTextColor(10, 0); //player name is green
	printf("   %s's map\n", name); //print the given player name.
}

//this function print how many a player have alive ships.
void printRemainingShips(int input) {
	setTextColor(11, 0); //printing remaining ships for given player.
	printf("   remaining ships: %d\n", input);
}

//this function will print water which is blue "~"
void printwater() {
	setTextColor(9, 0);
	printf("~");
}

void printChar(int c) {
	if (c == 0) printwater();
	else {
		if (c == 102) setTextColor(red, 0); //ship location has interfrence with another ship.
		else if (c == 103) setTextColor(10, 0); //ship location is good.
		else setTextColor(8, 0); //a ship that has been placed before.
		printf("o");
	}
}

//printing row #j.
void printRow(int FOCP[][11], int size, int i) {
	int j;
	printRowNumber(FOCP, i);
	for (j = 1; j < size; j++) {
		printChar(FOCP[i][j]);
		printf("  ");
	}
	printf("\n\n");
}

//this function will print a player's map who is arranging their map and it will be updated when they click something.
void printMapArrange(int FOCP[][11], int size) {
	int i;
	printFirstColumn(FOCP, 0, size);
	for (i = 1; i < size; i++) printRow(FOCP, size, i);
}

void printMinusNum(int c) {
	setTextColor(red, 0);
	printf("%d", c);
	if (c < -99) printf(" ");
	else if (c < -9) printf("  ");
	else printf("   ");
}

void printPosNum(int c) {
	setTextColor(9, 0);
	printf("%d", c);
	if (c < 10) printf("    "); //this spaces are to fix the distance between numbers.
	else if (c < 100) printf("   ");
	else printf("  ");
}

//this function will print the given player number map but with the exact number, for debugging purposes.
void debugprintmap(int FOCP[][11], int remainShips, char name[], int size) {
	int i, j;
	clearscreen();
	setTextColor(10, 0);
	printf("Debug Map\n");
	printName(name);
	printRemainingShips(remainShips);
	printFirstColumn(FOCP, 1, size);
	for (i = 1; i < size; i++) { //printing row #i.
		printRowNumber(FOCP, i);
		for (j = 1; j < size; j++) {  //printing row #j.
			if (FOCP[i][j] < 0) printMinusNum(FOCP[i][j]);
			else printPosNum(FOCP[i][j]);
		}
		printf("\n\n");
	}
}
