void saveGame(int turn);
ALLEGRO_BITMAP *splashScreen; //this is used for intro.

//this function will reset display to the background.
void resetDisplay() {
	ALLEGRO_BITMAP *background = al_load_bitmap("resources//images//bg.bmp");
	//clearscreen(); //there is a thing when loading background if it is a png file, it is like a warning. it won't happen if its a bitmap format. it will be printed in console, i got rid of it.
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(background, 0, 0, NULL);
	al_destroy_bitmap(background);
}
//when a shoot with no error happens, this function will print a orange square on the target.
void overlayShootLoc(int x, int y) {
	ALLEGRO_BITMAP *orangeBox = al_load_bitmap("resources//images//boxOrange.png");
	al_draw_bitmap(orangeBox, (y * 48) + 13, (x * 48) + 185, NULL);
	al_flip_display();
	al_destroy_bitmap(orangeBox);
}

//before a game starts, it will show who is vs who.
void drawIntoGameText(char p1[], char p2[]) {
	resetDisplay();
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 90, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), 640, 100, ALLEGRO_ALIGN_CENTER, "WELCOME TO BATTLESHIP!");
	al_flip_display();
	al_rest(0.5);
	al_draw_text(font, al_map_rgb(0, 0, 255), 640, 300, ALLEGRO_ALIGN_CENTER, p1);
	al_flip_display();
	al_rest(0.5);
	al_draw_text(font, al_map_rgb(255, 255, 255), 640, 410, ALLEGRO_ALIGN_CENTER, "VS");
	al_flip_display();
	al_rest(0.5);
	al_draw_text(font, al_map_rgb(255, 0, 0), 640, 500, ALLEGRO_ALIGN_CENTER, p2);
	al_flip_display();
	al_rest(2.0);
	al_destroy_font(font);
}

//this function will draw the numbers that indicate row/column number for display.
void drawNumbers(int size) {
	int i;
	ALLEGRO_BITMAP *box = al_load_bitmap("resources//images//num//1.png");
	for (i = 1; i < size; i++) {
		al_draw_bitmap(box, (i * 48) + 12, 185, NULL);
		al_draw_bitmap(box, 12, (i * 48) + 185, NULL);
		if (i == 1) box = al_load_bitmap("resources//images//num//2.png");
		else if (i == 2) box = al_load_bitmap("resources//images//num//3.png");
		else if (i == 3) box = al_load_bitmap("resources//images//num//4.png");
		else if (i == 4) box = al_load_bitmap("resources//images//num//5.png");
		else if (i == 5) box = al_load_bitmap("resources//images//num//6.png");
		else if (i == 6) box = al_load_bitmap("resources//images//num//7.png");
		else if (i == 7) box = al_load_bitmap("resources//images//num//8.png");
		else if (i == 8) box = al_load_bitmap("resources//images//num//9.png");
		else box = al_load_bitmap("resources//images//num//10.png");
	}
	al_destroy_bitmap(box);
}

//this function draw a white square which will show player's current target in the display.
void drawOverLay(int i, int j) {
	ALLEGRO_BITMAP *overlay = al_load_bitmap("resources//images//boxOverlay.png");
	al_draw_bitmap(overlay, (j * 48) + 13, (i * 48) + 185, NULL);
	al_destroy_bitmap(overlay);
}

//this function will draw player map in display as boxes.
void drawPlayerMap(int FOCP[][11], int size) {
	ALLEGRO_BITMAP *box = al_load_bitmap("resources//images//box.png");
	ALLEGRO_BITMAP *dabox = al_load_bitmap("resources//images//boxdamaged.png");
	ALLEGRO_BITMAP *debox = al_load_bitmap("resources//images//boxdestroyed.png");
	int i, j;
	for (i = 1; i < size; i++) {
		for (j = 1; j < size; j++) {
			if (FOCP[i][j] == -101) al_draw_bitmap(debox, (j * 48) + 13, (i * 48) + 185, NULL);
			else if (FOCP[i][j] < 0) al_draw_bitmap(dabox, (j * 48) + 13, (i * 48) + 185, NULL);
			else al_draw_bitmap(box, (j * 48) + 13, (i * 48) + 185, NULL);
		}
	}
	al_destroy_bitmap(box);
	al_destroy_bitmap(dabox);
	al_destroy_bitmap(debox);
}

