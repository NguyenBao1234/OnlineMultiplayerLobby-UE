// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWD.h"

void UMenuWD::SetUpWD()
{
	this->AddToViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(InputModeData);
	}
}

void UMenuWD::TearDownWD()
{
	this->RemoveFromParent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	}

}

void UMenuWD::SetMenuInterface(IMenuInterface* InterfaceForMenu)
{
	MenuInterface = InterfaceForMenu;
}
