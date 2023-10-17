void botHardHit(int i, int j, int k);
//this function return a number between 1 to 100, it is like a percentage. it used for setting a random ship size for bot.
int botSetRandomShipSize() {
	int rng = rand();
	rng %= 100;
	rng++;
	return(rng);
}

/*this function will receive a number. this number is like a percentage for setting a diffrent ship size for bots.
every ship size has a percentage.the number this function return will determine what ship will it be.*/
int botShipSize(int rng, int *vertical) {
	if (rng <= 87) {
		*vertical = 1;
		if (rng <= 10) return(1); //10% chance for 1*1.
		else if (rng <= 35) return(2); //25% chance for 1*2.
		else if (rng <= 70) return(3); //35% chance for 1*3.
		else if (rng <= 80) return(4); //10% chance for 1*4.
		else return(5); //7% chance for 1*5.
	}
	*vertical = 2;
	if (rng <= 97) return(2); //10% chance for 2*2.
	return(3); //3% chance for 2*3.
}

//this function recieve a ship dimension setted by bot, if remaining ship spots are less than ship size it will return 0 which mean false.
int botCheckValidShipSize(int len, int wid, int remain) {
	int volume = len * wid;
	if (volume > remain) return(0);
	return(1);
}

//a randomizer to set a bot ship direction.
int botSetShipDirection() {
	int rng = rand();
	return(rng % 2);
}

//a random generated location for setting our ships.
int botSetRandomLoc(int *x, int *y, int len, int wid, int size) {
	if (len >= size || wid >= size) return(0);  //to prevent putting ships larger than map size, for example 1 * 5 ship in a 3 * 3 map.
	*x = rand();
	*y = rand();
	*x %= (size - len);
	*y %= (size - wid);
	(*x)++;
	(*y)++;
	return(1);
}

/*this function make the bot to try etting the ship in a location, if there is no error it will be setted there.
first it will set direction, then it will set the ship to the random loaction. if there is no interfrence ship will be set there and function will return true.
if there is a error bot will do the last line again. if it fails 5 time, function will return false. size of the ship will be set again.*/
int botTrySetLoc(int len, int wid, int FOCP2[][11], int ho[], int playerShips[][101], int size) {
	int x, y, dir, st, st1;
	for (int i = st = 0; i < 5 && !st; i++) {
		dir = botSetShipDirection();
		if (dir == 1) swap(&len, &wid);
		st1 = botSetRandomLoc(&x, &y, len, wid, size);
		if (st1) st = checkForInterference(len, wid, x, y, FOCP2, 0);
	}
	if (st) setShipLoc(FOCP2, 2, len, wid, x, y, playerShips, ho);
	return(st);
}

//this function is the loop for the bot trying to set their ships. 
void botSetLoc(int spots, int FOCP2[][11], int ho[], int playerShips[][101], int size, int remainShips[]) {
	int len, wid, rng, ships = 0;
	while (spots != 0) {
		rng = botSetRandomShipSize();
		len = botShipSize(rng, &wid);
		if (botCheckValidShipSize(len, wid, spots) && botTrySetLoc(len, wid, FOCP2, ho, playerShips, size)) { //if this conditions met, bot has successfully setted a ship.
			spots -= (len * wid);
			ships++;
		}
	}
	remainShips[1] = ships;
}

//randomizing location to shot
void botTargetRandomLoc(int *x, int *y, int size) {
	*x = rand();
	*y = rand();
	*x %= (size - 1);
	*y %= (size - 1);
	(*x)++;
	(*y)++;
}

//where the bot shot.
void printWhereBotShot(int i, int j) {
	drawShoot(j, i);
	overlayShootLoc(i, j);
	playShootSound();
}

//a 10% randomizer, used for easy bot.
int tenPercent() {
	int chance = rand();
	return(chance % 10);
}

//since bot easy AI is hit, this is literally them when hitting a target. they won't scan around it and other things.
int botEasyHit(int i, int j, int FOCP1[][11], int playerShips[][101], int remainShips[], int size) {
	int temp;
	temp = FOCP1[i][j];
	FOCP1[i][j] *= -1; //damaging a ship, literally.
	playerShips[0][temp]--;
	if (playerShips[0][temp] == 0) drawDestroyed(1, temp, FOCP1, remainShips, size);
	else drawHit(1);
	return(0);
}

