/*
TEAM FRANKLIN
*/
#include "Game.h"
#include "Bitmap.h"

//  variable for triggering start screen
static bool start = true;
static bool gameOver = false;



//  GAME CONSTRUCTOR
Game::Game( HWND hWnd,const KeyboardServer& kServer )
:	gfx ( hWnd ),
	kbd( kServer )
{
	srand(time(NULL));//  essential for random number generation
	p.setX(400);//  starting location
	p.setY(400);//  starting location
	p.setState(rand() % 3);// starting shape
	p.setScore(0);// starting score
	p.setShapeW(10);// starting shape size (width)
	p.setWave(1);
	numOfShapes = 12;
	InitializeSPtr();
	PlaySound(L"mint.WAV", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	getchar();
}


// Gets called every step
void Game::Go()
{
	gfx.BeginFrame();// starts frame
	if (gameOver == false) {
		// if start == true, wait at start screen. Else, begin to start the game
		if (start == true)
			StartScreen();
		else {
			CheckWaveOver();//Check for a finished wave
			ComposeFrame();// Draw each frame
			DrawShapes();// Draw each shape
			MoveShapes();// Update each shapes location
			CheckGameOver();// check for end of game
		}
	}
	else {
		Sleep(3000);// Pause for 3 seconds
		EndScreen();// Show end game
	}
	gfx.EndFrame();// ends frame
}

// Draws each frame
void Game::ComposeFrame()
{
	// Loads image for making text
	LoadFont(&fixedSys, fontSurf, "Fixedsys16x28.bmp", 16, 28, 32);

	DrawScore();
	DrawWave();

	// Player movement speed
	int PSPEED;
	if (p.getShapeW() <= 20)
		PSPEED = 5;
	else if (p.getShapeW() <= 35)
		PSPEED = 4;
	else if (p.getShapeW() <= 50)
		PSPEED = 3;
	else
		PSPEED = 2;

	// Movement Left
	if (kbd.LeftIsPressed())
		p.setX(p.getX() - PSPEED);
	// Movement Right
	if (kbd.RightIsPressed())
		p.setX(p.getX() + PSPEED);
	// Movement Up
	if (kbd.UpIsPressed())
		p.setY(p.getY() - PSPEED); 
	// Movement Down
	if (kbd.DownIsPressed())
		p.setY(p.getY() + PSPEED);
	
	// Keep Player In Game Window
	if (p.getX() < 0)
		p.setX(0);
	if (p.getY() < 0)
		p.setY(0);
	if (p.getX() > 800 - p.getShapeW() - 1)
		p.setX(800 - p.getShapeW() - 1);
	if (p.getY() > 600 - p.getShapeW() - 1)
		p.setY(600 - p.getShapeW() - 1);
	
	//COLLISION CHECKING
	CheckCollision();	
}

// Draw each shape
void Game::DrawShapes() {

	// if player is close enough to the edge where growing would draw the player of the screen,
	// reset the x and v values of the player to a safe location
	if (p.getX() + p.getShapeW() + 1 >= 800 - 1)
		p.setX(p.getX() - 1);
	if (p.getY() + p.getShapeW() + 1 >= 600 - 1)
		p.setY(p.getY() - 1);

	DrawPlayer(p.getX(), p.getY());// Draws the player
	
	// Draws each object in sPtr array
	for (int i = 0; i < numOfShapes; i++) {
		if (sPtr[i]->doesShExist())
			DrawObj(sPtr[i]);
	} 
}

// Draws player
void Game::DrawPlayer(int x, int y)
{
	if (p.getState() == 0)
		DrawPlayerCir(x, y);
	if (p.getState() == 1)
		DrawPlayerTri(x, y);
	if (p.getState() == 2)
		DrawPlayerSqr(x, y);
}

// Draws an object
void Game::DrawObj(IShape * s)
{
	if (s->isCirc)// Draw as circle
		DrawObjCir(s->getX(), s->getY());
	if (s->isTri)// Draw as triangle
		DrawObjTri(s->getX(), s->getY());
	if (s->isSqr)// Draw as square
		DrawObjSqr(s->getX(), s->getY());
}

// Update each objects location
void Game::MoveShapes() {
	for (int i = 0; i < numOfShapes; i++) {
		// Update location with velocity
		sPtr[i]->setX(sPtr[i]->getX() + sPtr[i]->getXV());
		sPtr[i]->setY(sPtr[i]->getY() + sPtr[i]->getYV());
		
		// Hard coded wall collision detection
		// Will change an objects velocity when they hit a wall
		// Approaching Left
		if (sPtr[i]->getX() < 0) {
			sPtr[i]->setX(0);
			sPtr[i]->updateVel(0);
		}
		// Approaching Top
		if (sPtr[i]->getY() < 0) {
			sPtr[i]->setY(0);
			sPtr[i]->updateVel(1);
		}
		//Approaching Right
		if (sPtr[i]->getX() > 789) {
			sPtr[i]->setX(789);
			sPtr[i]->updateVel(2);
		}
		//Approaching Bottom
		if (sPtr[i]->getY() > 589) {
			sPtr[i]->setY(589);
			sPtr[i]->updateVel(3);
		}
	}
}

// Checks for collision between an IShape and a Player
bool Game::doesCollide(IShape * s, Player p) {
	return (p.getX() + p.getShapeW() > s->getX() &&
				p.getX() < s->getX() + 10 &&
					p.getY() + p.getShapeW() > s->getY() &&
						p.getY() < s->getY() + 10);
}

// Initialize sPtr
void Game::InitializeSPtr() {
	for (int i = 0; i < numOfShapes; i+=3) {
		sPtr[i] = new Circle();
		sPtr[i + 1] = new Triangle();
		sPtr[i + 2] = new Square();
	}
}

// Check for the end of the game
void Game::CheckGameOver() {
	if (p.getShapeW() < 7) {
		gameOver = true;
		for (int i = 0; i < (numOfShapes); i++) {
			delete sPtr[i];
		}
		delete sPtr;
	}
}

// Check for end of wave
void Game::CheckWaveOver() {

	if (numOfShapes == 0) {
		p.setX(400);
		p.setY(400);
		numOfShapes = 9 + (3 * p.getWave());
		NextWaveShapeInitialize();
		LoadingScreenNextWave();
		// add score bonus
		if (p.getWave() > 9)
			p.setScore(p.getScore() + 100);
		else
			p.setScore(p.getScore() + ((p.getWave() - 1) * 10));
	}
}

//initialize shapes for next wave
void Game::NextWaveShapeInitialize() {	
	for (int i = 0; i < numOfShapes; i+=3) {
		sPtr[i] = new Circle();
		sPtr[i + 1] = new Triangle();
		sPtr[i + 2] = new Square();
	}
}

// COLLISION CHECKING
/*Takes each element of the sPtr array and checks f the Player is colliding with it.
*If so, update score, change the player state, update player size, set the object to
*not existing, and relocate the object */
void Game::CheckCollision() {
	for (int i = 0; i < numOfShapes; i++) {
		if (doesCollide(sPtr[i], p) && sPtr[i]->doesShExist() == true) {
			// update score
			if ((p.getState() == 0 && sPtr[i]->isCirc) ||
				(p.getState() == 1 && sPtr[i]->isTri) ||
				(p.getState() == 2 && sPtr[i]->isSqr)) {
				p.setScore(p.getScore() + 2);// update score
				p.setShapeW(p.getShapeW() + 2);// update player size
			}
			else {
				p.setScore(p.getScore() - 1);// update score
				p.setShapeW(p.getShapeW() - 2);// update player size
			}
			p.newState();// change player shape
			
			//Remove object from array list and slide everything down
			sPtr[i]->setExist(false);// set object shape to non existant
			delete sPtr[i];
			for (int j = i; j < numOfShapes; j++) {
				sPtr[j] = sPtr[j + 1];
			}
			numOfShapes--;

			CheckCollision();
			//MakeNewObj(sPtr[i]);// reloacte shape
		}
	}	
}

// Draws a circle
void Game::DrawObjCir(int x, int y)
{
	for (int i = 3; i <= 6; i++) {
		gfx.PutPixel(x + i, y, 0, 255, 0);
		gfx.PutPixel(x, y + i, 0, 255, 0);
		gfx.PutPixel(x + 9, y + i, 0, 255, 0);
		gfx.PutPixel(x + i, y + 9, 0, 255, 0);
	}
	gfx.PutPixel(x + 2, y + 1, 0, 255, 0);
	gfx.PutPixel(x + 2, y + 8, 0, 255, 0);
	gfx.PutPixel(x + 1, y + 2, 0, 255, 0);
	gfx.PutPixel(x + 1, y + 7, 0, 255, 0);

	gfx.PutPixel(x + 7, y + 1, 0, 255, 0);
	gfx.PutPixel(x + 8, y + 2, 0, 255, 0);
	gfx.PutPixel(x + 7, y + 8, 0, 255, 0);
	gfx.PutPixel(x + 8, y + 7, 0, 255, 0);
}

// Draws a triangle
void Game::DrawObjTri(int x, int y)
{
	int j = 11;
	for (int i = 0; i <= 5; i++) {
		int k = 2;
		while (k > 0) {
			gfx.PutPixel(x + i, y + j, 255, 0, 0);
			gfx.PutPixel(x + i, y + j - 1, 255, 0, 0);
			k--;
		}
		j -= 2;
	}

	j = 11;
	for (int i = 10; i >= 5; i--) {
		int k = 2;
		while (k > 0) {
			gfx.PutPixel(x + i, y + j, 255, 0, 0);
			gfx.PutPixel(x + i, y + j - 1, 255, 0, 0);
			k--;
		}
		j -= 2;
	}

	for (int i = 0; i <= 10; i++)
		gfx.PutPixel(x + i, y + 11, 255, 0, 0);
}

// Draws a square
void Game::DrawObjSqr(int x, int y) {
	for (int i = 0; i <= 10; i++) {
		gfx.PutPixel(x + i, y, 70, 250, 200);
		gfx.PutPixel(x + 10, y + i, 70, 250, 200);
		gfx.PutPixel(x + i, y + 10, 70, 250, 200);
		gfx.PutPixel(x, y + i, 70, 250, 200);
	}
}

//  Draws player circle
void Game::DrawPlayerCir(int x, int y)
{
	int xi = x;
	int yi = y;
	double radius = (p.getShapeW() - 4) / 2;

	for (double i = 0.0; i < 360.0; i += 0.1)
	{
		double angle = i * PI / 180;
		int xi = (int)(x + radius * cos(angle));
		int yi = (int)(y + radius * sin(angle));
		gfx.PutPixel(xi + radius + 2, yi + radius + 2, 0, 255, 0);
	}

	for (int i = 0; i <= p.getShapeW(); i++) {
		gfx.PutPixel(x + i, y, 255, 255, 255);
		gfx.PutPixel(x + p.getShapeW(), y + i, 255, 255, 255);
		gfx.PutPixel(x + i, y + p.getShapeW(), 255, 255, 255);
		gfx.PutPixel(x, y + i, 255, 255, 255);
	}
}

//  Draws player triangle
void Game::DrawPlayerTri(int x, int y)
{
	int r = 0;
	for (int i = 0; i <= p.getShapeW() - 4; i++)
		gfx.PutPixel(x + i, y + p.getShapeW() - 4 + 2, 255, 0, 0);
	for (int i = p.getShapeW() - 4; i > 0; i -= 2) {
		for (int k = 0; k < p.getShapeW() - 4; k++) {
			for (int j = 0; j < 2; j++) {
				if (k == 0)
					gfx.PutPixel(x + k + r + 2, y + i - j + 2, 255, 0, 0);
				if (k == p.getShapeW() - 1 - 4)
					gfx.PutPixel(x + k - r + 2, y + i - j + 2, 255, 0, 0);
			}
		}
		r++;
	}
	for (int i = 0; i <= p.getShapeW(); i++) {
		gfx.PutPixel(x + i, y, 255, 255, 255);
		gfx.PutPixel(x + p.getShapeW(), y + i, 255, 255, 255);
		gfx.PutPixel(x + i, y + p.getShapeW(), 255, 255, 255);
		gfx.PutPixel(x, y + i, 255, 255, 255);
	}
}

//  Draws player square
void Game::DrawPlayerSqr(int x, int y) {
	for (int i = 0; i <= p.getShapeW(); i++) {
		gfx.PutPixel(x + i, y, 255, 255, 255);
		gfx.PutPixel(x + p.getShapeW(), y + i, 255, 255, 255);
		gfx.PutPixel(x + i, y + p.getShapeW(), 255, 255, 255);
		gfx.PutPixel(x, y + i, 255, 255, 255);
	}
	for (int i = 0; i <= (p.getShapeW() - 8); i++) {
		gfx.PutPixel(x + i + 4, y + 3, 70, 250, 200);
		gfx.PutPixel(x + (p.getShapeW() - 4), y + i + 3, 70, 250, 200);
		gfx.PutPixel(x + i + 4, y + (p.getShapeW() - 4), 70, 250, 200);
		gfx.PutPixel(x + 4, y + i + 3, 70, 250, 200);
	}
}

// Creates the start screen and initializes sPtr
void Game::StartScreen() {

	// TITLE SCREEN TEXT
	gfx.DrawChar('P', 240, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('R', 256, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 272, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('S', 288, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('S', 304, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 320, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('S', 336, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('P', 352, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 368, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('C', 384, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 400, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 416, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('T', 432, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('O', 448, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 464, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('P', 480, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('L', 496, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 512, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('Y', 528, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('[', 320, 326, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(']', 448, 326, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	
	
	gfx.DrawChar('S', 304, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('H', 320, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 336, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('P', 352, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 368, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 384, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('S', 400, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('M', 416, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 432, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('C', 448, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('K', 464, 140, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	
	// END TITLE SCREEN TEXT

	//Circle
	int ax = 240;
	int ay = 190;
	int xi = ax;
	int yi = ay;
	double radius = 32;

	for (double i = 0.0; i < 360.0; i += 0.1)
	{
		double angle = i * PI / 180;
		int xi = (int)(ax + radius * cos(angle));
		int yi = (int)(ay + radius * sin(angle));
		gfx.PutPixel(xi + radius + 2, yi + radius + 2, 0, 255, 0);
	}

	//Square
	int bx = 480;
	int by = 190;
	for (int i = 0; i <= 64; i++) {
		gfx.PutPixel(bx + i, by,		70, 250, 200);
		gfx.PutPixel(bx + 64, by + i,	70, 250, 200);
		gfx.PutPixel(bx + i, by + 64,	70, 250, 200);
		gfx.PutPixel(bx, by + i,		70, 250, 200);
	}

	//Triangle
	int cx = 360;
	int cy = 190;
	int r = 0;
	for (int i = 0; i <= 64 - 4; i++)
		gfx.PutPixel(cx + i, cy + 64 - 4 + 2, 255, 0, 0);
	for (int i = 64 - 4; i > 0; i -= 2) {
		for (int k = 0; k < 64 - 4; k++) {
			for (int j = 0; j < 2; j++) {
				if (k == 0)
					gfx.PutPixel(cx + k + r + 1, cy + i - j + 2, 255, 0, 0);
				if (k == 64 - 1 - 4)
					gfx.PutPixel(cx + k - r + 2, cy + i - j + 2, 255, 0, 0);
			}
		}
		r++;
	}

	// Load image for text
	LoadFont(&fixedSys, fontSurf, "Fixedsys16x28.bmp", 16, 28, 32);

	// Waits for space bar to start game
	if (kbd.SpaceIsPressed())
		start = false;
}

// draws current score to screen
void Game::DrawScore() {

	// DRAW SCORE TEXT
	gfx.DrawChar('S', 0, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('C', 16, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('O', 32, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('R', 48, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 64, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(':', 80, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	// END DRAW SCORE TEXT

	// DRAW SCORE VARIABLE
	/* Draws the score by taking each digit of the score and printing it out*/
	
	if (p.getScore() < 10)
		gfx.DrawChar((char)((p.getScore() % 10) + 16), 96, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	else if (p.getScore() < 100) {
		gfx.DrawChar((char)(((p.getScore() / 10) % 10) + 16), 96, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)((p.getScore() % 10) + 16), 112, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (p.getScore() < 1000) {
		gfx.DrawChar((char)(((p.getScore() / 100) % 10) + 16), 96, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getScore() / 10) % 10) + 16), 112, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)((p.getScore() % 10) + 16), 128, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}
	else {
		gfx.DrawChar((char)(((p.getScore() / 1000) % 10) + 16), 96, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getScore() / 100) % 10) + 16), 112, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getScore() / 10) % 10) + 16), 128, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)((p.getScore() % 10) + 16), 144, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}

}

// draws current wave to screen
void Game::DrawWave() {
	if (p.getWave() > 9) {
		gfx.DrawChar((char)((p.getWave() % 10) + 16), 784, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getWave() / 10) % 10) + 16), 768, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));

	}
	else {
		gfx.DrawChar((char)((p.getWave() % 10) + 16), 768, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}
	
	
	gfx.DrawChar(':', 752, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 736, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('V', 720, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 704, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('W', 688, 1, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));

}

// show end game
void Game::EndScreen() {

	gfx.DrawChar('G', 336, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 352, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('M', 368, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 384, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 400, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('O', 416, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('V', 432, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 448, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('R', 464, 270, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));

	// DRAW SCORE TEXT
	gfx.DrawChar('S', 336, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('C', 352, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('O', 368, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('R', 384, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 400, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(':', 416, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	// END DRAW SCORE TEXT

	// DRAW SCORE VARIABLE
	/* Draws the score by taking each digit of the score and printing it out*/
	gfx.DrawChar((char)(((p.getScore() / 100) % 10) + 16), 432, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar((char)(((p.getScore() / 10) % 10) + 16), 448, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar((char)((p.getScore() % 10) + 16), 464, 300, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));

	// Load image for text
	LoadFont(&fixedSys, fontSurf, "Fixedsys16x28.bmp", 16, 28, 32);
	if (kbd.SpaceIsPressed())
		std::exit;
}

// displays the screen before the next wave starts
void Game::LoadingScreenNextWave() {
	Sleep(500);
	gfx.EndFrame();
	gfx.BeginFrame();

	// -- Draw Middle Row -- //
	gfx.DrawChar('s', 272, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('c', 288, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('o', 304, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('r', 320, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('e', 336, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 352, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('b', 368, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('o', 384, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('n', 400, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('u', 416, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('s', 432, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 448, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('+', 464, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	
	if (p.getWave() < 100) {
		gfx.DrawChar((char)((((p.getWave() * 10) / 10) % 10) + 16), 480, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getWave() * 10) % 10) + 16), 496, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}
	else {
		gfx.DrawChar('1', 480, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar('0', 496, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar('0', 512, 290, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}

	// -- Draw Bottom Row -- //
	gfx.DrawChar('N', 304, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 320, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('X', 336, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('T', 352, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 368, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('W', 384, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 400, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('V', 416, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 432, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(':', 448, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	
	if (p.getWave() + 1 < 10)
		gfx.DrawChar((char)(((p.getWave() + 1) % 10) + 16), 464, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	else {
		gfx.DrawChar((char)((((p.getWave() + 1) / 10) % 10) + 16), 464, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)(((p.getWave() + 1) % 10) + 16), 480, 390, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}

	// -- Draw Top Row -- //
	gfx.DrawChar('W', 272, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('A', 288, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('V', 304, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 320, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(' ', 336, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('C', 352, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('O', 368, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('M', 384, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('P', 400, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('L', 416, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 432, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('T', 448, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar('E', 464, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	gfx.DrawChar(':', 480, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	
	if (p.getWave() < 10)
		gfx.DrawChar((char)((p.getWave() % 10) + 16), 496, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	else {
		gfx.DrawChar((char)(((p.getWave() / 10) % 10) + 16), 496, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
		gfx.DrawChar((char)((p.getWave() % 10) + 16), 512, 190, &fixedSys, D3DCOLOR_XRGB(255, 255, 255));
	}

	gfx.EndFrame();

	p.setWave(p.getWave() + 1);// increase wave by one
	
	
	//p.setShapeW(10);// reset player size
	LoadFont(&fixedSys, fontSurf, "Fixedsys16x28.bmp", 16, 28, 32);// load font
	Sleep(7000);// pause until next level

}

/*
// Relocates object to new location (not on top of player current location)
void Game::MakeNewObj(IShape * s)
{
if (p.getX() <= 400)
s->setX(rand() % (787 - 400 + 1) + 400);
else
s->setX(rand() % (400 + 1));

if (p.getY() <= 300)
s->setY(rand() % (587 - 300 + 1) + 300);
else
s->setX(rand() % (300 + 1));

// turn objects exist to true after updation location
s->setExist(true);
}
*/