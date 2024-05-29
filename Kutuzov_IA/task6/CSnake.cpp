#include "CSnake.h"
#include <iostream>
#include <cmath>

CSnakeHead::CSnakeHead(CWorld* InWorld, std::string InName) : CObject(InWorld, InName), CInputReceiver(InWorld->GetGame(), InName + "_InputReceiver")
{
	RenderLayerLocation.RenderLayer = 1;
	Sprites.push_back(TSprite("SnakeHead_L"));
	CurrentDirection = TVector2D(-1.f, 0.f);
	InputDirection = TVector2D(-1.f, 0.f);

	SetReceiveInput(true);

	Speed = 300.f;
	DistanceSinceLastCheck = 0.f;

	MouthCollider = AddScript<CRectangleCollider>("MouthCollider");
	MouthCollider->SetSize(TVector2D(40, 40));

	AddTag("Eater");
}

CSnakeHead::~CSnakeHead()
{
	BodyList.clear();
}

void CSnakeHead::ReceiveInput(int Input, char Action)
{
	if (Action == A_KEY_PRESS)
	{
		if (Input == KEY_UP || Input == KEY_W)
			SetInputDirection(TVector2D(0.f, 1.f));

		else if (Input == KEY_DOWN || Input == KEY_S)
			SetInputDirection(TVector2D(0.f, -1.f));

		else if (Input == KEY_LEFT || Input == KEY_A)
			SetInputDirection(TVector2D(-1.f, 0.f));

		else if (Input == KEY_RIGHT || Input == KEY_D)
			SetInputDirection(TVector2D(1.f, 0.f));
	}
}

bool CSnakeHead::Created()
{
	CSnakeWorld* SnakeWorldCast = dynamic_cast<CSnakeWorld*>(GetWorld());
	if (SnakeWorldCast)
		SnakeWorld = SnakeWorldCast;

	return true;
}

void CSnakeHead::Update(float DeltaTime)
{
	CObject::Update(DeltaTime);

	Move(CurrentDirection * DeltaTime * Speed);
	DistanceSinceLastCheck += DeltaTime * Speed;

	if (CurrentDirection.X != 0.f)
		SetPosition(TVector2D(GetPosition().X, TILE_SIZE * (round(GetPosition().Y / TILE_SIZE))));

	if (CurrentDirection.Y != 0.f)
		SetPosition(TVector2D(TILE_SIZE * (round(GetPosition().X / TILE_SIZE)), GetPosition().Y));

	// Tiles

	SnakeWorld->FreeTile(BodyList[BodyList.size() - 1]->GetCurrentTile());
	SnakeWorld->OccupyTile(SnakeWorld->GetTile(GetPosition()));

	// UpdateBody;

	for (auto BodySegment : BodyList)
		BodySegment->Move(BodySegment->GetDirection() * DeltaTime * Speed);

	if (DistanceSinceLastCheck >= TILE_SIZE)
	{
		for (int i = BodyList.size() - 1; i > 0; i--)
		{
			BodyList[i]->SetCurrentTile(SnakeWorld->GetTile(BodyList[i]->GetPosition()));
			BodyList[i]->SetDirection(BodyList[i - 1]->GetDirection());
		}

		BodyList[0]->SetDirection(CurrentDirection);

		DistanceSinceLastCheck = 0;
		CurrentDirection = InputDirection;
		UpdateSprite();
	}	
}

void CSnakeHead::SetInputDirection(TVector2D& NewDirection)
{
	if (NewDirection != CurrentDirection * -1.f)
		InputDirection = NewDirection;
}

void CSnakeHead::UpdateSprite()
{
	if (CurrentDirection.X == 1.f)
		Sprites[0].SpriteID = "SnakeHead_R";

	else if (CurrentDirection.X == -1.f)
		Sprites[0].SpriteID = "SnakeHead_L";

	else if (CurrentDirection.Y == 1.f)
		Sprites[0].SpriteID = "SnakeHead_F";

	else if (CurrentDirection.Y == -1.f)
		Sprites[0].SpriteID = "SnakeHead_B";
}

void CSnakeHead::GameOver()
{
	Speed = 0.f;

	CSnakeWorld* SnakeWorldCast = static_cast<CSnakeWorld*>(GetWorld());
	if (SnakeWorldCast)
		SnakeWorldCast->GameOver();
}

void CSnakeHead::Victory()
{
	Speed = 0.f;
}

void CSnakeHead::AddSegment()
{
	CSnakeBody* NewSegment = GetWorld()->Spawn<CSnakeBody>("SnakeBodySegment");
	int BodySize = BodyList.size();

	NewSegment->SpawnBody(this, BodySize);

	if (BodySize > 0)
	{
		NewSegment->SetDirection(BodyList[BodySize - 1]->GetDirection());
		NewSegment->SetPosition(BodyList[BodySize - 1]->GetPosition() - (BodyList[BodySize - 1]->GetDirection() * TILE_SIZE));
	}

	else
	{
		NewSegment->SetDirection(CurrentDirection);
		NewSegment->SetPosition(GetPosition() - CurrentDirection * TILE_SIZE);
	}
	
	NewSegment->SetCurrentTile(SnakeWorld->GetTile(NewSegment->GetPosition()));
	BodyList.push_back(NewSegment);

	SnakeWorld->OccupyTile(SnakeWorld->GetTile(NewSegment->GetPosition()));
}


// Snake Body

CSnakeBody::CSnakeBody(CWorld* InWorld, std::string InName) : CObject(InWorld, InName) 
{
	Sprites.push_back(TSprite("SnakeBody"));
	RenderLayerLocation.RenderLayer = 1;
	SnakeHead = nullptr; 

	BodyCollider = AddScript<CRectangleCollider>("BodyCollider");
	BodyCollider->SetSize(TVector2D(50, 50));
}

CSnakeBody::~CSnakeBody()
{
}

// Direction
TVector2D CSnakeBody::GetDirection()
{
	return Direction;
}


void CSnakeBody::SetDirection(TVector2D& NewDirection)
{
	Direction = NewDirection;
}

// CurrentTile Interface
TIntVector2D CSnakeBody::GetCurrentTile()
{
	return CurrentTile;
}
void CSnakeBody::SetCurrentTile(TIntVector2D& NewTile)
{
	CurrentTile = NewTile;
}

void CSnakeBody::SpawnBody(CSnakeHead* InSnakeHead, int InIndex)
{
	SnakeHead = InSnakeHead;
	Index = InIndex;
}

void CSnakeBody::Update(float DeltaTime)
{
	CObject::Update(DeltaTime);

	if (Direction.X != 0.f)
		SetPosition(TVector2D(GetPosition().X, TILE_SIZE * (round(GetPosition().Y / TILE_SIZE))));

	if (Direction.Y != 0.f)
		SetPosition(TVector2D(TILE_SIZE * (round(GetPosition().X / TILE_SIZE)), GetPosition().Y));

}

void CSnakeBody::OnCollided(CCollider* OtherCollider, CCollider* Collider)
{
	if (Index > 1 && OtherCollider->GetOwner() == SnakeHead)
		SnakeHead->GameOver();
}
