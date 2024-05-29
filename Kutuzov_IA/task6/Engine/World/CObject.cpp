#include "CObject.h"
#include "CScript.h"
#include "CWorld.h"

using namespace std;

// CObject

CObject::CObject(CWorld* World, std::string Name) : CEntity(World, Name)
{
	Visible = true;
}

CObject::~CObject()
{
	for (auto Script : Scripts)
		delete Script.second;
}


// Position
TVector2D CObject::GetPosition() { return Position; }

void CObject::SetPosition(TVector2D& NewPosition) { Position = NewPosition; }

void CObject::Move(TVector2D& Offset) { Position += Offset; }


// Add Script
/*
template<typename T>
T* CObject::AddScript(std::string Name)
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
*/

bool CObject::CallAddScript(CScript* Script, string Name)
{
	if (!Script)
		return false;

	string ScriptName = GetEntityWorldID() + "_" + Name;
	int ScriptNameIndex = Scripts.count(ScriptName);

	if (ScriptNameIndex > 0)
		ScriptName += to_string(ScriptNameIndex);

	Scripts[ScriptName] = Script;
	return Script->EntityCreated(ScriptName);
}


// Rendering
bool CObject::IsVisible() { return Visible; }

void CObject::SetIsVisible(bool InIsVisible)
{
	Visible = InIsVisible;
}

std::vector<TSprite>& CObject::GetSprites() { return Sprites; }


// Collision
void CObject::OnCollided(CCollider* OtherCollider, CCollider* Collider) {}

void CObject::Update(float DeltaTime)
{
	for (auto Script : Scripts)
		Script.second->EntityUpdate(DeltaTime);
}


void CObject::ReceivedCollision(CCollider* OtherCollider, CCollider* Collider)
{
	OnCollided(OtherCollider, Collider);
}

void CObject::CreatedCollision(CCollider* OtherCollider, CCollider* Collider)
{
	OnCollided(OtherCollider, Collider);
}

TRenderLayerLocation& CObject::GetRenderLayerLocation() { return RenderLayerLocation; }



