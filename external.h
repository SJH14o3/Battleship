/*for colorful printing. also constant variables if we changed our minds for a color.
to be fair variable names doesn't tell if we are going to change our minds*/
const int white = 15; //we prefer this brighter white than #7 which is default.
const int red = 4;

//color text function. it will change the color of charactors in console.
void setTextColor(int textColor, int backColor) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}

//console screen cleaner.
void clearscreen() {
	system("cls");
}

//console will printed in white color
void printWhite() {
	setTextColor(white, 0);
}

//console will printed in red color
void printRed() {
	setTextColor(red, 0);
}

//this function simply swap two variables.
void swap(int *a, int *b) {
	int temp; //temporarely.
	temp = (*a);
	(*a) = (*b);
	(*b) = temp;
}