//simply draw a text to say who turn is.
void drawWhoTurnIs(char name[]) {
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 70, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 0, ALLEGRO_ALIGN_LEFT, "Turn for");
	al_draw_text(font, al_map_rgb(255, 255, 255), 280, 0, ALLEGRO_ALIGN_LEFT, name);
	al_destroy_font(font);
}

//this function will draw needed information text for players map durning targeting someone.
void drawMap(int side, int FOCP[][11], int remaining, int remainRepairs[], char name[], int size) {
	char nameofp[5], Rrepairs[10];
	intToA(nameofp, remaining);
	intToA(Rrepairs, remainRepairs[side - 1]);
	resetDisplay();
	drawWhoTurnIs(name);
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 70, ALLEGRO_ALIGN_LEFT, "Remaining repairs:");
	al_draw_text(font, al_map_rgb(255, 255, 255), 310, 70, ALLEGRO_ALIGN_LEFT, Rrepairs);
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 105, ALLEGRO_ALIGN_LEFT, "Opponent's Map:");
	al_draw_text(font, al_map_rgb(255, 255, 255), 10, 140, ALLEGRO_ALIGN_LEFT, "Remaining ships:");
	al_draw_text(font, al_map_rgb(255, 255, 255), 280, 140, ALLEGRO_ALIGN_LEFT, nameofp);
	drawNumbers(size);
	drawPlayerMap(FOCP, size);
	al_destroy_font(font);
}
//just draw two big text...
void drawRepairInProgress(char name[]) {
	resetDisplay();
	drawWhoTurnIs(name);
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 120, NULL);
	al_draw_text(font, al_map_rgb(193, 156, 4), 640, 260, ALLEGRO_ALIGN_CENTER, "Repair in Progress...");
	al_flip_display();
	al_destroy_font(font);
}

//some text about how it is the repairing thing with the given location.
void drawRepairStats(int col, int row, int FOCP[][11], int pNUM, char name[], int mode, int playerShips[][101]) {
	int sw = 1;
	ALLEGRO_FONT *font1 = al_load_font("resources//fonts//BRLNSDB.TTF", 70, NULL);
	if (FOCP[row][col] < 0 && FOCP[row][col] > -101) {
		drawRepairInProgress(name);
		repairShip(col, row, FOCP, pNUM, mode, playerShips);
		al_draw_text(font1, al_map_rgb(0, 225, 4), 640, 380, ALLEGRO_ALIGN_CENTER, "a ship part has been repaired!");
		sw = 0;
	}
	else if (FOCP[row][col] == -101) al_draw_text(font1, al_map_rgb(255, 0, 0), 14, 114, ALLEGRO_ALIGN_LEFT, "Can't repair a dead ship!");
	else al_draw_text(font1, al_map_rgb(255, 0, 0), 14, 114, ALLEGRO_ALIGN_LEFT, "This is not a damaged ship location!");
	if (sw) printf("\a");
	al_flip_display();
	al_rest(2.0);
	al_destroy_font(font1);
}

//when a player want to use a repair, they need their map, this function will do that.
void drawRepairMap(int side, int FOCP[][11], char name[], int size) {
	resetDisplay();
	drawWhoTurnIs(name);
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	drawNumbers(size);
	drawPlayerMap(FOCP, size);
	al_destroy_font(font);
}

//when the overlay box moves, the last box need to be updated, this function will do that.
void drawOneBox(int i, int j, int FOCP[][11]) {
	ALLEGRO_BITMAP *box = al_load_bitmap("resources//images//box.png");
	al_draw_bitmap(box, (j * 48) + 13, (i * 48) + 185, NULL);
	al_destroy_bitmap(box);
}

