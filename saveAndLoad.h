void saveGame(int turn);
void loadBotMemory();

errno_t FileH;

//with this File, we can check if there is a saved in progress game. if it is true, it will print an extra line and extra command will be availabe.
int checkForSavedFile(ALLEGRO_DISPLAY *display) {
	FILE *check;
	FileH = fopen_s(&check, "save.txt", "rt");
	int sw;
	if (check) fscanf_s(check, "%d", &sw);
	else sw = 0;
	fclose(check);
	if (!sw) al_show_native_message_box(display, "Native Message box", "ERROR", "No Saved Game Detected", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	else return(1);
	return(0);
}

//when a loaded game ends, the file will be reseted. by simply adding 0 to the first line.
void resetLoadedGameFile() {
	FILE *check;
	//next two lines will reset the saved file. it will delete content of save and replace it with a 0. 0 mean there is no saved game.
	FileH = fopen_s(&check, "save.txt", "wt");
	fprintf(check, "0");
	fclose(check);
}

//this will determine what game mode game is using player 2 name, because player 2 name can't have space, no mistake will be made.
int saveGameMode(char p2[], FILE **save) {
	int mode;
	if (!strcmp(p2, "Easy Bot")) mode = 2;
	else if (!strcmp(p2, "Hard Bot")) mode = 3;
	else mode = 1;
	fprintf(*save, "%d\n", mode);
	return(mode);
}

//this function will save every content of their map excluding column and row numbers.
void savePlayerMap(int FOCP[][11], int size, FILE **save) {
	int i, j;
	for (i = 1; i < size; i++) { //this loop save player 1 map.
		for (j = 1; j < size; j++) fprintf(*save, "%d ", FOCP[i][j]);
		fprintf(*save, "\n"); //a little bit user friendly change. with this it will be easy to read map via the text file.
	}
}

//this will save player one stats into file.
void savePlayer1(char p1[], int remainShips[] , int remainRepairs[], int FOCP1[][11], int playerShips[][101], int size, FILE **save) {
	fprintf(*save, "%s\n", p1); //player one name.
	fprintf(*save, "%d\n", remainShips[0]); //player one remaining ships.
	fprintf(*save, "%d\n", remainRepairs[0]); //player one remaining repairs.
	savePlayerMap(FOCP1, size, &*save);
	for (int i = 0; i < 101; i++) fprintf(*save, "%d ", playerShips[0][i]); //saving player 1 dead ship mechanism.
}

//this will print player two stats. also it will save hard bot memory if the game it is one that mode.
void savePlayer2(int mode, char p2[], int remainShips[], int remainRepairs[], int FOCP2[][11], int playerShips[][101], int size, FILE **save) {
	if (mode == 1) fprintf(*save, "%s\n", p2); //  player two name. since bot names are determined, there is no need to save their names.
	fprintf(*save, "%d\n", remainShips[1]); // player two remaining ships.
	fprintf(*save, "%d\n", remainRepairs[1]); //player two remaining repairs
	savePlayerMap(FOCP2, size, &*save);
	for (int i = 0; i < 101; i++) fprintf(*save, "%d ", playerShips[1][i]); //saving player 2 dead ship mechanism.
}

//function loads player one required stats from loading a game.
void loadPlayer1(FILE **load, int remainShips[], int FOCP1[][11], int playerShips[][101], int remainRepairs[], char p1[], int size) {
	int i, j;
	fscanf_s(*load, "%s", p1, _countof("01234567890123456"));
	fscanf_s(*load, "%d %d", &remainShips[0], &remainRepairs[0]);
	for (j = 1; j < size; j++) for (i = 1; i < size; i++) fscanf_s(*load, "%d", &FOCP1[j][i]); // Reading player1 map
	for (i = 0; i < 101; i++) fscanf_s(*load, "%d", &playerShips[0][i]); //reading player1 if alive ship mechanism
}

//function loads player one required stats from loading a game.
void loadPlayer2(int mode, FILE **load, int remainShips[], int FOCP2[][11], int playerShips[][101], int remainRepairs[], char p2[], int size) {
	int i, j;
	if (mode == 1) fscanf_s(*load, "%s", p2, _countof("01234567890123456")); // Determining the player2 name basde on the mode.
	else if (mode == 2) strcpy_s(p2, 16, "Easy Bot");
	else if (mode == 3) strcpy_s(p2, 16, "Hard Bot");
	fscanf_s(*load, "%d %d", &remainShips[1], &remainRepairs[1]);
	for (j = 1; j < size; j++) for (i = 1; i < size; i++) fscanf_s(*load, "%d", &FOCP2[j][i]);  // Reading player2 map
	for (i = 0; i < 101; i++) fscanf_s(*load, "%d", &playerShips[1][i]); //reading player2 if alive ship
	if (mode == 3) loadBotMemory(); //bot hard has a memory, to maintain its powers, it must be restored.
}