//easy bot AI.
void botEasyShoot(int FOCP1[][11], int playerShips[][101], int remainShips[], int size) {
	int i, j, chance, sw = 1;
	al_rest(0.5);
	chance = tenPercent(); //bot easy has a 10% chance to remember the location they shot!
	do {
		botTargetRandomLoc(&i, &j, size);
		//this condition will stop bot at shooting at a remembered missed shot or a damaged ship or a destroyed ship. it is easy but it is not stupid.
		if (FOCP1[i][j] == 101 || FOCP1[i][j] < 0);
		else {
			printWhereBotShot(i, j);
			if (FOCP1[i][j] == 0) {
				//bot easy has a 10% chance to remember the location they shot! if chance is 0 which mean false it will remember that location.
				if (!chance) FOCP1[i][j] = 101;
				drawMissed();
				sw = 0;
			}
			else if (FOCP1[i][j] > 0) sw = botEasyHit(i, j, FOCP1, playerShips, remainShips, size);
		}
	} while (sw);
}

//when easy bot has a damaged spot it has 10% chance to repair a random damaged location.
void botEasyRepair(int count, int FOCP2[][11], int playerShips[][101], int remainRepairs[], int size) {
	int i, j, c, temp, sw = 1, rng = rand();
	rng %= count;
	for (c = 0, i = 1; i < size && sw; i++) {
		for (j = 1; j < size && sw; j++) {
			if (FOCP2[i][j] < 0 && FOCP2[i][j] > -101) {
				if (c == rng) {
					FOCP2[i][j] *= -1;
					temp = FOCP2[i][j];
					playerShips[1][temp]++;
					sw = 0;
				}
				else c++;
			}
		}
	}
	remainRepairs[1]--;
	drawBotUsedRepair();
}

//what bot easy is going to do.
void botEasyAction(int FOCP1[][11], int FOCP2[][11], char p2[], int playerShips[][101], int remainRepairs[], int remainShips[], int size) {
	drawMap(2, FOCP1, remainShips[0], remainRepairs, p2, size);
	al_flip_display();
	int count = countDamagedShipsSpots(FOCP2, size);
	if (count) {
		int rng = tenPercent();
		if (!rng && remainRepairs[1]) botEasyRepair(count, FOCP2, playerShips, remainRepairs, size);
		else botEasyShoot(FOCP1, playerShips, remainShips, size);
	}
	else botEasyShoot(FOCP1, playerShips, remainShips, size);
}

//Missed! actually it is here since bot hard remember every missed shot locations. they won't shoot there again.
void botHardMissed(int i, int j, int FOCP1[][11]) {
	FOCP1[i][j] = 101;
	drawMissed();
}

//when bot has remember a location of a spot this function is called.
int botHardAggroShot(int i, int j, int k, int FOCP1[][11]) {
	printWhereBotShot(i, j);
	if (FOCP1[i][j] == 0) botHardMissed(i, j, FOCP1);
	else if (FOCP1[i][j] > 0) botHardHit(i, j, k);
	return (0);
}

/*this function will check around the location bot want to repair, if there is a damaged spot,
it won't use repair. first it will check if the location is in the map.*/
int botHardCheckAroundRepairSpot(int row, int col, int FOCP2[][11], int size) {
	int i, sw = 1;
	//checking up-left and up of the spot, because the first column is just numbers, this won't have any problem.
	for (i = col - 1; i < col + 1 && sw; i++) if (FOCP2[row - 1][i] < 0) sw = 0;
	if (col < size - 1 && sw) if (FOCP2[row - 1][col + 1] < 0) sw = 0; //checking up-right.
	if (sw && FOCP2[row][col - 1] < 0) sw = 0; //checking left.
	if (col < size - 1 && sw) if (FOCP2[row][col + 1] < 0) sw = 0; //checking left.
	if (row < size - 1 && sw) for (i = col - 1; i < col + 1 && sw; i++) if (FOCP2[row + 1][i] < 0) sw = 0; //checking down-left, down
	if (row < size - 1 && col < size - 1 && sw) if (FOCP2[row + 1][col + 1] < 0) sw = 0; //checking down-right.
	return(sw);
}

//when every condition for hard bot has been met, bot repair that location.
void BotHardRepaired(int row, int col, int FOCP2[][11], int ifRepaired[], int playerShips[][101], int remainRepairs[]) {
	int temp;
	drawBotUsedRepair();
	FOCP2[row][col] *= -1;
	temp = FOCP2[row][col];
	playerShips[1][temp]++;
	ifRepaired[temp] = 1;
	remainRepairs[1]--;
}
