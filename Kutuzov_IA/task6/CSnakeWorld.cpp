#include "CSnakeWorld.h"
#include "CSnake.h"
#include "CFruit.h"
#include "CWall.h"
#include <random>
#include <cmath>

#include <iostream>

CSnakeWorld::CSnakeWorld(CGame* InGame, int N, int M, int InTargetScore) : CWorld(InGame), SizeX(N), SizeY(M), TargetScore(InTargetScore)
{
}



void CSnakeWorld::InitWorld()
{
	srand(time(0));

	TIntVector2D SnakeLocation = TVector2D();

	if (SizeX > 10 && SizeY > 0)
		do
		{
			SnakeLocation = TIntVector2D((rand() % SizeX) - SizeX / 2, (rand() % SizeY) - SizeY / 2);
		} while (!(SnakeLocation.X > -1 * (SizeX / 2) + 4 && SnakeLocation.X < (SizeX / 2) - 6));

	SnakeHead = Spawn<CSnakeHead>("Player");
	SnakeHead->SetPosition(TVector2D(SnakeLocation * TILE_SIZE));
	SnakeHead->AddSegment();
	SnakeHead->AddSegment();
	SnakeHead->AddSegment();
	SnakeHead->AddSegment();
	SnakeHead->AddSegment();
	OccupyTile(GetTile(SnakeHead->GetPosition()));

	Walls.push_back(Spawn<CWall>("NorthWall"));
	Walls.push_back(Spawn<CWall>("SouthWall"));
	Walls.push_back(Spawn<CWall>("WestWall"));
	Walls.push_back(Spawn<CWall>("EastWall"));

	Walls[0]->SetPosition(TVector2D(0.f, Top() + WallThickness));
	Walls[0]->SetSize(TVector2D(Right() - Left() + 3 * WallThickness, WallThickness));

	Walls[1]->SetPosition(TVector2D(0.f, Bottom() - WallThickness));
	Walls[1]->SetSize(TVector2D(Right() - Left() + 3 * WallThickness, WallThickness));

	Walls[2]->SetPosition(TVector2D(Left() - WallThickness, 0.f));
	Walls[2]->SetSize(TVector2D(WallThickness, Top() - Bottom() + WallThickness));

	Walls[3]->SetPosition(TVector2D(Right() + WallThickness, 0.f));
	Walls[3]->SetSize(TVector2D(WallThickness, Top() - Bottom() + WallThickness));

	BG = Spawn<CBackground>("Background");
	ScoreMeter = Spawn<CScoreMeter>("ScoreMeter");
	ScoreMeter->SetPosition(TVector2D(Left() - 450.f, Top()));

	Score = 0;
	SpawnFruit();
}

void CSnakeWorld::Update(float DeltaTime)
{
	if (Reset)
	{
		Reset = false;
		InitWorld();
	}

	if (GameOverTimer > 0.f)
	{
		GameOverTimer -= DeltaTime;
		if (GameOverTimer <= 0.f)
			ResetWorld();
	}

	else if (Score >= TargetScore)
		Victory();
}

void CSnakeWorld::SpawnFruit()
{
	TIntVector2D RandomCoords;
	do
	{
		RandomCoords = TIntVector2D((rand() % SizeX) - SizeX / 2, (rand() % SizeY) - SizeY / 2);
	} while (OccupiedTilesList.find(RandomCoords) != OccupiedTilesList.end());

	CFruit* Fruit = Spawn<CFruit>("Fruit");
	Fruit->SetPosition(TVector2D(RandomCoords) * TILE_SIZE);
	
}

void CSnakeWorld::ResetWorld()
{
	Walls.clear();

	auto Copy_Entities = Entities;
	for (auto Entity : Copy_Entities)
		if (CObject* Object = dynamic_cast<CObject*>(Entity.second))
			if (Object != WorldScriptHandler)
				Destroy(Object);

	Reset = true;
}

void CSnakeWorld::FruitEaten()
{
	Score++;
	ScoreMeter->SetScore(Score);
	SpawnFruit();
	SnakeHead->AddSegment();
}

void CSnakeWorld::GameOver()
{
	GameOverTimer = 2.f;
	Spawn<CGameOver>("GameOver");
}

void CSnakeWorld::Victory()
{
	SnakeHead->Victory();

	GameOverTimer = 2.f;
	Spawn<CVictory>("Victory");
}



// Tiles
void CSnakeWorld::OccupyTile(TIntVector2D& Tile)
{
	OccupiedTilesList.insert(Tile);
}

void CSnakeWorld::FreeTile(TIntVector2D& Tile)
{
	OccupiedTilesList.erase(Tile);
}

TIntVector2D CSnakeWorld::GetTile(TVector2D& Position)
{
	return TIntVector2D(
		round(Position.X / TILE_SIZE),
		round(Position.Y / TILE_SIZE)
	);
}


// Borders
float CSnakeWorld::Left()
{
	return -1 * (SizeX / 2) * TILE_SIZE;
}

float CSnakeWorld::Right()
{
	return (SizeX / 2) * TILE_SIZE;
}

float CSnakeWorld::Top()
{
	return (SizeY / 2) * TILE_SIZE;
}

float CSnakeWorld::Bottom()
{
	return -1 * (SizeY / 2) * TILE_SIZE;
}

CSnakeHead* CSnakeWorld::GetSnake()
{
	return SnakeHead;
}


// Background
CBackground::CBackground(CWorld* World, std::string Name) : CObject(World, Name)
{
	Sprites.push_back(TSprite("Background"));
}


// Score meter
CScoreMeter::CScoreMeter(CWorld* World, std::string Name) : CObject(World, Name) 
{
	RenderLayerLocation.RenderLayer = 3;
	Sprites.push_back(TSprite("0", TVector2D(0, 0), 2.f));
	Sprites.push_back(TSprite("0", TVector2D(120, 0), 2.f));
	Sprites.push_back(TSprite("0", TVector2D(240, 0), 2.f));
}

void CScoreMeter::SetScore(int Score)
{
	Sprites[0].SpriteID = GetSprite((Score / 100) % 10);
	Sprites[1].SpriteID = GetSprite((Score / 10) % 10);
	Sprites[2].SpriteID = GetSprite((Score / 1) % 10);
}

std::string CScoreMeter::GetSprite(int Digit)
{
	switch (Digit)
	{
	case 1: return "1";
	case 2: return "2";
	case 3: return "3";
	case 4: return "4";
	case 5: return "5";
	case 6: return "6";
	case 7: return "7";
	case 8: return "8";
	case 9: return "9";
	case 0: return "0";


	default: return "0";
	}
}

CGameOver::CGameOver(CWorld* World, std::string Name) : CObject(World, Name)
{
	Sprites.push_back(TSprite("GameOver"));
	RenderLayerLocation.Index = 3;
}

CVictory::CVictory(CWorld* World, std::string Name) : CObject(World, Name)
{
	Sprites.push_back(TSprite("Victory"));
	RenderLayerLocation.Index = 3;
}

