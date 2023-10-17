#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<allegro5\allegro.h>
#include<allegro5\allegro_acodec.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_image.h>
#include<allegro5\color.h>
#include<allegro5\allegro_native_dialog.h>
#include"external.h" //this header contain some function which TAs gave us to use in the project.
#include"playSound.h" //this header contain functions to play in game sound and musics. musics are disabled duo to quera upload limit.
#include"misc.h" //other functions or functions that are used in different headers, it has been included higher than most of headers for that.
#include"map.h" //it will used for printing map in console.
#include"initials.h" //before a game starts some changes must be applied. functions in this header will do that.
#include"allegroDraw.h" //contain function which draw text and images from allegro5.
#include"bots.h" //most of functions for bots are in this header.
#include"saveAndLoad.h" //contain most of functions which are needed for saving and loading game.
#include"projectWayMode.h" //contain every needed function for project way input mode. i have explaied diffrent input modes after including headers.
#include"manualModeTexts.h" //print hints and errors for manual mode.
#include"manualMode.h" //contain needed function for manual input mode.
#include"fileMode.h" //contain needed function for file input mode.
#include"allegroKeyboard.h" //contain functions for handling keyboard inputs in allegro display.

/*There are three ways to input. first and best is manual mode. inputs will be received in console.
in manual mode user can't give wrong output. errors will printed if this happen. player will use arrow keys to set their ships.
a live map will be printed to show placed ships and the ship which is being placed.
second is project way mode. it will receive inputs the way for project. we don't expect user to enter inputs with errors.
third is file mode. file mode open a text file which user have saved like a project way input but in file. like project way, we don't expect errors.*/

//player names strings
char p1[16];
char p2[16];

/*size of the map, also for making job easier after scanning size it will sum with one.
for example the array will be used to print column and row numbers.
also the given coordinates will match the players array index.*/
int size;

/*board for each players. max size is 11. it is just numbers.
ignoring first column and first row, 0 mean water, 1 to 100 mean a ship, the number is correspond to the ship number.
-100 to -1 mean damaged ship, the -(number) is correspond to the ship number. -101 mean a dead ship.
101 mean a bot remembered spot, it happen when a bot missed a shot and rememberd that spot so bot won't shoot at that location again.
102 and 103 are used in manual input mode. when a ship has interfrence with another ship, it will be 102, so it will print as red.
103 mean correct so it will be printed as green.*/
int FOCP1[11][11], FOCP2[11][11];

/*the (if ship alive) mechanism. [0][] for player1 ,[1][] for player2.
this arrays will be used for determining how much alive spots does a ship has. every ship part has a number, parts of a ship have the same number.
the array index match the ship number, if ship #2 has 5 spots, then playerships[player][2] = 5.
main use of this array is to keep how many parts the #i ship has and if it is alive, when a part of that ship is damaged,
the #i ship index in this array will be one less, when one house reach zero it mean the last part of that ship is damaged
so the ship must count as a dead ship now.*/
int playerShips[2][101];

/*this global variable are used to help place the ships in the array with a correct number, [0] for player1, [1] for player2.
it starts at 1, when a player place a ship, that ship will be number 1, then ho[player]++ happen, next ship will be #2.*/
int ho[2];

int remainShips[2]; // number of remaining ships for players
int remainRepairs[2]; //number of repairs left for each player


FILE *save; //file pointer for saving file.
FILE *load; //file pointer for loading file.

errno_t test; //visual studio act weird here. i had to use fopen_s duo to security. this will let me open files properly.

ALLEGRO_DISPLAY *display; //Alegro display window.
ALLEGRO_SAMPLE *music; //music.
ALLEGRO_SAMPLE *ui; //a sound for clicking buttons.

/*this structure is used for hard bot. if a hard bot found a target, it will lock around it, this structure is to remember that location.
whenever bot find new spots, that location will come here, since in aggro shoot mode bot check from last of this structure,
it will look around the last damaged spot. when a ship is destroyed, the aggro from every spot of that ship will be cleared from structure.*/
struct hardBotMemory {
	int row;
	int col;
} Aggro[30];

