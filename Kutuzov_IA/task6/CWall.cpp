#include "CWall.h"
#include "CSnakeWorld.h"
#include "CSnake.h"

CWall::CWall(CWorld* World, std::string Name): CObject(World, Name) 
{
	Collider = AddScript<CRectangleCollider>("WallCollider");
	Sprites.push_back(TSprite("Wall"));
	RenderLayerLocation.RenderLayer = 2;
}

void CWall::SetSize(TVector2D InSize)
{
	Collider->SetSize(InSize);
	Sprites[0].TargetSize = InSize / 100.f;
}

void CWall::OnCollided(CCollider* OtherCollider, CCollider* Collider)
{
	if (OtherCollider->GetOwner()->HasTag("Eater"))
	{
		CSnakeWorld* SnakeWorldCast = dynamic_cast<CSnakeWorld*>(GetWorld());
		if (SnakeWorldCast)
			SnakeWorldCast->GetSnake()->GameOver();
	}
}
