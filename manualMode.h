void setSituation(int y, int x, int map[][11], int len, int wid, int situation); //declaration.
//this section is checking things functions.

//this function will check if second player has entered same name for player 1.
int checkNoSimilarName(char name1[], char name2[]) {
	if (strcmp(name1, name2)) return (1);
	else printErrorSimilarName();
	return(0);
}

//this function will check if details are valid in manual mode more info in function.
int checkValidDetails(int len, int wid, int amount, int ships, int size) {
	//if ship lenght is bigger than map size this error will happen (when player enter ship details, if width is bigger than lenghts, they will be swapped.)
	if (len > (size - 1) || wid > (size - 1)) printErrorLongerThanMap();
	else if (len < 1 || wid < 1) printErrorSmallShip();
	else if (amount < 1) printAmountError();
	//if the given ship is more than remaining ships spots this will be the error for it.
	else if ((len * wid * amount) > ships) printErrorPassedSpotsLimit();
	else return(0);
	return(-1);
}

/*this function is used in manual input mode, during setting ships, if current ship has interference with another ship,
it will be printed in diffrent color.*/
void keyModeCheckInterference(int FOCP[][11], int FOCPcopy[][11], int len, int wid, int x, int y) {
	//at default a ship that is going to be setted at a location is in 1 1 and it is vertical, if it has interfrence there it will be printed red.
	if (checkForInterference(len, wid, y, x, FOCP, 0)) setSituation(y, x, FOCPcopy, len, wid, 103);
	//as i said in the top line, but if there is no interfrence. it will be printed as green.
	else setSituation(y, x, FOCPcopy, len, wid, 102);
}

//when a player rotate a ship, this function will check if the rotated ship is in map, if not it will change a variable.
int checkInMap(int loc, int len, int size) {
	if (len + loc > size) loc = (size - len);
	return(loc);
}

//this section is the get input functions.

//this function recieve map size in manual mode, if an appropriate size is not given, it will ask the user to enter the size again.
int getMapSizeManual() {
	clearscreen();
	int sw, scale;
	printMapSizeHint();
	do {
		sw = 1;
		printWhite();
		scale = scanMapSize();
		if (scale < 11 && scale > 2) sw = 0; //like it has been said in the printf, size must be between 3 to 10. this is correct input.
		else printErrorForMapSize();  //wrong input.
	} while (sw);  //if the correct input is given, loop will be end.
	return (scale);
}

//this function scan amount of ships spots for each player from the user.
int getShipSpotsAmountManual(int size) {
	clearscreen();
	int ships, sw = 1;
	printHintForGettingSpots(size);
	do {
		printWhite();
		ships = scanShipSpotAmount();
		if (ships <= (((size)-1)*((size)-1)) && ships > 0) sw = 0; //correct input.
		else printErrorSpotAmount();
	} while (sw); //if the correct input is given, loop will be end.
	clearscreen();
	return (ships);
}

//get player 1 name.
void getPlayer1Name(char name[]) {
	printOrderGPN(1);
	printWhite();
	scanf_s("%s", name, (unsigned)_countof("123456790123456")); //to pevent a warning message, _countof has been used
	clearscreen();
}

//get player 2 name. since player 2 must not have same name with player one, functions are diffrent.
void getPlayer2Name(char name2[], char name1[]) {
	int sw = 0;
	printOrderGPN(2);
	do
	{
		printWhite();
		scanf_s("%s", name2, (unsigned)_countof("123456790123456"));
		sw = checkNoSimilarName(name1, name2);
	} while (!sw);
	clearscreen();
}

//after setting some ships with same details, player will be asked for action, either continue or they are done.
void getOrderManualmode(int *order) {
	printGetOrderHint();
	getEnter();
	(*order) = _getch();
}

//this function will recieve the amount of repairs in manual input mode.
void getRepairsAmount(int remainRepairs[]) {
	printHintForRepairAmount();
	do
	{
		printWhite();
		scanf_s("%d", &remainRepairs[0]);
		if (remainRepairs[0] > 100) printPassedRepairLimit();
	} while (remainRepairs[0] > 100);
	if (remainRepairs[0] < 0) remainRepairs[0] = 0; //if user give less than 0, remainRepairs will be set at 0.
	remainRepairs[1] = remainRepairs[0];
	clearscreen();
}


//other functions.

/*this function is used for manual input mode when user actively using arrow keys and live map is being printed,
if situation is 102 it mean ship current location does not have interference with another ship.
else if 101 it mean it has interference and will be printed in red.*/
void setSituation(int y, int x, int map[][11], int len, int wid, int situation) {
	int i, j;
	for (i = x; i < x + wid; i++) {
		for (j = y; j < y + len; j++) {
			map[i][j] = situation;
		}
	}
}

//this function control x and y by using arrow keys in manual mode.
void getArrowDir(int *x, int *y, int len, int wid, int size) {
	int ch = _getch(); //when an arrow key is pressed it will send two input. this is the second one.
	if (ch == 72) { //up key. for y is inveresed so if up key is pressed, decreasing y.
		if (*x != 1) (*x)--;
		else printf("\a"); //if x is 1 you can't make it lower, printing a error sound!
	}
	else if (ch == 75) { //left key.
		if (*y != 1) (*y)--;
		else printf("\a"); //if y is 1 you can't make it lower.
	}
	else if (ch == 77) { //right key.
		if (*y < size - len) (*y)++;
		else printf("\a"); //it will prevent from going out side of map from right.
	}
	else { //down key.
		if (*x < size - wid) (*x)++;
		else printf("\a");//it will prevent from going out side of map from down.
	}
}

