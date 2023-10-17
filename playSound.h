int Randomizer();

//every pauses like between a shoot and landing a shoot has came from sound times.

//when a ship is destroyed, this sound will be played
void playBEsound() {
	ALLEGRO_SAMPLE *BE = al_load_sample("resources//sounds//big_explosion.ogg");
	al_play_sample(BE, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_rest(3.0);
}

//when a part of a ship is damaged but the ship is still alive, this sound will be played.
void playSEsound() {
	ALLEGRO_SAMPLE *sfx = al_load_sample("resources//sounds//small_explosion.ogg");
	al_play_sample(sfx, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_rest(2.2);
	al_destroy_sample(sfx);
}

//when a shoot with no error happens, this sound will be played.
void playShootSound() {
	ALLEGRO_SAMPLE *shootsound = al_load_sample("resources//sounds//shoot.ogg");
	al_play_sample(shootsound, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_rest(3.5);
	al_destroy_sample(shootsound);
}

//when a successfull repair happen, this sound will be played.
void playRepairSound() {
	ALLEGRO_SAMPLE *sfx = al_load_sample("resources//sounds//repair.ogg");
	al_play_sample(sfx, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_rest(4.25);
	al_destroy_sample(sfx);
}

//when a shot is missed, this sound will be made.
void playMissedSound() {
	ALLEGRO_SAMPLE *sfx = al_load_sample("resources//sounds//water_splash.ogg");
	al_play_sample(sfx, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	al_rest(2.0);
	al_destroy_sample(sfx);
}

//this function plays the a music which will play during recieving inputs.
void playArrangeMusic(ALLEGRO_SAMPLE **music) {
	al_destroy_sample(*music);
	(*music) = al_load_sample("resources//sounds//arrange.ogg");
	al_play_sample(*music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
}

//there are two musics files for the game. one of them will randomly loop during a game. every music file contain 3 musics.
void playGameMusic(ALLEGRO_SAMPLE **music) {
	al_destroy_sample(*music);
	int i = Randomizer();
	if (i == 1) (*music) = al_load_sample("resources//sounds//game1.ogg");
	else (*music) = al_load_sample("resources//sounds//game2.ogg");
	al_play_sample(*music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
}

//when a player wins against a bot or someone wins in multiplayer, this sound will be played.
void playVictorySound(ALLEGRO_SAMPLE ***music) {
	al_destroy_sample(**music);
	**music = al_load_sample("resources//sounds//victory.ogg");
	al_play_sample(**music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
}

//when a player loses against a bot this sound will be played.
void playDefeatSound(ALLEGRO_SAMPLE ***music) {
	al_destroy_sample(**music);
	**music = al_load_sample("resources//sounds//defeat.ogg");
	al_play_sample(**music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
}

//play main menu music.
void playMainMenuMusic(ALLEGRO_SAMPLE **music) {
	al_destroy_sample(*music);
	*music = al_load_sample("resources\\sounds\\main.ogg");
	al_play_sample(*music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
}