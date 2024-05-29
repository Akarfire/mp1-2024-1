#include "Engine/ScarletEngine.h"

class CMyScript : public CScript
{
private:
	virtual bool Created() override
	{
		CScript::Created();
		// Вызывается при созднаии скрипта
		return true;
	}

	virtual void Update(float DeltaTime)
	{
		CScript::Update(DeltaTime);
		// Вызвается каждый игровой кадр
	}

public:
	CMyScript(CObject* InOwner, std::string InName) : CScript(InOwner, InName) {}
};


class CMyObject : public CObject
{
	// Пример добавления коллайдера к объекту
	CRectangleCollider* MainCollider;

private:
	virtual bool Created() override
	{
		CObject::Created();
		// Вызывается при созднаии объекта

		return true;
	}

	virtual void Update(float DeltaTime) override
	{
		CObject::Update(DeltaTime);
		// Вызвается каждый игровой кадр
	}

	virtual void OnCollided(CCollider* OtherCollider, CCollider* Collider)
	{
		CObject::OnCollided(OtherCollider, Collider);
		// Вызывается когда коллайдер этого объекта Collider (в данном случае лишь MainCollider)
		// сталкивается с коллайдером OtherCollider
	}

public:
	CMyObject(CWorld* InWorld, std::string InName) : CObject(InWorld, InName)
	{
		// Создание коллайдера
		MainCollider = AddScript<CRectangleCollider>("Main Collider");

		/* Имя спрайта в списке, относительное положение, масштаб
		Sprites.push_back(TSprite("SpriteID", TVector2D(0.f, 0.f), 1.f, TVector2D(1.f, 1.f)));

		// Номер слоя рендера
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
		// Вызывается когда игрок нажимает на кнопку

		/* Пример:
			Игрок нажимает кнопку 'W',
			
			Функция вызывается с параметрами: 
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
		// Вызывается при инициализации мира

		// Создание объекта типа CMyObject
		CMyObject* Obj = Spawn<CMyObject>("New My Object");

		// Удаление обхекта Obj
		Destroy(Obj);
	}

	virtual void Update(float DeltaTime) override
	{
		// Вызывается каждый игровой кадр
	}

public:
	CMyWorld(CGame* InGame) : CWorld(InGame) {}
};


class CMyGame : public CGame
{
public:
	CMyGame() : CGame()
	{
		// Можно заменить создаваемые классы на свои

		GameWorld = new CWorld(this); // замена на свой класс наслденик требуется почти всегда

		GameRender = new CGameRender(this); // замена на класс наследник обязательна!
											//выбор метода рендера (встроенный: CRenderOpenGL)

		PlayerController = new CPlayerController(this); // замена не обязательно и не требуется 
														//в болишинтве случаев

		// Настройки
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