//when repair has an error, this function will draw that. with quite a big size.
void drawRepairError(int mode) {
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	if (mode == 1) al_draw_text(font, al_map_rgb(255, 0, 0), 376, 70, ALLEGRO_ALIGN_LEFT, "No Repairs remaining, skipping your turn");
	else {
		al_draw_text(font, al_map_rgb(255, 0, 0), 376, 70, ALLEGRO_ALIGN_LEFT, "No damaged ships are in your map.");
		al_draw_text(font, al_map_rgb(255, 0, 0), 376, 105, ALLEGRO_ALIGN_LEFT, "lost one repair regardless.");
		al_draw_text(font, al_map_rgb(255, 0, 0), 376, 140, ALLEGRO_ALIGN_LEFT, "Skipping your turn.");
	}
	printf("\a");
	al_flip_display();
	al_rest(2.0);
	al_destroy_font(font);
}

//when shot hit a ship, this text will be drawn.
void drawHit(int mode) {
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(255, 182, 0), 376, 105, ALLEGRO_ALIGN_LEFT, "Hit!");
	al_flip_display();
	if (mode) playSEsound();
	al_destroy_font(font);
}

//when a ship is destroy, this text will be drawn.
void drawDestroyed(int player, int temp, int FOCP[][11], int remainShips[], int size) {
	drawHit(0);
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(0, 201, 24), 376, 140, ALLEGRO_ALIGN_LEFT, "a ship has been destroyed");
	al_flip_display();
	kill(player, temp, FOCP, remainShips, size);
	al_destroy_font(font);
}

//if a player stupidly shoot at a already damaged location!
void drawShootError() {
	ALLEGRO_FONT *font2 = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font2, al_map_rgb(255, 0, 0), 350, 70, ALLEGRO_ALIGN_LEFT, "This spot is already damaged!");
	printf("\a");
	al_flip_display();
	al_destroy_font(font2);
	al_rest(2.0);
}

//when a shot is missed, this message will be drown.
void drawMissed() {
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(255, 0, 0), 376, 105, ALLEGRO_ALIGN_LEFT, "Missed!");
	al_flip_display();
	playMissedSound();
	al_destroy_font(font);
}

//when shooting, a text will be drown to say where is the location, is also used for bots.
void drawShoot(int x, int y) {
	char target[6];
	intToA(target, x);
	addNumToStr(target, y);
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(255, 232, 140), 376, 70, ALLEGRO_ALIGN_LEFT, "SHOOT AT:");
	al_draw_text(font, al_map_rgb(255, 232, 140), 550, 70, ALLEGRO_ALIGN_LEFT, target);
	al_flip_display();
	al_destroy_font(font);
}

//when a game finished and we have a winner, this function print winner player map so loser can have a look.
void drawFinalMap(int FOCP[][11], int size) {
	ALLEGRO_BITMAP *box = al_load_bitmap("resources//images//box.png");
	ALLEGRO_BITMAP *dabox = al_load_bitmap("resources//images//boxdamaged.png");
	ALLEGRO_BITMAP *debox = al_load_bitmap("resources//images//boxdestroyed.png");
	ALLEGRO_BITMAP *sbox = al_load_bitmap("resources//images//boxship.png");
	drawNumbers(size);
	int i, j;
	for (i = 1; i < size; i++) {
		for (j = 1; j < size; j++) {
			if (FOCP[i][j] == -101) al_draw_bitmap(debox, (j * 48) + 13, (i * 48) + 185, NULL);
			else if (FOCP[i][j] < 0) al_draw_bitmap(dabox, (j * 48) + 13, (i * 48) + 185, NULL);
			else if (FOCP[i][j] == 0 || FOCP[i][j] == 101) al_draw_bitmap(box, (j * 48) + 13, (i * 48) + 185, NULL);
			else al_draw_bitmap(sbox, (j * 48) + 13, (i * 48) + 185, NULL);
		}
	}
	al_destroy_bitmap(box);
	al_destroy_bitmap(dabox);
	al_destroy_bitmap(debox);
	al_destroy_bitmap(sbox);
}