/*this structure is used for hard bot and has two uses. basically it saves a location and a condition.

1: when a player hit a ship of the bot the location will be saved in last and st (condition) will be saved to 1.
when last.st is 1, bot will know player has hit them, for a 50% chance bot will repair that spot but before doing that,
first it check if they have repaired that ship before, because for example player shoot at one spot and bot repair it.
but player shoot at that spot again. it doens't make sence to repair that spot again so they will shoot instead.
if they have not then bot check around the damaged spot and check if there is a damaged spot.
bot hard it is smart enough to understand they should not repair a spot while other spot around it are damaged too.
if the ship has not been repaired and there is no damaged ship around the spot, that spot will be repaired.

2: when a player use a repair. bot has a 50% chance to forget that spot, so most likely it won't attack that spot again.
or less likely will randomly shoot at that spot again. so you never know... .for other 50%, it will shoot at repaired spot immediately.*/
struct LocAndCon {
	int st; //it st = 1, it mean player hit the bot in last round or player just used a repair.
	int col;
	int row;
} last, humanRepaired = { 0 };

int ifRepaired[101]; //as i explained in the LocAndCon comment, this array is where bot will know if this ship has repaired before.

void gameCore(int whoStart, int mode); //declaration.
void botTargetRandomLoc(int *x, int *y, int size); //declaration.

//note: since i was not able to send structures to a header, i had to keep every functions that used structure in source.cpp

//bot hard has a memory, it must be reset to default when starting a new game.
void initiateHardBot() {
	for (int i = 0; i < 30; i++) Aggro[i].col = Aggro[i].row = 0;
	for (int i = 0; i < 101; i++) ifRepaired[i] = 0;
	last.st = 0;
}

//this function call every needed function for manual input mode.
void shipLocManual(int ships, int mode) {
	getPlayer1Name(p1);
	shipLocKey(ships, 1, p1, FOCP1, ho, playerShips, remainShips, size);
	if (mode == 1) { //if it is not multiplayer this will be skipped. bot can handle themself for setting their ships.
		getPlayer2Name(p2, p1);
		shipLocKey(ships, 2, p2, FOCP2, ho, playerShips, remainShips, size);
	}
	getRepairsAmount(remainRepairs);
	getEnter();
}

//this function call every needed function for project way input mode.
void shipLocPW(int ships, int mode) {
	getPlayerNamePW(p1);
	getShipLocPW(FOCP1, 1, ships, ho, playerShips, remainShips);
	if (mode == 1) {
		getPlayerNamePW(p2);
		getShipLocPW(FOCP2, 2, ships, ho, playerShips, remainShips);
	}
	scanf_s("%d", &remainRepairs[0]);
	remainRepairs[1] = remainRepairs[0];
	getEnter();
}

//bot hard has a memory and it must be saved for loading the game.
void saveBotMemory() {
	int i;
	fprintf(save, "\n");
	for (i = 0; i < 30; i++) fprintf(save, "%d %d ", Aggro[i].col, Aggro[i].row);
	fprintf(save, "\n");
	for (i = 0; i < 101; i++) fprintf(save, "%d ", ifRepaired[i]);
	fprintf(save, "\n");
	fprintf(save, "%d %d %d\n%d %d %d", last.st, last.col, last.row, humanRepaired.st, humanRepaired.col, humanRepaired.row);
}

//when this function is called it will save every needed variable and it will exit the game. player will able to continue game later.
void saveGame(int turn) {
	int mode;
	clearscreen();
	test = fopen_s(&save, "save.txt", "wt");
	fprintf(save, "1\n"); //first line, 1 mean there is a saved game. 0 mean there is not a saved game.
	mode = saveGameMode(p2, &save);
	fprintf(save, "%d\n", size); //saving size of the map.
	if (mode == 1) fprintf(save, "%d\n", turn); //this will print what player turn is. it only used for multiplayer.
	else fprintf(save, "1\n"); //bot can't ask for a break right?
	savePlayer1(p1, remainShips, remainRepairs, FOCP1, playerShips, size, &save);
	fprintf(save, "\n---\n");//a distance.
	savePlayer2(mode, p2, remainShips, remainRepairs, FOCP2, playerShips, size, &save);
	if (mode == 3) saveBotMemory(); //saving hard bot memory.
	fclose(save);
	exit(0);
}

