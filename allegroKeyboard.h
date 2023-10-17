void shootNoError(int row, int col, int FOCP[][11], int mode, int player, int pships[]);

//this function is will recieve clicked buttons for repair map.
void repairPart(int FOCP[][11], int pNUM, char name[], int mode, int playerShips[][101], int size, int side, ALLEGRO_DISPLAY *display) {
	drawRepairMap(pNUM, FOCP, name, size);
	drawOverLay(1, 1);
	al_flip_display();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TIMER *t = al_create_timer(0.15);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(t));
	al_start_timer(t);
	int x = 1, y = 1, sw = 0;
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				if (pNUM == 1) drawRepairStats(x, y, FOCP, 1, name, mode, playerShips);
				else drawRepairStats(x, y, FOCP, 2, name, mode, playerShips);
				done = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				sw = showExitDialog(display);
				if (sw == 1) showSaveGameDialog(side, display);
				break;
			default:
				break;
			}
		}
		if (events.type == ALLEGRO_EVENT_TIMER) {
			al_get_keyboard_state(&keyState);
			drawOneBox(y, x, FOCP);
			if (al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
				if (y < size - 1) y++;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_UP)) {
				if (y > 1) y--;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
				if (x > 1) x--;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
				if (x < size - 1) x++;
			}
			sw = 1;
		}
		if (sw) {
			sw = 0;
			drawOverLay(y, x);
			al_flip_display();
		}
	}
	al_destroy_timer(t);
	al_destroy_event_queue(event_queue);
}

//this function will manipulate player inputs for targeting enemy.
void MapButtons(int side, int FOCP[][11], int remaining, int mode, int target, char name[], ALLEGRO_DISPLAY *display, int playerShips[][101], int remainRepairs[], int size, int FOCPturn[][11]) {
	drawMap(side, FOCP, remaining, remainRepairs, name, size);
	drawOverLay(1, 1);
	al_flip_display();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TIMER *t = al_create_timer(0.15);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(t));
	al_start_timer(t);
	int x = 1, y = 1, sw = 0;
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				sw = checkForShootError(x, y, FOCP);
				if (!sw) {
					drawShoot(x, y);
					overlayShootLoc(y, x);
					playShootSound();
					shootNoError(y, x, FOCP, mode, target, playerShips[target - 1]);
				}
				else drawShootError();
				done = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				sw = showExitDialog(display);
				if (sw == 1) showSaveGameDialog(side, display);
				break;
			case ALLEGRO_KEY_R:
				repair(FOCPturn, side, name, mode, playerShips, remainRepairs, size, side, display);
				done = true;
				break;
			default:
				break;
			}
		}
		if (events.type == ALLEGRO_EVENT_TIMER) {
			al_get_keyboard_state(&keyState);
			drawOneBox(y, x, FOCP);
			if (al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
				if (y < size - 1) y++;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_UP)) {
				if (y > 1) y--;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
				if (x > 1) x--;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
				if (x < size - 1) x++;
			}
			sw = 1;
		}
		if (sw) {
			sw = 0;
			drawOverLay(y, x);
			al_flip_display();
		}
	}
	al_destroy_timer(t);
	al_destroy_event_queue(event_queue);
}

//it will manipulate input menu.
int InputMenuSelect(ALLEGRO_SAMPLE *ui) {
	drawInputMenuFirst();
	InputMenuBrightButton(1);
	al_flip_display();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	int x = 1;
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				al_play_sample(ui, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				done = true;
				return(x);
				break;
			case ALLEGRO_KEY_UP:
				if (x > 1) {
					InputMenuButton(x);
					x--;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if (x < 3) {
					InputMenuButton(x);
					x++;
				}
				break;
			default:
				break;
			}
			InputMenuBrightButton(x);
			al_flip_display();
		}
	}
	al_destroy_event_queue(event_queue);
	return(x);
}

//manipulate single menu buttons.
int singlemenuButtons(ALLEGRO_SAMPLE *ui) {
	drawMenu(0);
	drawBrightButton(1, 2);
	al_flip_display();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	int x = 1;
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				al_play_sample(ui, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				done = true;
				return(x);
				break;
			case ALLEGRO_KEY_UP:
				if (x > 1) {
					drawNormalButton(x, 2);
					x--;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if (x < 3) {
					drawNormalButton(x, 2);
					x++;
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				return(3);
			default:
				break;
			}
			drawBrightButton(x, 2);
		}
	}
	al_destroy_event_queue(event_queue);
	return(x);
}

//manipulate main menu buttons.
int menuButtons(int x, ALLEGRO_DISPLAY *display, ALLEGRO_SAMPLE *ui) {
	drawBrightButton(x, 1);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	int sw;
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				al_play_sample(ui, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				done = true;
				return(x);
				break;
			case ALLEGRO_KEY_UP:
				if (x > 1) {
					drawNormalButton(x, 1);
					x--;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if (x < 4) {
					drawNormalButton(x, 1);
					x++;
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				sw = showExitDialog(display);
				if (sw) return (0);
				else break;
			default:
				break;
			}
			drawBrightButton(x, 1);
		}
	}
	al_destroy_event_queue(event_queue);
}

//manipulate guide screen.
int showGameGuide() {
	drawGameGuide();
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	bool done = false;
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_UP) done = true;
	}
	al_destroy_event_queue(event_queue);
	return(3);
}
