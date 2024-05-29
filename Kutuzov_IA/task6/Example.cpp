#include "Engine/ScarletEngine.h"

class CMyScript : public CScript
{
private:
	virtual bool Created() override
	{
		CScript::Created();
		// ���������� ��� �������� �������
		return true;
	}

	virtual void Update(float DeltaTime)
	{
		CScript::Update(DeltaTime);
		// ��������� ������ ������� ����
	}

public:
	CMyScript(CObject* InOwner, std::string InName) : CScript(InOwner, InName) {}
};


class CMyObject : public CObject
{
	// ������ ���������� ���������� � �������
	CRectangleCollider* MainCollider;

private:
	virtual bool Created() override
	{
		CObject::Created();
		// ���������� ��� �������� �������

		return true;
	}

	virtual void Update(float DeltaTime) override
	{
		CObject::Update(DeltaTime);
		// ��������� ������ ������� ����
	}

	virtual void OnCollided(CCollider* OtherCollider, CCollider* Collider)
	{
		CObject::OnCollided(OtherCollider, Collider);
		// ���������� ����� ��������� ����� ������� Collider (� ������ ������ ���� MainCollider)
		// ������������ � ����������� OtherCollider
	}

public:
	CMyObject(CWorld* InWorld, std::string InName) : CObject(InWorld, InName)
	{
		// �������� ����������
		MainCollider = AddScript<CRectangleCollider>("Main Collider");

		/* ��� ������� � ������, ������������� ���������, �������
		Sprites.push_back(TSprite("SpriteID", TVector2D(0.f, 0.f), 1.f, TVector2D(1.f, 1.f)));

		// ����� ���� �������
		RenderLayerLocation.RenderLayer = 0;
		*/
	}
};


class CMyPlayer : public CObject, public CInputReceiver
{

private:
	virtual bool Created() override
	{
		CObject::Created();
		return true;
	}
	
	virtual void Update(float DeltaTime) override 
	{
		CObject::Update(DeltaTime);
	}
	
	virtual void ReceiveInput(int Input, char Action) override 
	{
		// ���������� ����� ����� �������� �� ������

		/* ������:
			����� �������� ������ 'W',
			
			������� ���������� � �����������: 
				Input = KEY_W, Action = A_KEY_PRESS
		*/
	}

public:
	CMyPlayer(CWorld* InWorld, std::string InName) : CObject(InWorld, InName), CInputReceiver(InWorld->GetGame()) {}
};


class CMyWorld : public CWorld
{
private:
	virtual void InitWorld() override
	{
		// ���������� ��� ������������� ����

		// �������� ������� ���� CMyObject
		CMyObject* Obj = Spawn<CMyObject>("New My Object");

		// �������� ������� Obj
		Destroy(Obj);
	}

	virtual void Update(float DeltaTime) override
	{
		// ���������� ������ ������� ����
	}

public:
	CMyWorld(CGame* InGame) : CWorld(InGame) {}
};


class CMyGame : public CGame
{
public:
	CMyGame() : CGame()
	{
		// ����� �������� ����������� ������ �� ����

		GameWorld = new CWorld(this); // ������ �� ���� ����� ��������� ��������� ����� ������

		GameRender = new CGameRender(this); // ������ �� ����� ��������� �����������!
											//����� ������ ������� (����������: CRenderOpenGL)

		PlayerController = new CPlayerController(this); // ������ �� ����������� � �� ��������� 
														//� ���������� �������

		// ���������
		GameName = "My Game";
	}
};


int main()
{
	CMyGame* Game = new CMyGame();

	Game->RunGame();

	delete Game;

	return 0;
}

*/