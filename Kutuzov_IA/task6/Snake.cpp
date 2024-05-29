#include "Engine/ScarletEngine.h"
#include "CSnakeWorld.h"
#include <iostream>

using namespace std;

class CSnakeGame : public CGame
{
public:
	CSnakeGame(int TargetScore = 5)
	{
		GameName = "Snake";
		Camera.Resolution = TVector2D(1920, 1080);
		Camera.UnitsPerPixel = 2.f;

		TargetFPS = 60.f;
		DeltaTimeLimit = .1f;

		GameWorld = new CSnakeWorld(this, 20, 20, TargetScore);

		GameRender = new CRenderOpenGL(this,
			{
				{"Square",		TSpriteGL("Resources/ChibiZar.png",		100, 100)},
				{"SnakeHead_F", TSpriteGL("Resources/SnakeHead_F.png",	100, 100)},
				{"SnakeHead_B", TSpriteGL("Resources/SnakeHead_B.png",	100, 100)},
				{"SnakeHead_R", TSpriteGL("Resources/SnakeHead_R.png",	100, 100)},
				{"SnakeHead_L", TSpriteGL("Resources/SnakeHead_L.png",	100, 100)},
				{"SnakeBody",	TSpriteGL("Resources/SnakeBody.png",	100, 100)},
				{"Wall",		TSpriteGL("Resources/Wall.png",			100, 100)},
				{"Fruit",		TSpriteGL("Resources/Fruit.png",		50,  50)} ,
				{"Background",	TSpriteGL("Resources/Background.png",	3440,2160)},
				{"GameOver",	TSpriteGL("Resources/GameOver.png",		1920,1080)},
				{"Victory",		TSpriteGL("Resources/Victory.png",		1920,1080)},

				{"1",			TSpriteGL("Resources/One.png",			100, 100)},
				{"2",			TSpriteGL("Resources/Two.png",			100, 100)},
				{"3",			TSpriteGL("Resources/Three.png",		100, 100)},
				{"4",			TSpriteGL("Resources/Four.png",			100, 100)},
				{"5",			TSpriteGL("Resources/Five.png",			100, 100)},
				{"6",			TSpriteGL("Resources/Six.png",			100, 100)},
				{"7",			TSpriteGL("Resources/Seven.png",		100, 100)},
				{"8",			TSpriteGL("Resources/Eight.png",		100, 100)},
				{"9",			TSpriteGL("Resources/Nine.png",			100, 100)},
				{"0",			TSpriteGL("Resources/Zero.png",			100, 100)},
			});

		PlayerController = new CPlayerController(this);
	}
	~CSnakeGame() {}
};


bool IsInt(string InString)
{
	bool Result = true;
	for (int i = 0; i < InString.length(); i++)
		if (!(isdigit(InString.c_str()[i]) || InString.c_str()[i] == '-'))
		{
			Result = false;
			break;
		}

	return Result;
}


int main()
{
	int TargetScore = 5;

	bool Inputing = true;

	do
	{
		cout << "Please enter Target Score (1 - 99): ";
		string sInt;
		cin >> sInt;

		Inputing = !(IsInt(sInt) && stoi(sInt) > 0 && stoi(sInt) < 100);

		if (Inputing)
			cout << endl << "Not a valid score!" << endl;

		else
			TargetScore = stoi(sInt);

	} while (Inputing);
	

	CSnakeGame* SnakeGame = new CSnakeGame(TargetScore);

	SnakeGame->RunGame();

	delete SnakeGame;
	return 0;
}