//literally.
void drawBotUsedRepair() {
	playRepairSound();
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 35, NULL);
	al_draw_text(font, al_map_rgb(255, 0, 0), 376, 70, ALLEGRO_ALIGN_LEFT, "Bot Used a Repair!");
	al_flip_display();
	al_destroy_font(font);
	al_rest(1.5);
}

//it will draw input texts.
void InputMenuText(int count) {
	ALLEGRO_BITMAP *text;
	if (count == 1) text = al_load_bitmap("resources//images//FM.png");
	else if (count == 2) text = al_load_bitmap("resources//images//MM.png");
	else text = al_load_bitmap("resources//images//PW.png");
	al_draw_bitmap(text, 503, (count * 177) + 41, NULL);
	al_destroy_bitmap(text);
}

//show currently selected button in this menu.
void InputMenuBrightButton(int count) {
	ALLEGRO_BITMAP *img = al_load_bitmap("resources//images//b3.png");
	al_draw_bitmap(img, 503, (count * 177) + 41, NULL);
	InputMenuText(count);
	al_destroy_bitmap(img);
}

//draw the given number button in input menu, also it used to renew a button which was previously selected.
void InputMenuButton(int count) {
	ALLEGRO_BITMAP *img = al_load_bitmap("resources//images//b1.png");
	al_draw_bitmap(img, 503, (count * 177) + 41, NULL);
	InputMenuText(count);
	al_destroy_bitmap(img);
}

//when entering input menu, since most of those images does'nt need update, it will be drown once.
void drawInputMenuFirst() {
	resetDisplay();
	ALLEGRO_BITMAP *img = al_load_bitmap("resources//images//CIM.png");
	al_draw_bitmap(img, 116, 75, NULL);
	for (int i = 1; i < 4; i++) InputMenuButton(i);
	img = al_load_bitmap("resources//images//fileMode.png");
	al_draw_bitmap(img, 802, 175, NULL);
	img = al_load_bitmap("resources//images//projectWay.png");
	al_draw_bitmap(img, 762, 529, NULL);
	img = al_load_bitmap("resources//images//manualMode.png");
	al_draw_bitmap(img, 194, 352, NULL);
	al_destroy_bitmap(img);
}

//this function draw a simple tip to tell player enter inputs in console.
void showToUseConsole() {
	resetDisplay();
	ALLEGRO_BITMAP *img = al_load_bitmap("resources//images//tipboard.png");
	al_draw_bitmap(img, 147, 72, NULL);
	img = al_load_bitmap("resources//images//inputGuide.png");
	al_draw_bitmap(img, 263, 304, NULL);
	al_flip_display();
	al_destroy_bitmap(img);
}

//this function recieve the name of starter of each game and print it.
void printWhoStarts(char name[16]) {
	resetDisplay();
	ALLEGRO_FONT *font = al_load_font("resources//fonts//BRLNSDB.TTF", 90, NULL);
	al_draw_text(font, al_map_rgb(255, 255, 255), 640, 300, ALLEGRO_ALIGN_CENTER, name);
	al_draw_text(font, al_map_rgb(255, 255, 255), 640, 400, ALLEGRO_ALIGN_CENTER, "WILL START THE GAME!");
	al_flip_display();
	clearscreen();
	al_destroy_font(font);
	al_rest(3.0);
}

void drawWhoWonMultiplayer(char name[]) {
	ALLEGRO_FONT *font4 = al_load_font("resources//fonts//BRLNSDB.TTF", 60, NULL);
	al_draw_text(font4, al_map_rgb(55, 182, 16), 8, 8, ALLEGRO_ALIGN_LEFT, "Winner is:");
	al_draw_text(font4, al_map_rgb(55, 182, 16), 280, 8, ALLEGRO_ALIGN_LEFT, name);
	al_draw_text(font4, al_map_rgb(55, 182, 16), 8, 68, ALLEGRO_ALIGN_LEFT, "Winner's map:");
	al_destroy_font(font4);
}

