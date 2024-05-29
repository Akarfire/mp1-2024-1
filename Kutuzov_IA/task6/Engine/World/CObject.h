#pragma once
#include "../Game/DataTypes.h"
#include "CEntity.h"
#include "CCollider.h"
#include <string>
#include <map>
#include <vector>

class CEntity;
class CScript;

class CObject : public CEntity
{
protected:
	TVector2D Position; // Coordinates (top left corner)

	bool Visible; // Is Visible
	TRenderLayerLocation RenderLayerLocation; // Location on the Render Layer

	std::vector<TSprite> Sprites; // Sprites that the object consists of with local offsets

	std::map<std::string, CScript*> Scripts; // Map of all the scripts attached to object


protected:
	virtual void OnCollided(CCollider* OtherCollider, CCollider* Collider);
	virtual void Update(float DeltaTime) override;

public:
	CObject(CWorld* World, std::string Name = "Object");
	virtual ~CObject();

	// Position
	TVector2D GetPosition();
	void SetPosition(TVector2D& NewPosition);
	void Move(TVector2D& Offset);

	// Scripts
	template <typename T> T* AddScript(std::string Name)
	{
		T* NewScript = new T(this, Name);
		if (CallAddScript(NewScript, Name))
			return NewScript;

		else
		{
			delete NewScript;
			return nullptr;
		}
	}

	bool CallAddScript(CScript* Script, std::string Name); // Adds an already existing script to the object

	// Rendering
	bool IsVisible();
	void SetIsVisible(bool InIsVisible);
	std::vector<TSprite>& GetSprites();
	TRenderLayerLocation& GetRenderLayerLocation();

	// Collision
	virtual void ReceivedCollision(CCollider* OtherCollider, CCollider* Collider);
	virtual void CreatedCollision(CCollider* OtherCollider, CCollider* Collider);

};
