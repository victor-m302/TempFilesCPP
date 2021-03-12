// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include "SDL_TTF.h"
#include <string>
#include <cstring>
using namespace std;



// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2{
	float x;
	float y;
};

// Game class
class Game{
public:
	void Text(SDL_Renderer* renderer);

	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// Pong specific
	// Direction of paddle
	int mPaddleDir;
	// Position of paddle
	Vector2 mPaddlePos;
	// Position of ball
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;
	
	//Parametros para a segunda bolinha
	//Posição da bolinha2
	Vector2 mBallPos2;
	//Velocidade da bolinha 2
	Vector2 mBallVel2;

	SDL_Window* window;
	SDL_Surface* surface;
	TTF_Font* font;
	SDL_Texture* text;
	SDL_Rect textRect;
	SDL_Color color = { 0,0,0 };
	SDL_Surface* text_surface;

	char* scoreStr;


};