//during loading a game with hard bot, bot needs to restore it memory.
void loadBotMemory() {
	int i;
	for (i = 0; i < 30; i++) {
		fscanf_s(load, "%d", &Aggro[i].col);
		fscanf_s(load, "%d", &Aggro[i].row);
	}
	for (i = 0; i < 101; i++) fscanf_s(load, "%d", &ifRepaired[i]);
	fscanf_s(load, "%d %d %d %d %d %d", &last.st, &last.col, &last.row, &humanRepaired.st, &humanRepaired.col, &humanRepaired.row);
}

//when this function is called it will load the saved game via file.
void loadGame() {
	int isSave, mode, whosTurn;
	char dash[4];
	test = fopen_s(&load, "save.txt", "rt"); // Opening the save file to read the info
	fscanf_s(load, "%d", &isSave);
	fscanf_s(load, "%d %d %d", &mode, &size, &whosTurn);
	loadPlayer1(&load, remainShips, FOCP1, playerShips, remainRepairs, p1, size);
	fscanf_s(load, "%s", dash, _countof(dash));
	loadPlayer2(mode, &load, remainShips, FOCP2, playerShips, remainRepairs, p2, size);
	initialBoard(2, FOCP1, FOCP2, size); //it is not a fresh game. see this function for more info.
	fclose(load);
	Start(1, p1, p2, &music);
	gameCore(whosTurn, mode);
}

//this function call needed function for the input type that has been given.
void shipLocMode(int ships, char mode)
{
	if (mode == 2) shipLocManual(ships, 1); //the manual input mode, which works by arrow keys and a lot of hints.
	else shipLocPW(ships, 1); //the project way. no hints and just enter some number.
}

//this function is like previous but for single player. only diffrent is it will send 0 instead of 1. so it won't ask bot stats from player.
void shipLocModeSingle(int ships, char mode) {
	if (mode == 2) shipLocManual(ships, 0);
	else if (mode == 3) shipLocPW(ships, 0);
}

/*when a player repair their ship in single player with hard bot, that location will be saved.
hard bot has a chance to remember this location and shoot at it again.*/
void saveRepairLoc(int row, int col) {
	humanRepaired.st = 1;
	humanRepaired.col = col;
	humanRepaired.row = row;
}

//valid shot. when there is no error.
void shootNoError(int row, int col, int FOCP[][11], int mode, int player, int pships[]) {
	int temp;
	setTextColor(red, 0);
	if (FOCP[row][col] == 0) drawMissed();//missed shot
	else if (FOCP[row][col] > 0) { //landing a hit!
		temp = FOCP[row][col]; //saving the number, it will be used in the (if alive ship) mechanism.
		FOCP[row][col] *= -1; //this mean this ship is damaged now and it will be printed as a light red x on map.
		pships[temp]--; //i explained how (if alive ship) mechanism works.
		if (pships[temp] == 0) drawDestroyed(player, temp, FOCP, remainShips, size); //if it is 0 it mean a ship has been just destroyed!
		else {
			drawHit(1);
			if (mode == 3) { //it will store last damaged spot. used for bot hard using a repair.
				last.st = 1;
				last.col = col;
				last.row = row;
			}
		}
	}
}

//this function is called whenever for some reason a aggro location must be forgotten.
void botHardSortAggro(int c) {
	for (; Aggro[c].col; c++) {
		Aggro[c].col = Aggro[c + 1].col;
		Aggro[c].row = Aggro[c + 1].row;
	}
}

//when a ship is destroyed, bot needs to clear the location of that ship from their memory.
void botHardClearAggro() {
	int c, i, j;
	for (c = 0; Aggro[c].col;) {
		j = Aggro[c].col;
		i = Aggro[c].row;
		if (FOCP1[i][j] == -101) botHardSortAggro(c);
		else c++;
	}
}

/*comparing to easy bot, this is more complex definitely. when bot hard find a spot,
it will save it to memory, next rounds it will scan around that location.*/
void botHardHit(int i, int j, int k) {
	int temp = FOCP1[i][j];
	Aggro[k + 1].row = i; //adding new damaged location to aggro.
	Aggro[k + 1].col = j;
	FOCP1[i][j] *= -1;
	playerShips[0][temp]--;
	if (playerShips[0][temp] == 0) {
		drawDestroyed(1, temp, FOCP1, remainShips, size);
		botHardClearAggro();
	}
	else drawHit(1);
}

