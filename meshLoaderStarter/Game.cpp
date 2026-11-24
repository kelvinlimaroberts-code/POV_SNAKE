#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"
#include "GeometryBuilder.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/*quick way to configure a model
* m - IN/OUT model
* source - IN mesh to use
* scale/pos/rot - IN
*/
void Setup(GameObject_t& m, Mesh_t& source, const Vector3& scale, const Vector3& pos, const Vector3& rot)
{
	m.Initialise(source);
	m.GetScale() = scale;
	m.GetPosition() = pos;
	m.GetRotation() = rot;
}

//same as above, but most of the time we want a uniform scale
void Setup(GameObject_t& m, Mesh_t& source, float scale, const Vector3& pos, const Vector3& rot)
{
	Setup(m, source, Vector3(scale, scale, scale), pos, rot);
}


void Game::LoadGameObjects()
{
	MyD3D& d3d = WinUtil::Get().GetD3D();
	
	//create an array of TOTAL GameObjects
	GameObject_t m;
	mGameObjects.insert(mGameObjects.begin(), GameObjectID::TOTAL, m);
	//get a quad and cube mesh ready. also the dragon
	Mesh_t& quadMesh = BuildQuad(d3d.GetMeshMgr());
	Mesh_t& cubeMesh = BuildCube(d3d.GetMeshMgr());
	Mesh_t& sm = d3d.GetMeshMgr().CreateMesh("dragon");
	Mesh_t& otherMesh = d3d.GetMeshMgr().CreateMesh("other");



	//textured lit box
	mGameObjects[GameObjectID::BOX].Initialise(cubeMesh);
	mGameObjects[GameObjectID::BOX].GetPosition() = Vector3(0, -0.5f, 1);
	mGameObjects[GameObjectID::BOX].GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	Material mat = mGameObjects[3].GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tiles.dds");
	mat.texture = "tiles.dds";
	mat.flags |= Material::TFlags::TRANSPARENCY;	//this one uses blend factor controlled transparency
	mat.SetBlendFactors(0.5, 0.5, 0.5, 1);
	mGameObjects[GameObjectID::BOX].SetOverrideMat(&mat);

	//cross - meant to be see through so we want two of them
	//one is the back of the cross, the other is the front
	mGameObjects[GameObjectID::CROSS].Initialise(cubeMesh);
	mat.flags &= ~Material::TFlags::TRANSPARENCY;
	mGameObjects[GameObjectID::CROSS].GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	mGameObjects[GameObjectID::CROSS].GetPosition() = Vector3(1.5f, -0.45f, 1);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "cross.dds");
	mat.texture = "cross";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;	
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	mGameObjects[GameObjectID::CROSS].SetOverrideMat(&mat);

	mGameObjects[GameObjectID::CROSS2] = mGameObjects[GameObjectID::CROSS];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	mGameObjects[GameObjectID::CROSS2].SetOverrideMat(&mat);

	//window - another see through box so we need two models
	mGameObjects[GameObjectID::WINDOW].Initialise(cubeMesh);
	mGameObjects[GameObjectID::WINDOW].GetScale() = Vector3(0.75f, 0.75f, 0.75f);
	mGameObjects[GameObjectID::WINDOW].GetPosition() = Vector3(-1.75f, 0, 1.25f);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "alphaWindow.dds");
	mat.texture = "alphawindow";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	mGameObjects[GameObjectID::WINDOW].SetOverrideMat(&mat);

	mGameObjects[GameObjectID::WINDOW2] = mGameObjects[GameObjectID::WINDOW];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	mGameObjects[GameObjectID::WINDOW2].SetOverrideMat(&mat);

	//quad wood floor
	Setup(mGameObjects[GameObjectID::FLOOR], quadMesh, Vector3(3, 1, 3), Vector3(0, -1, 0), Vector3(0, 0, 0));
	mat = mGameObjects[GameObjectID::FLOOR].GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "floor.dds");
	mat.texture = "floor.dds";
	mGameObjects[GameObjectID::FLOOR].SetOverrideMat(&mat);

	//back wall
	Setup(mGameObjects[GameObjectID::BACK_WALL], quadMesh, Vector3(3, 1, 1.5f), Vector3(0, 0.5f, 3), Vector3(-PI/2, 0, 0));
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(),"wall.dds");
	mat.texture = "wall.dds";
	mGameObjects[GameObjectID::BACK_WALL].SetOverrideMat(&mat);

	//left wall
	Setup(mGameObjects[GameObjectID::LEFT_WALL], quadMesh, Vector3(3, 1, 1.5f), Vector3(-3, 0.5f, 0), Vector3(-PI/2, -PI/2, 0));
	mGameObjects[GameObjectID::LEFT_WALL].SetOverrideMat(&mat);

	//the sun
	d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));

	//the dragon
	sm.CreateFrom("data/dragon/Dragon.X", d3d);
	Setup(mGameObjects[GameObjectID::DRAGON], sm, 0.0005f, Vector3(0, 0, -2), Vector3(0, 0, 0));
	

	//the other one (DJ Kahlid)
	otherMesh.CreateFrom("data/snakeStuff/sphere.obj", d3d);
	Setup(mGameObjects[GameObjectID::OTHER], otherMesh, 0.5f, Vector3(0, 1, -2), Vector3(0, 0, 0));

	mat = mGameObjects[GameObjectID::OTHER].GetMesh().GetSubMesh(0).material;
	//mat.gfxData.Set(Vector4(1, 0.f, 0.f, 0), Vector4(1, 0.5f, 0.5f, 0), Vector4(1, 1, 1, 1)); //red
	mat.gfxData.Set(Vector4(0.2f, 0.9f, 0.2f, 0), Vector4(0.4f, 0.9f, 0.7f, 0), Vector4(0.5f, 0.7f, 0.7f, 1)); //green
	mGameObjects[GameObjectID::OTHER].SetOverrideMat(&mat);


}

