#ifndef GAME_H
#define GAME_H
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
	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);

	//stick all gameObjects in a vector and use an enum to index them
	std::vector<GameObject_t> mGameObjects;
	enum GameObjectID { FLOOR,  BOX, CROSS, CROSS2, WINDOW, WINDOW2, DRAGON, SPHERE, TOTAL }; 

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

	//load all models
	void LoadGameObjects();
	//separate out in-game rendering
	void RenderGame(float dTime);

	//member function from 

	void TakeInputKey();		//needs editing
	void OutputDeathMessage();  //needs editing
	bool GameIsRunning();
	void FactorCollisions(short2 nextPos);
	void GenerateRandomPos(short2&);
	void RegenerateApple();
};

#endif