//PC: pararell with column. bot will shot at a spot where is pararell with column with the remembered spot.
int botHardShotPC(int k, int FOCP1[][11]) {
	int i, j, sw = 1;
	i = Randomizer(); //randomizng to shot at back or front.
	if (i == 1) i = Aggro[k].row + 1;
	else i = Aggro[k].row - 1;
	j = Aggro[k].col;
	if (i > 0 && i < size) {
		if (FOCP1[i][j] == 101 || FOCP1[i][j] < 0); //stop bot to be stupid (it won't shot at a damaged/dead ship or a remembered missed shot)
		else sw = botHardAggroShot(i, j, k, FOCP1);
	}
	return(sw);
}

//same as previous function but for row.
int botHardShotPR(int k, int FOCP1[][11]) {
	int i, j, sw = 1;
	j = Randomizer();
	if (j == 1) j = Aggro[k].col + 1;
	else j = Aggro[k].col - 1;
	i = Aggro[k].row;
	if (j > 0 && j < size) {
		if (FOCP1[i][j] == 101 || FOCP1[i][j] < 0);
		else sw = botHardAggroShot(i, j, k, FOCP1);
	}
	return(sw);
}

/*first it will randomize to pararell to what dimenstion. it has a limit. when ever for some reason bot can't shoot somewhere,
like every spots around a spot are remebered spot or damaged. if this happen that location will be thrown out of memory.*/
int BotHardAggroMode(int *total, int k, int FOCP1[][11]) {
	int t, sw = 1;
	do {
		t = Randomizer();
		if (t == 1) sw = botHardShotPC(k, FOCP1); //pararell with column.
		else sw = botHardShotPR(k, FOCP1); //pararell to row.
		(*total)--;
	} while (sw && (*total));
	return(sw);
}

//when there is no aggro, bot shoot randomly.
int botHardNoAggroMode(int i, int j, int FOCP1[][11]) {
	int temp, sw = 1;
	printWhereBotShot(i, j);
	if (FOCP1[i][j] == 0) {
		botHardMissed(i, j, FOCP1);
		sw = 0;
	}
	else if (FOCP1[i][j] > 0) {
		sw = 0;
		temp = FOCP1[i][j];
		FOCP1[i][j] *= -1;
		playerShips[0][temp]--;
		if (playerShips[0][temp] == 0) drawDestroyed(1, temp, FOCP1, remainShips, size);
		else {
			drawHit(1);
			Aggro[0].row = i;   //activationg aggro!
			Aggro[0].col = j;
		}
	}
	return(sw);
}

//Hard bot AI. it is much smarter.
void botHardShoot() {
	al_rest(0.5);
	int i, j, k, total, sw = 1;
	for (k = 29, total = 20; k >= 0 && sw;) { //if bot has find a spot, it will enter this loop.
		total = 20; //to prevent infinite loop. like if it has shot all around a location and randomizer can't output a valid location.
		if (Aggro[k].row) sw = BotHardAggroMode(&total, k, FOCP1); //if true, mean bot has found a ship and now want to destroy it!
		if (!total) botHardSortAggro(k); //when a failed aggro happen, bot will remove that location from it memory.
		else k--;
	}
	if (sw) { //if there is no aggro or a failed aggro happen (an infinite loop has been prevented).
		do { //randomizng to shot.
			botTargetRandomLoc(&i, &j, size);
			if (FOCP1[i][j] == 101 || FOCP1[i][j] < 0); //stop bot shooting at a dead/damaged ship or a remembered missed shot location.
			else sw = botHardNoAggroMode(i, j, FOCP1);
		} while (sw);
	}
}

//if bot is damaged, first they check if they have repaired that ship before.
void botHardCheckForRepair() {
	int st = 0;
	int i = last.row;
	int j = last.col;
	int temp = FOCP2[i][j] * -1;
	if (!ifRepaired[temp]) st = botHardCheckAroundRepairSpot(i, j, FOCP2, size);
	if (st) BotHardRepaired(i, j, FOCP2, ifRepaired, playerShips, remainRepairs);
	else botHardShoot();
}