void Game::Initialise()
{
	srand(time(0));   //generate seed
	//identify GRID items ON INIT
		//IDENTIFY walls
	for (short y = 0; y < gridSize; y++) //fill first and last rows as walls (y)
	{
		map.IdentifyAsItem(WALL, { 0, y });
		map.IdentifyAsItem(WALL, { gridSize - 1, y });
	}
	for (short x = 1; x < gridSize; x++) //fill first and last columns as walls (x)
	{
		map.IdentifyAsItem(WALL, { x, 0 });
		map.IdentifyAsItem(WALL, { x, gridSize - 1 });
	}

	//place player
	player.MoveSnake({ (gridSize - 2) / 2, (gridSize - 2) / 2 }, map);

	//place apple
	RegenerateApple();


	//create objects for font rendering
	MyD3D& d3d = WinUtil::Get().GetD3D();
	mpFontBatch = new SpriteBatch(&d3d.GetDeviceCtx());
	assert(mpFontBatch);
	mpFont = new SpriteFont(&d3d.GetDevice(), L"../bin/data/fonts/algerian.spritefont");
	assert(mpFont);

	LoadGameObjects();
}

void Game::Release()
{
	//tidy up
	delete mpFontBatch;
	mpFontBatch = nullptr;
	delete mpFont;
	mpFont = nullptr;
}

void Game::Update(float dTime)
{
	////spin things around
	//mAngle += dTime * 0.5f;
	//mModels[Modelid::BOX].GetRotation().y = mAngle;

	//mModels[Modelid::CROSS].GetRotation().y = -mAngle;
	//mModels[Modelid::CROSS2].GetRotation().y = -mAngle;

	//mModels[Modelid::WINDOW].GetRotation().y = -mAngle * 0.5f;
	//mModels[Modelid::WINDOW2].GetRotation().y = -mAngle * 0.5f;


	//mModels[Modelid::DRAGON].GetRotation().y = mAngle * 2;
	//mModels[Modelid::OTHER].GetRotation().y = -mAngle * 2;
	if (GameIsRunning())
	{
		TakeInputKey();
		short2 nextPos = { 0,0 };
		if (player.ValidSnakeMovement(nextPos))
		{
			FactorCollisions(nextPos);
			player.MoveSnake(nextPos, map);
		}
	}
	else
	{
		OutputDeathMessage();
	}
	
}

void Game::Render(float dTime)
{
	RenderGame(dTime); //separate it out? assuming other things will be added
}

