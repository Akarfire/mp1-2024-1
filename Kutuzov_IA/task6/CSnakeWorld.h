#pragma once
#include "Engine/ScarletEngine.h"

#include <set>

#define TILE_SIZE 80

class CSnakeHead;
class CWall;
class CBackground;
class CScoreMeter;

class CSnakeWorld : public CWorld
{
	float GameOverTimer = 0.f;
	bool Reset = false;

	int SizeX, SizeY;
	float WallThickness = 100.f;

	std::set<TIntVector2D> OccupiedTilesList;

	CSnakeHead* SnakeHead;
	std::vector<CWall*> Walls;
	CBackground* BG;
	CScoreMeter* ScoreMeter;

	int Score;
	int TargetScore = 5;

protected:
	virtual void InitWorld() override;
	virtual void Update(float DeltaTime) override;
	void SpawnFruit();

	void ResetWorld();

public:
	CSnakeWorld(CGame* InGame, int N, int M, int InTargetScore = 5);

	void FruitEaten();
	void GameOver();
	void Victory();

	// Tiles
	void OccupyTile(TIntVector2D& Tile);
	void FreeTile(TIntVector2D& Tile);

	TIntVector2D GetTile(TVector2D& Position);

	// Borders
	float Left();
	float Right();
	float Top();
	float Bottom();
	
	// Snake
	CSnakeHead* GetSnake();
};


class CBackground : public CObject
{
public:
	CBackground(CWorld* World, std::string Name);
};

class CGameOver : public CObject
{
public:
	CGameOver(CWorld* World, std::string Name);
};

class CVictory : public CObject
{
public:
	CVictory(CWorld* World, std::string Name);
};


class CScoreMeter : public CObject
{
	int Score;

private:
	std::string GetSprite(int Digit);

public:
	CScoreMeter(CWorld* World, std::string Name);

	void SetScore(int Score);
};

