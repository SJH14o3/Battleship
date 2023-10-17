//this section is for hints functions.

//just a text.
void printMapSizeHint() {
	/*before every scanf and printf function there will be a setTextColor to change their color,
	because colors won't change after printf or scanf function which there color has been changed for them.*/
	printWhite();
	printf("Enter Map Size (Min is 3 and Max is 10):\n");
}

//this function will print a hint.
void printHintForGettingSpots(int size) {
	printWhite();
	//can you fit 10 ships which are 3 * 1 in a 3 * 3 map?
	printf("Enter the Max Amount of Ships Spots for Each Teams: Min is 1, Max is %d.\n", ((size - 1)*(size - 1)));
}

//this function print a colorful text for getPlayerName function.
void printOrderGPN(int pNum) {
	printWhite();
	printf("Enter");
	setTextColor(12, 0);
	printf(" Player %d name.", pNum);
	printWhite();
	printf(" 15 Characters max.");
	setTextColor(12, 0);
	printf(" DO NOT USE SPACE.\n");
	printWhite();
}

//the name says what it does.
void printHintForManualMode(char name[]) {
	printWhite();
	printf("%s, set up your ship, first enter details of ship.\n", name);
}

//a hint when entering ship details in manual mode.
void printHintDetails(int ships) {
	printWhite();
	printf("Enter details of ship with this method:\n(width) (lenght) (amount)\ne.g: 1 2 2\n%d spots reamaining\n", ships);
}

//it just print a text.
void printHintForSetLoc(char name[]) {
	printWhite();
	printf("%s, arrange your ships using arrow keys.\nclick 'r' to rotate your ship.\n", name);
	printf("Press Enter to set your ship in the current location.\nPressing 'c' will cancel placement of the ship. setted ship won't change.\n");
}

//when a player done with a set of ships, they will be asked to add more or end, this function print a hint for that.
void printGetOrderHint() {
	printWhite();
	printf("Enter Order:\n-: end\nothers: continue adding\n");
}

void printHintForRepairAmount() {
	printWhite();
	printf("Enter amount of repairs:\n");
}



//this section is for error functions.

//just a error for invalid map size.
void printErrorForMapSize() {
	printRed(); //yeah, errors will be printed in red!
	printf("\aSize Must Be Between 3 - 10. Please Enter appropriate size.\n"); //also every error will make sound!
}

//this function print error when number of spots given is incorrect.
void printErrorSpotAmount() {
	printRed();
	printf("\aPlease Enter appropriate amount for ships\n");
}

//in manual mode, when player2 enter same name with player1, this error will be shown.
void printErrorSimilarName() {
	printRed();
	printf("\aPlayer 2 cannot enter same name as Player 1.\nEnter your name again.\n");
}

//when ship has a size less than 1.
void printErrorSmallShip() {
	printRed();
	printf("\aShip size must be at least 1!\n");
}

//when ship is bigger than map, this error will be shown.
void printErrorLongerThanMap() {
	printRed();
	printf("\aShip can't be bigger than Map!\n");
}

void printAmountError() {
	printRed();
	printf("\aEnter at least 1 amount!\n");
}

//when entered details pass remaining ship spots.
void printErrorPassedSpotsLimit() {
	printRed();
	printf("Passed Ship spots limit!\n");
}

//this function will print
void printPassedRepairLimit() {
	printRed();
	printf("Maximum repair is 100, Enter Repair amount again\n");
}