//after a game is done, this function will be called to draw who won.
void drawWhoWon(int mode, int FOCP1[][11], int FOCP2[][11], ALLEGRO_SAMPLE **music, char p1[], char p2[], int remainShips[], int size) {
	resetDisplay();
	ALLEGRO_FONT *font3 = al_load_font("resources//fonts//BRLNSDB.TTF", 60, NULL);
	if (remainShips[0] == 0) {
		if (mode != 1) {
			al_draw_text(font3, al_map_rgb(255, 0, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "You lost");
			al_draw_text(font3, al_map_rgb(255, 0, 0), 8, 68, ALLEGRO_ALIGN_LEFT, "bot map:");
			playDefeatSound(&music);
		}
		else {
			drawWhoWonMultiplayer(p2);
			playVictorySound(&music);
		}
		drawFinalMap(FOCP2, size);
	}
	else {
		playVictorySound(&music);
		if (mode != 1) {
			al_draw_text(font3, al_map_rgb(55, 182, 16), 8, 8, ALLEGRO_ALIGN_LEFT, "You Won");
			al_draw_text(font3, al_map_rgb(55, 182, 16), 8, 68, ALLEGRO_ALIGN_LEFT, "Winner's map:");
			al_flip_display();
		}
		else drawWhoWonMultiplayer(p1);
		drawFinalMap(FOCP1, size);
	}
	al_flip_display();
	al_destroy_font(font3);
}

//draw texts for main menu buttons
void drawMainMenuText(int num) {
	ALLEGRO_BITMAP *text;
	if (num == 1) text = al_load_bitmap("resources\\images\\sp.png");
	else if (num == 2) text = al_load_bitmap("resources\\images\\mp.png");
	else if (num == 3) text = al_load_bitmap("resources\\images\\lg.png");
	else text = al_load_bitmap("resources\\images\\guideb.png");
	al_draw_bitmap(text, 503, (num * 107) + 191, NULL);
	al_destroy_bitmap(text);
}

//draw texts for single player menu buttons
void drawSingleMenuText(int num) {
	ALLEGRO_BITMAP *text;
	if (num == 1) text = al_load_bitmap("resources\\images\\easy.png");
	else if (num == 2) text = al_load_bitmap("resources\\images\\hard.png");
	else text = al_load_bitmap("resources\\images\\back.png");
	al_draw_bitmap(text, 503, (num * 131) + 197, NULL);
	al_destroy_bitmap(text);
}

//this function will draw a brightened button to show that curretly that button is selected. used for both single and main menu.
void drawBrightButton(int num, int mode) {
	ALLEGRO_BITMAP *buttons = al_load_bitmap("resources\\images\\b3.png");
	if (mode == 1) {
		al_draw_bitmap(buttons, 503, (num * 107) + 191, NULL);
		drawMainMenuText(num);
	}
	else {
		al_draw_bitmap(buttons, 503, (num * 131) + 197, NULL);
		drawSingleMenuText(num);
	}
	al_flip_display();
	al_destroy_bitmap(buttons);
}

//this function will draw button for both single and main menu.
void drawNormalButton(int num, int mode) {
	ALLEGRO_BITMAP *buttons = al_load_bitmap("resources\\images\\b1.png");
	if (mode == 1) {
		al_draw_bitmap(buttons, 503, (num * 107) + 191, NULL);
		drawMainMenuText(num);
	}
	else {
		al_draw_bitmap(buttons, 503, (num * 131) + 197, NULL);
		drawSingleMenuText(num);
	}
	al_destroy_bitmap(buttons);
}

//it will draw both main and 
void drawMenu(int mode) {
	ALLEGRO_BITMAP *logo = al_load_bitmap("resources\\images\\logo.png");
	resetDisplay();
	al_draw_bitmap(logo, 495, 12, NULL);
	int i;
	if (mode == 1) for (i = 1; i < 5; i++) drawNormalButton(i, 1);
	else for (i = 1; i < 4; i++) drawNormalButton(i, 2);
	al_destroy_bitmap(logo);
}

//it will show the game guide picture.
void drawGameGuide() {
	ALLEGRO_BITMAP *guide = al_load_bitmap("resources\\images\\guide.png");
	al_draw_bitmap(guide, 0, 0, NULL);
	al_flip_display();
	al_destroy_bitmap(guide);
}

//after clicking yes for exiting mid game, the second message box will appear to ask the user if they want to save their game.
void showSaveGameDialog(int turn, ALLEGRO_DISPLAY *display) {
	int ans = al_show_native_message_box(display, "Native Message box", "Save The Current game?", "choose your action", NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION);
	if (ans == 1) saveGame(turn);
	else exit(0);
}

//with pressing esc mid game, a box will appear and ask the user for exiting. 
int showExitDialog(ALLEGRO_DISPLAY *display) {
	int ans = al_show_native_message_box(display, "Native Message box", "Exit?", "choose your action", NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION);
	if (ans == 1) return(1);
	return(0);
}

//a fade out effect for intro.
void blackFadeout() {
	ALLEGRO_BITMAP *black = al_load_bitmap("resources\\images\\tintedBlack.png");
	for (int j = 0; j < 20; j++)
	{
		al_draw_bitmap(black, 0, 0, NULL);
		al_flip_display();
		al_rest(0.025);
		if (_kbhit()) return;
	}
	al_destroy_bitmap(black);
}

//a fade in effect for intro.
void fadein() {
	for (int j = 0; j < 20; j++)
	{
		al_draw_bitmap(splashScreen, 0, 0, NULL);
		al_flip_display();
		al_rest(0.05);
		if (_kbhit()) return;
	}
}

//it will draw a black for whole display.
void blackScreen(int mode) {
	al_destroy_bitmap(splashScreen);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	splashScreen = al_load_bitmap("resources\\images\\black.png");
	al_draw_bitmap(splashScreen, 0, 0, NULL);
	if (mode) al_flip_display();
}

//this function draw the given splashscreen image.
int IntroFazes(int faze) {
	blackFadeout();
	al_destroy_bitmap(splashScreen);
	if (faze == 1) {
		splashScreen = al_load_bitmap("resources\\images\\2.png");
		clearscreen(); //there is something wrong with this picture i could not figure it out, it will print like a error in the console, so getting rid of it.
	}
	else if (faze == 2) splashScreen = al_load_bitmap("resources\\images\\3.png");
	else splashScreen = al_load_bitmap("resources\\images\\4.png");
	fadein();
	return(faze + 1);
}

//this function control intro images. i thought doing this would be really cool. also i made it skippable some how.
int intro() {
	int sw = 1;
	splashScreen = al_load_bitmap("resources\\images\\1.png");
	fadein();
	clock_t goal1 = 2875 + clock();
	clock_t goal2 = 7258 + clock();
	clock_t goal3 = 11618 + clock();
	clock_t goal4 = 16120 + clock();
	do
	{
		if (_kbhit()) {
			sw = _getch();
			return(0);
		}
		else if (clock() > goal4) sw = 0;
		else if (clock() > goal1 && sw == 1) sw = IntroFazes(sw);
		else if (clock() > goal2 && sw == 2) sw = IntroFazes(sw);
		else if (clock() > goal3 && sw == 3) sw = IntroFazes(sw);
	} while (sw);
	return(1);
}

//it will play intro music.
void playIntro() {
	ALLEGRO_SAMPLE *music1; //this is only for intro music.
	music1 = al_load_sample("resources\\sounds\\intro.ogg");
	al_play_sample(music1, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	int sw = intro();
	al_rest(0.02);
	if (!sw) al_destroy_sample(music1);
	blackScreen(0);
}