errno_t forFile; //visual studio act weird here. i had to use fopen_s duo to security. this will let me open files properly.
FILE *inputF; // File pointer for file input mode.

//this function will read map size in the file when on file input mode.
int getMapSizeFile() {
	int scale;
	fscanf_s(inputF, "%d", &scale);
	scale++;
	return (scale);
}

//this function scan amount of ships spots for each player from the file.
int getShipSpotAmountFile() {
	int ships; // total ships
	fscanf_s(inputF, "%d", &ships);
	return (ships);
}

//this function is like last function but from file.
void fgetShipDetails(int *wid, int *len, int *amount) {
	fscanf_s(inputF, "%d %d %d", &(*wid), &(*len), &(*amount));
	if ((*wid) > (*len)) swap(&(*len), &(*wid));
}

//file setting up ships mode. this function will scan given player their ships location. it will return error if an incorrect input is given.
void getShipLocFile(int FOCP[][11], int ships, int playerNum, int remainShips[], int ho[], int playerShips[][101])
{
	int len, wid, lenc, widc, amount, i, row, col;
	char order[4], direction[2];
	do
	{
		fgetShipDetails(&wid, &len, &amount);
		remainShips[playerNum - 1] += amount;
		for (i = 0; i < amount; i++)
		{
			lenc = len;
			widc = wid;
			fscanf_s(inputF, "%d %d", &row, &col);
			fscanf_s(inputF, "%s", direction, _countof(direction));
			if (direction[0] == 'v' || direction[0] == 'V') swap(&lenc, &widc);
			setShipLoc(FOCP, playerNum, lenc, widc, row, col, playerShips, ho);
		}
		fscanf_s(inputF, "%s", order, _countof(order));
	} while (order[0] != '-' && order[1] != '-' && order[2] != '-');
}

//scan player name in file input mode.
void getPlayerNameFile(char name[]) {
	fscanf_s(inputF, "%s", name, _countof("1234567890123456")); //too prevent a warning message, _countof has been used.
}

//this function will recieve the amount of repairs in file input mode.
void fgetRepairsAmount(int remainRepairs[]) {
	fscanf_s(inputF, "%d", &remainRepairs[0]);
	remainRepairs[1] = remainRepairs[0];
}

//this function call every needed function for file input mode.
void shipLocFile(int ships, int mode, char name1[], char name2[], int FOCP1[][11], int FOCP2[][11], int remainRepairs[], int remainShips[], int ho[], int playerShips[][101]) {
	getPlayerNameFile(name1);
	getShipLocFile(FOCP1, ships, 1, remainShips, ho, playerShips);
	/*since total repairs most be at last of the text file,
	even in single player this will be scanned but it has not effect, bots still set their ships by their AI.*/
	getPlayerNameFile(name2);
	getShipLocFile(FOCP2, ships, 2, remainShips, ho, playerShips);
	fgetRepairsAmount(remainRepairs);	
}

//call every needed functions for file input mode. it is here since file in this header is global.
int fileInputMode(int *scale, int mode, char name1[], char name2[], int FOCP1[][11], int FOCP2[][11], int playerShips[][101], int ho[], int remainShips[], int remainRepairs[]) {
	forFile = fopen_s(&inputF, "inputFile.txt", "rt");
	(*scale) = getMapSizeFile();
	int ships = getShipSpotAmountFile();
	initiateGame(FOCP1, FOCP2, playerShips, ho, remainShips, *scale);
	shipLocFile(ships, mode, name1, name2, FOCP1, FOCP2, remainRepairs, remainShips, ho, playerShips);
	fclose(inputF);
	return(ships);
}