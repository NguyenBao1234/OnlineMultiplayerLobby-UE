// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MenuWD.generated.h"

/**
 * 
 */
UCLASS()
class M_GAMECPP_API UMenuWD : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetUpWD();
	void TearDownWD();
	void SetMenuInterface(IMenuInterface* InterfaceForMenu);

protected:
	IMenuInterface* MenuInterface;
};