//when bot remembered player repair spot, it will be forced to shoot at it.
void botHardForceShoot() {
	int col = humanRepaired.col;
	int row = humanRepaired.row;
	printWhereBotShot(row, col);
	drawShoot(col, row);
	shootNoError(row, col, FOCP1, 2, 2, playerShips[0]);
}

//when a player use a repair this function is called.
void botHardPlayerUsedRepair() {
	int c, sw = 1, i = Randomizer();
	humanRepaired.st = 0; //redoing this to zero, it will stay zero until player use repair again.
	if (i == 1) {
		for (c = 0; sw;c++) {
			if ((Aggro[c].col == humanRepaired.col) && (Aggro[c].row == humanRepaired.row)) {
				sw = 0;
				botHardSortAggro(c);
			}
		}
		botHardShoot();
	}
	else botHardForceShoot();
}

//what bot hard must do?
void botHardAction() {
	int i;
	drawMap(2, FOCP1, remainShips[0], remainRepairs, p2, size);
	al_flip_display();
	if (last.st) {
		last.st = 0;
		i = Randomizer();
		if (i == 1 && remainRepairs[1]) botHardCheckForRepair();
		else botHardShoot();
	}
	else if (humanRepaired.st) botHardPlayerUsedRepair();
	else botHardShoot();
}

//this function will call the appropriate function for getting map size.
int getShipSpotAmount(int mode) {
	int count;
	if (mode == 2) count = getShipSpotsAmountManual(size);
	else count = scanShipSpotAmount();
	return(count);
}

//this function will call appropriate function for recieving map size.
void getMapSize(int mode) {
	if (mode == 2) size = getMapSizeManual();
	else size = scanMapSize();
	size++;
}


//this function ask user if they want to use file mode or manual mode or project way for getting them input.
int shipInputMode() {
	int mode;
	mode = InputMenuSelect(ui);
	if (mode != 1) {
		clearscreen();
		showToUseConsole();
		playArrangeMusic(&music);
	}
	return(mode);
}

//as it says, the core of the game, this function will control rounds of game.
void gameCore(int whoStart, int mode) {
	if (whoStart == 1)
	{
		printWhoStarts(p1);
		do { //game starts!
			//debugprintmap(FOCP2, remainShips[1], p2, size); //debug print map it disabled, no debugging is happening.
			MapButtons(1, FOCP2, remainShips[1], mode, 2, p1, display, playerShips, remainRepairs, size, FOCP1);
			if (remainShips[1]) { //making sure the player who started as second can't do anything when they died.
				//debugprintmap(FOCP1, remainShips[0], p1, size);
				if (mode == 1) MapButtons(2, FOCP1, remainShips[0], mode, 1, p2, display, playerShips, remainRepairs, size, FOCP2);
				else if (mode == 2) botEasyAction(FOCP1, FOCP2, p2, playerShips, remainRepairs, remainShips, size);
				else botHardAction();
			}
		} while (remainShips[0] > 0 && remainShips[1] > 0);
	}
	else {
		printWhoStarts(p2);
		do { //game starts!
			//debugprintmap(FOCP1, remainShips[0], p1, size);
			if (mode == 1) MapButtons(2, FOCP1, remainShips[0], mode, 1, p2, display, playerShips, remainRepairs, size, FOCP2);
			else if (mode == 2) botEasyAction(FOCP1, FOCP2, p2, playerShips, remainRepairs, remainShips, size);
			else botHardAction();
			if (remainShips[0]) {
				//debugprintmap(FOCP2, remainShips[1], p2, size);
				MapButtons(1, FOCP2, remainShips[1], mode, 2, p1, display, playerShips, remainRepairs, size, FOCP1); //player 1 targeting player 2
			}
		} while (remainShips[0] > 0 && remainShips[1] > 0);
	}
	drawWhoWon(mode, FOCP1, FOCP2, &music, p1, p2, remainShips, size);
}

//multiplayer game mode.
void multiplayer() {
	int mode = shipInputMode();  // input mode
	if (mode == 1) fileInputMode(&size, 1, p1, p2, FOCP1, FOCP2, playerShips, ho, remainShips, remainRepairs);
	else {
		getMapSize(mode);
		int ships = getShipSpotAmount(mode);
		initiateGame(FOCP1, FOCP2, playerShips, ho, remainShips, size);
		shipLocMode(ships, mode); // for choosing ships location input mode
	}
	Start(mode, p1, p2, &music);
	mode = Randomizer();
	gameCore(mode, 1);
}

