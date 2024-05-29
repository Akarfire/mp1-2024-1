#include "CWorld.h"
#include "CEntity.h"
#include "CObject.h"
#include "CScript.h"
#include "CWorldScriptHandler.h"

#include <chrono>
#include <iostream>

using namespace std;

// CWorld

CWorld::CWorld(CGame* InGame)
{
	if (InGame)
	{
		Game = InGame;

		WorldScriptHandler = Spawn<CWorldScriptHandler>("World Script Handler");
	}

	else
		throw (exception("Invalid Game pointer!"));
}

CWorld::~CWorld() 
{
	for (auto Entity : Entities)
		delete Entity.second;
}

// Initializes the world
void CWorld::WorldInitCall()
{
	InitWorld();
}

// Called every game frame
float CWorld::WorldUpdate(float DeltaTime)
{
	auto TStart = chrono::high_resolution_clock::now();

	// Calls update for every entity in the world
	for (auto Entity : Entities)
		Entity.second->EntityUpdate(DeltaTime);

	Update(DeltaTime);

	UpdateCollisions();

	HandleDestructionList();

	auto TEnd = chrono::high_resolution_clock::now();

	float NewDeltaTime = float((chrono::duration_cast<chrono::milliseconds>(TEnd - TStart)).count()) / 1000.f;

	return NewDeltaTime;
}

void CWorld::InitWorld() {} // To override in child classes

void CWorld::Update(float DetlaTime) {} // To override in child classes


// Returns Game Instance pointer
CGame* CWorld::GetGame() { return Game; }

// Returns World Script Handler
CWorldScriptHandler* CWorld::GetWorldScriptHandler() { return WorldScriptHandler; }


// Spawning (for classes with defined CallSpawn)
/*
template <typename T> T* CWorld::Spawn(string Name)
{
	T* NewObject = new T(this, Name);
	if (CallSpawn(NewObject))
		return NewObject;

	else
	{
		delete NewObject;
		return nullptr;
	}
}
*/

// Entity handling
// Spawns entity into the world
bool CWorld::CallSpawn(CEntity* InEntity)
{
	Entities[InEntity->GetEntityWorldID()] = InEntity;

	return InEntity->EntityCreated(InEntity->GetEntityWorldID(), this);
}

// Adds a new object into the world
bool CWorld::CallSpawn(CObject* InObject)
{
	if (!InObject)
		return false;

	AddToRenderLayer(InObject);

	return CallSpawn(static_cast<CEntity*>(InObject)); // Spawns Entity
}


void CWorld::HandleDestructionList()
{
	for (int i = 0; i < DestructionList.size(); i++)
	{
		CEntity* Entity = DestructionList[i];
		if (Entity)
			delete Entity;
	}

	DestructionList.clear();
}

// Destroys entity in the world
bool CWorld::CallDestroy(CEntity* InEntity)
{
	if (!InEntity)
		return false;

	string EntityID = InEntity->GetEntityWorldID();

	if (!(Entities.count(EntityID) > 0))
		return false;

	Entities.erase(EntityID);

	return true;
}

// Destroys object in the world
bool CWorld::CallDestroy(CObject* InObject)
{
	//cout << "Destroying: " << InObject->GetEntityWorldID() << endl;
	if (!InObject)
		return false;

	if (!(Entities.count(InObject->GetEntityWorldID()) > 0))
		return false;

	RemoveFromRenderLayer(InObject);

	return CallDestroy(dynamic_cast<CEntity*>(InObject));
}

void CWorld::Destroy(CEntity* InEntity)
{
	CObject* ObjectCast = dynamic_cast<CObject*>(InEntity);
	if (ObjectCast)
		CallDestroy(ObjectCast);

	else
		CallDestroy(InEntity);

	//cout << InEntity->GetEntityWorldID() << endl;
	DestructionList.push_back(InEntity);
}


// Handling object collisions in the world
void CWorld::UpdateCollisions()
{
	// Very unoptimized, should probably implement some kind of chunk system to handle this properly

	for (auto Collider : CollisionCreators)
		for (auto OtherCollider : CollisionReceivers)
		{
			if (Collider.second != OtherCollider.second && Collider.second->CheckCollision(OtherCollider.second))
			{
				if (!Collider.second->HasCollisionWith(OtherCollider.second))
				{
					Collider.second->AddCollision(OtherCollider.second);
					OtherCollider.second->AddCollision(Collider.second);
					Collider.second->GetOwner()->CreatedCollision(OtherCollider.second, Collider.second);
					OtherCollider.second->GetOwner()->ReceivedCollision(Collider.second, OtherCollider.second);
				}
			}

			else if (Collider.second->HasCollisionWith(OtherCollider.second))
			{
				Collider.second->RemoveCollision(OtherCollider.second);
				OtherCollider.second->RemoveCollision(Collider.second);
			}
		}
}

void CWorld::RegisterCollider(CCollider* Collider)
{
	if (Collider->CreatesCollision)
		CollisionCreators[Collider->GetEntityWorldID()] = Collider;

	if (Collider->ReceivesCollision)
		CollisionReceivers[Collider->GetEntityWorldID()] = Collider;
}

void CWorld::UnregisterCollider(CCollider* Collider)
{
	if (Collider->CreatesCollision)
		CollisionCreators.erase(Collider->GetEntityWorldID());

	if (Collider->ReceivesCollision)
		CollisionReceivers.erase(Collider->GetEntityWorldID());
}

std::string CWorld::GenerateUniqueEntityName(std::string Name)
{
	int Index = 0;
	string EntityID = Name;

	while (Entities.count(EntityID + "_" + to_string(Index)) > 0)
		Index++;

	EntityID += "_" + to_string(Index);

	return EntityID;
}

std::map<int, std::vector<CObject*>>& CWorld::GetRenderLayers()
{
	return RenderLayers;
}

// Rendering Layers
void CWorld::AddToRenderLayer(CObject* InObject)
{
	TRenderLayerLocation ObjectRenderLayer = InObject->GetRenderLayerLocation();

	InObject->GetRenderLayerLocation().Index = RenderLayers[ObjectRenderLayer.RenderLayer].size();
	RenderLayers[ObjectRenderLayer.RenderLayer].push_back(InObject);
}

void CWorld::RemoveFromRenderLayer(CObject* InObject)
{
	TRenderLayerLocation ObjectRenderLayer = InObject->GetRenderLayerLocation();

	for (int i = ObjectRenderLayer.Index; i < RenderLayers[ObjectRenderLayer.RenderLayer].size(); i++)
		RenderLayers[ObjectRenderLayer.RenderLayer][i]->GetRenderLayerLocation().Index--;

	RenderLayers[ObjectRenderLayer.RenderLayer].erase(RenderLayers[ObjectRenderLayer.RenderLayer].begin() + ObjectRenderLayer.Index);
}