void Game::RenderGame(float dTime)
{
	MyD3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	float alpha = 0.5f + sinf(mAngle * 2)*0.5f;

	//setup camera and projections, prep shaders
	d3d.GetFX().SetPerFrameConsts(d3d.GetDeviceCtx(), mCamPos);
	CreateViewMatrix(d3d.GetFX().GetViewMatrix(), mCamPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
	CreateProjectionMatrix(d3d.GetFX().GetProjectionMatrix(), 0.25f*PI, WinUtil::Get().GetAspectRatio(), 1, 1000.f);
	Matrix w = Matrix::CreateRotationY(sinf(mAngle));
	d3d.GetFX().SetPerObjConsts(d3d.GetDeviceCtx(), w);

	////main cube - forced transparency under pogram control (blend factors)
	////update the blend factors and add some spinning spot lights
	//Vector3 dir = Vector3(1, 0, 0);
	//Matrix m = Matrix::CreateRotationY(mAngle);
	//dir = dir.TransformNormal(dir, m);
	//d3d.GetFX().SetupSpotLight(1, true, mModels[Modelid::BOX].GetPosition(), dir, Vector3(0.2f, 0.05f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	//dir *= -1;
	//d3d.GetFX().SetupSpotLight(2, true, mModels[Modelid::BOX].GetPosition(), dir, Vector3(0.05f, 0.2f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	//float d = sinf(mAngle)*0.5f + 0.5f;
	//mModels[Modelid::BOX].HasOverrideMat()->SetBlendFactors(d, d, d, 1);

	//render all models
	for (auto& mod : mGameObjects)
		d3d.GetFX().Render(mod);

	d3d.EndRender();
}

//push the camera around and check for exit
LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const float camInc = 200.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		case 'a':
			mCamPos.y += camInc;
			break;
		case 'z':
			mCamPos.y -= camInc;
			break;
		case 'd':
			mCamPos.x -= camInc;
			break;
		case 'f':
			mCamPos.x += camInc;
			break;
		case 'w':
			mCamPos.z += camInc;
			break;
		case 's':
			mCamPos.z -= camInc;
			break;
		case ' ':
			mCamPos = mDefCamPos;
			break;
		}
	}
	//default message handling (resize window, full screen, etc)
	return WinUtil::Get().DefaultMssgHandler(hwnd, msg, wParam, lParam);
}


void Game::TakeInputKey()
{
	//std::cin >> lastKeyPress;
	DirectionValue desiredDir = player.GetDirection();
	switch ((InputValue)lastKeyPress)
	{
	case INPUT_A:
	case INPUT_a:
		desiredDir = LEFT_DIR;
		break;
	case INPUT_D:
	case INPUT_d:
		desiredDir = RIGHT_DIR;
		break;
	case INPUT_S:
	case INPUT_s:
		desiredDir = DOWN_DIR;
		break;
	case INPUT_W:
	case INPUT_w:
		desiredDir = UP_DIR;
		break;
	default:
		break;
	}
	switch (player.GetDirection())
	{
	case UP_DIR:
		if (desiredDir != DOWN_DIR)
		{
			player.GetDirection() = desiredDir;
		}
		break;
	case LEFT_DIR:
		if (desiredDir != RIGHT_DIR)
		{
			player.GetDirection() = desiredDir;
		}
		break;
	case RIGHT_DIR:
		if (desiredDir != LEFT_DIR)
		{
			player.GetDirection() = desiredDir;
		}
		break;
	case DOWN_DIR:
		if (desiredDir != UP_DIR)
		{
			player.GetDirection() = desiredDir;
		}
		break;
	default:
		player.GetDirection() = desiredDir;
		break;
	}
}

void Game::OutputDeathMessage()
{
	//std::cout << "you died. rip bozo. score = " << player.GetScore();
}

bool Game::GameIsRunning()
{
	return player.HasLife();
}


void Game::FactorCollisions(short2 nextPos)
{
	//now check collisions between things
		//check wall collision
	switch (map.GetIdentityOfItem(nextPos))
	{
	case WALL:
		player.HasLife() = false;
		break;
	case APPLE:
		map.UnidentifyItem(nextPos);
		++player;
		player.IncrementScore();
		map.AppleEaten() = true;
		RegenerateApple();
		break;
	case SNAKE_BODY:
		player.HasLife() = false;
		break;
	}
}



void Game::GenerateRandomPos(short2& pos)
{
	//return x and y as a value between 1 and gridSize-1
	pos.x = (rand() % gridSize - 1) + 1;
	pos.y = (rand() % gridSize - 1) + 1;
}

void Game::RegenerateApple()
{
	if (map.AppleEaten())
	{
		map.AppleEaten() = false;
		short2 applePos = { 1,1 };
		do
		{
			GenerateRandomPos(applePos);
		} while (map.GetIdentityOfItem(applePos) != GridItem::EMPTY);  // make sure that the desired new position for the apple is in an empty spot
		map.IdentifyAsItem(APPLE, applePos);
	}
}