//when player pressed r, this will change the variable which determine what direction ship is.
int changeDirection(int direction) {
	if (direction == 1) direction++; //1 mean horizontal and 2 mean vertical. so making it vertical here
	else direction--; //making it horizontal here
	return(direction);
}

//this function rotate ship in manual mode. it will call some function to apply needed changes.
int keyModeRotateShip(int *x, int *y, int *len, int *wid, int direction, int size) {
	direction = changeDirection(direction);
	swap(&*len, &*wid);
	(*x) = checkInMap(*x, *wid, size);
	(*y) = checkInMap(*y, *len, size);
	return(direction);
}

//copying whole array. because when setting ships location, we don't want to change in the main array until we press enter.
void copyArrays(int ao[][11], int ac[][11], int size) {
	int i, j;
	for (i = 0; i < size; i++) for (j = 0; j < size; j++) ac[j][i] = ao[j][i];
}

/*this function will recieve coordinates by using arrow keys to set ships location in manual mode.
first a copy of a player map will be made, this map will be updated when a player press a key to indicate where ship is.
if ship has interfrence it will be printed in red, else it will be printed i green*/
int setLoc(int *x, int *y, int FOCP[][11], char name[], int len, int wid, int size) {
	clearscreen();
	int ch = 1, direction = 1, sw;
	int FOCPcopy[11][11];
	copyArrays(FOCP, FOCPcopy, size);
	keyModeCheckInterference(FOCP, FOCPcopy, len, wid, *x, *y);
	do {
		sw = 1;
		printHintForSetLoc(name);
		printMapArrange(FOCPcopy, size); //printing live map.
		ch = _getch();
		clearscreen(); //i did not found a better way so the previous map will be cleared and the new one will be printed from scratch.
		copyArrays(FOCP, FOCPcopy, size);
		if (ch == 'r' || ch == 'R') direction = keyModeRotateShip(&*x, &*y, &len, &wid, direction, size); //pressing 'r' mean Rotate the ship.
		else if (ch == 224) getArrowDir(&*x, &*y, len, wid, size); //an arrow key has been pressed.
		keyModeCheckInterference(FOCP, FOCPcopy, len, wid, *x, *y); //with every click this function is called, colors are important.
	} while (ch != 13 && ch != 'c' && ch != 'C'); //enter mean location is finilized.
	if (ch == 'c' || ch == 'C') return (0); //canceling operation.
	if (direction == 1) return('h'); //horizontal setted ship.
	return('v'); //vertical setted ship.
}

//manual setting up ships mode. this function will scan given player their ships location. it will return error if an incorrect input is given.
int getShipLocKey(int FOCP[][11], int playerNum, int lenght, int width, char name[], int ho[], int playerShips[][101], int remainShips[], int size) {
	int direction, x = 1, y = 1, situation = 0; //situation -1: wrong input, 0: accepted input.
	printWhite();
	//I be honest, I gave wrong x and y to the function so that function x is this function y and same for other parameter. it's workes though.
	direction = setLoc(&y, &x, FOCP, name, lenght, width, size);
	if (!direction) return(-1); //-1 mean cancel.
	if (direction == 'v' || direction == 'V') swap(&lenght, &width);
	situation = checkForInterference(lenght, width, x, y, FOCP, 1);
	if (situation) setShipLoc(FOCP, playerNum, lenght, width, x, y, playerShips, ho);
	return(situation);  //0 mean error. 1 mean succes.
}

//in manual mode this function is the loop after player gave ship details to place ships. player can cancel placing ships though.
int placeShipLoop(int amount, int pNum, int lenght, int width, int FOCP[][11], char name[], int ho[], int playerShips[][101], int remainShips[], int size) {
	int i, sw1, sw;
	for (i = 0, sw1 = 1; i < amount && sw1;) {
		//when sw1 is false, setting ships with the given details are canceled, so they will be asked again for action.
		sw = getShipLocKey(FOCP, pNum, lenght, width, name, ho, playerShips, remainShips, size);
		if (sw == -1) sw1 = 0;
		else if (sw) i++; //if the correct input is given, it will be accepted.
	}
	remainShips[pNum - 1] += i;
	return(lenght * width * i);
}

//this function will scan player ships details and location in manual mode.
void shipLocKey(int ships, int pNum, char name[], int FOCP[][11], int ho[], int playerShips[][101], int remainShips[], int size) {
	int lenght, width, amount, sw = 1, order;
	printHintForManualMode(name);
	do {
		sw = 1;
		printHintDetails(ships);
		do {
			getShipDetails(&lenght, &width, &amount);
			sw = checkValidDetails(lenght, width, amount, ships, size);
		} while (sw);
		ships -= placeShipLoop(amount, pNum, lenght, width, FOCP, name, ho, playerShips, remainShips, size);
		if (!ships) order = '-';
		else getOrderManualmode(&order);
		clearscreen();
	} while (order != '-');
}