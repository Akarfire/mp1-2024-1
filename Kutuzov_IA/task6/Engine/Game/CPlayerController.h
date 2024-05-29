#pragma once
#include "CGame.h"
#include "KeyCodes.h"

class CInputReceiver;


class CPlayerController
{
	CGame* Game;

	std::map<std::string, CInputReceiver*> InputSubscribers;

	bool IsReceivingInput;

public:
	CPlayerController(CGame* InGame);
	virtual ~CPlayerController();

	// IsReceibingInput interface
	bool GetIsReceivingInput();
	void SetIsReceivingInput(bool InIsReceivingInput);

	// Send Input singal over to all of the Subscribers
	// This function is called from an input source, which in our case is GLFW window
	// You can use any input source you like, currently its described in CRenerOpenGL
	// But it doesn't have to be that way
	void SendInput(int Input, char Action);

	//Subscribing
	void SubscribeToInput(CInputReceiver* InputReceiver);
	void UnSubscribeToInput(CInputReceiver* InputReceiver);

	// Game
	CGame* GetGame();
};