//easy bot game mode.
void easybot() {
	int ships;
	int mode = shipInputMode();
	if (mode == 1) {
		ships = fileInputMode(&size, 0, p1, p2, FOCP1, FOCP2, playerShips, ho, remainShips, remainRepairs);
		//during a single player with file input mode bot need to configure them self. see this function for more info.
		initialBotFileMode(size, FOCP2, remainShips, playerShips, ho);
	}
	else {
		getMapSize(mode);
		ships = getShipSpotAmount(mode);
		initiateGame(FOCP1, FOCP2, playerShips, ho, remainShips, size);
		shipLocModeSingle(ships, mode);
	}
	strcpy_s(p2, "Easy Bot");
	botSetLoc(ships, FOCP2, ho, playerShips, size, remainShips);
	Start(mode, p1, p2, &music);
	mode = Randomizer();
	gameCore(mode, 2);
}

//hard bot game mode.
void hardbot() {
	int ships;
	initiateHardBot();
	int mode = shipInputMode();  // input mode
	if (mode == 1) {
		ships = fileInputMode(&size, 0, p1, p2, FOCP1, FOCP2, playerShips, ho, remainShips, remainRepairs);
		//during a single player with file input mode bot need to configure them self. see this function for more info.
		initialBotFileMode(size, FOCP2, remainShips, playerShips, ho);
	}
	else {
		getMapSize(mode);
		ships = getShipSpotAmount(mode);
		initiateGame(FOCP1, FOCP2, playerShips, ho, remainShips, size);
		shipLocModeSingle(ships, mode);
	}
	strcpy_s(p2, "Hard Bot");
	botSetLoc(ships, FOCP2, ho, playerShips, size, remainShips);
	Start(mode, p1, p2, &music);
	mode = Randomizer();
	gameCore(mode, 3);
}

//allegro must be initialized when program start running.
void initiateAllegro() {
	al_init();
	al_install_audio();
	al_init_acodec_addon();
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(1280, 720);
	al_set_window_position(display, 100, 100);
	al_set_window_title(display, "Battleship");
	al_reserve_samples(1);
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_keyboard();
	al_reserve_samples(5);
	playIntro();
	ui = al_load_sample("resources\\sounds\\ui.ogg");
}

//single player menu.
int SinglePlayerMenu() {
	int sw1 = 0;
	al_clear_to_color(al_map_rgb(0, 0, 0));
		int mode1 = singlemenuButtons(ui);
		if (mode1 == 1) easybot();
		else if (mode1 == 2) hardbot();
		else sw1 = 2; //pressing back button will made it to return to main menu.
	return(sw1);
}

//main menu.
int main() {
	srand(time(NULL)); //will let the randomizer function output different possibilities.
	initiateAllegro();
	int sw = 2, mode; //sw: control main menu, if it is 0 program will be closed. 
	int sw1; //sw1: if it true it mean player has clicked back from single player menu.
	int sw2 = 0; //sw2 will keep track of what button menu is on. like if player click guide and come back, they will be on guide button.
	do {
		if (sw == 2) { //first time and when user proceeded to main menu.
			playMainMenuMusic(&music);
			sw = sw1 = 1;
		}
		drawMenu(1);
		mode = menuButtons(sw2+1, display, ui);
		sw2 = 1;
		if (mode == 1) {
			sw1 = SinglePlayerMenu(); //it will bring single playe menu.
			if (sw1 == 0) sw = action(&sw2, display);
			else sw2 = 0; //this mean player pressed back.
		}
		else if (mode == 2) { //multiplayer
			multiplayer();
			sw = action(&sw2, display);
		}
		else if (mode == 3) { //load a saved game
			sw2 = checkForSavedFile(display);
			if (sw2) {
				loadGame();
				resetLoadedGameFile();
				sw = action(&sw2, display);
			}
			else sw2 = 2;
		}
		else if (mode == 4) sw2 = showGameGuide();
		else sw = 0;
	} while (sw);
} //end of code.