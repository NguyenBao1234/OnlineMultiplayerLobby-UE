// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class M_GAMECPP_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void LoadLobbyMenuMap() = 0;
	virtual void HostSessionRoom(FString NameRoom) = 0;
	virtual void FindSessionRoom() = 0;
	virtual void JoinSessionRoom(uint32 IndexWDRow) = 0;

};
