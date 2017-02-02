/* 
TEAM FRANKLIN 
*/
#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Circle.h"
#include "Player.h"
#include "Triangle.h"
#include "Square.h"
#include <math.h>
#include <ctime>
#include <array>
#include <cstdlib>
#include <mmsystem.h>
#include <windows.h>
const double PI = 3.141592653589793238463;

class Game
{
public:
	Game( HWND hWnd,const KeyboardServer& kServer );
	void Go();

	// Pointer to array of IShape pointers
	IShape ** sPtr = new IShape*[600];
private:
	
	void InitializeSPtr();// initialize sPtr
	void ComposeFrame();// Draw each frame
	void MoveShapes();// Update each shapes location
	void DrawShapes();// Draw each shape
	void StartScreen();// Initial start screen
	void EndScreen();// Show End Screen
	void CheckCollision();// Check for collision between player and objects
	void CheckWaveOver();// Check for end of a wave
	void NextWaveShapeInitialize();// initializes shapes for next level
	void LoadingScreenNextWave();// displays the screen before the next wave starts
	
	void DrawScore();// draws the score
	void DrawWave();// draws the current wave
	void DrawPlayer(int x, int y);// Main draw player function call
	void DrawObj(IShape * s);// Main draw Object function call
	void DrawObjCir(int x, int y);// Draws Circle object
	void DrawObjTri(int x, int y);// Draws Triangle Object
	void DrawObjSqr(int x, int y);// Draws Square Object
	void DrawPlayerCir(int x, int y);// Draws the player as a circle
	void DrawPlayerTri(int x, int y);// Draws the player as a triangle
	void DrawPlayerSqr(int x, int y);// Draws the player as a square

	void MakeNewObj(IShape * s);// Relocates an IShape that has been destroyed
	bool doesCollide(IShape * s, Player p);// Checks for collision between a Player and an IShape

	void CheckGameOver();// check for the end of the game
	

private:

	D3DGraphics gfx;// DirectX Framework Variable
	KeyboardClient kbd;// Access to keyboard variable
	Player p;// Player
	
	D3DCOLOR fontSurf[512 * 84];// Size of font image
	Font fixedSys;

protected:
	int numOfShapes;// Represents the number of shapes int the room (multiply by 3 for actual number)
	
};