#ifndef GAME_H
#define GAME_H
#pragma once
#include <cstdlib>      //for random
#include <ctime>        //for seed

#include <future>
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "Mesh.h"
#include "Model.h"
#include "singleton.h"


#include "Snake.h"



enum InputValue
{
	INPUT_NONE,
	INPUT_A = 65,
	INPUT_D = 68,
	INPUT_S = 83,
	INPUT_W = 87,

	INPUT_a = 97,
	INPUT_d = 100,
	INPUT_s = 115,
	INPUT_w = 119
};


class Game : public Singleton<Game>
{
public:
	//standard stuff
	Game() {}
	~Game() {
		Release();
	}
	void Update(float dTime);
	void Render(float dTime);
	void Initialise();
	void Release();
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//a camera we can push around the scene
	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(2, 38, 0);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(2, 38, 0);

	//stick all gameObjects in a vector and use an enum to index them
	std::vector<GameObject_t> mGameObjects;
	static constexpr int TotalSegments = gridSize*gridSize;
	enum GameObjectID { FLOOR, ORANGE, SEGMENT, TOTAL = SEGMENT + TotalSegments }; //always have segment as the penultimate for this logic to work

private:

	//spin some gameObjects around
	float mAngle = 0;
	//printing text
	DirectX::SpriteBatch *mpFontBatch = nullptr;
	DirectX::SpriteFont *mpFont = nullptr;

	//meber variables from prototpye
	//map
	Map_t map;
	//inputs
	char lastKeyPress = ' ';
	InputValue lastValidInput = INPUT_NONE;
	//player
	Snake_t player;
	//apple
	short2 applePos = { 1,1 };
	//game management
	const float delay = 0.5f;
	float delayTimer = 0.0f;

	//load all models
	void LoadGameObjects();
	//separate out in-game rendering
	void RenderGame(float dTime);

	//member function from 

	void TakeInputKey();		
	void OutputQuitMessage();  //needs editing
	bool GameIsRunning();
	void FactorCollisions(short2 nextPos);
	void GenerateRandomPos(short2&);
	void RegenerateApple();

	//other useful funcs
	DirectX::SimpleMath::Vector3 MapPosToVec3(short2 xy) { return DirectX::SimpleMath::Vector3(xy.x - gridSize/2.f +tileSize/2.f, 0, xy.y-gridSize/2.f+tileSize/2.f); }
};

#endif
