//this function will scan map size.
int scanMapSize() {
	int scale;
	printWhite();
	scanf_s("%d", &scale);
	return(scale);
}

//this function will scan ship spot amount.
int scanShipSpotAmount() {
	int count;
	printWhite();
	scanf_s("%d", &count);
	return(count);
}

//this function scan player name in Project Way input mode.
void getPlayerNamePW(char name[]) {
	scanf_s("%s", name, (unsigned)_countof("123456790123456"));
}

//setting up ship with Project Way.
void getShipLocPW(int FOCP[][11], int playerNum, int ships, int ho[], int playerShips[][101], int remainShips[]) {
	int lenght, width, amount, col, row, lenc, widc, c;
	char direction[10], order[20];
	do {
		printWhite();
		getShipDetails(&lenght, &width, &amount);
		remainShips[playerNum - 1] += amount;
		for (c = 0; c < amount; c++) {
			lenc = lenght;
			widc = width;
			scanf_s("%d %d", &row, &col);
			scanf_s("%s", direction, (unsigned)_countof(direction));
			if (direction[0] == 'v' || direction[0] == 'V') swap(&lenc, &widc);
			setShipLoc(FOCP, playerNum, lenc, widc, row, col, playerShips, ho);
		}
		ships -= (lenght * width * amount);
		scanf_s("%s", order, (unsigned)_countof(order));
	} while (order[0] != '-' && order[1] != '-' && order[2] != '-');
}