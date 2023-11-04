// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "M_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class M_GAMECPP_API UM_GameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UM_GameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMenuSessionWD();

	UFUNCTION(BlueprintCallable)
		void LoadPauseGameWD();

	//override IMenuInterface
	virtual void LoadLobbyMenuMap()override;//Chi mo map menu, load that su o blueprint callable function
	virtual void FindSessionRoom() override;
	virtual void HostSessionRoom(FString NameRoom)override;
	virtual void JoinSessionRoom(uint32 IndexWDRow)override;

private:
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TSubclassOf<class UUserWidget> SessionMenuWDClass;
	TSubclassOf<class UUserWidget> PauseMenuWDClass;
	class USessionMenuWD* SessionMenuWD;

	//create property of session as MaxConnection, Presence, so on...
	void CreateSessionAgain();
	FString DisplayNameRoom;
	class IOnlineSubsystem* Subsystem;
	IOnlineSessionPtr SessionInterface;

	//events bind AddObject
	void OnCompleteFindSession(bool Success);
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroyedSessionComplete(FName SessionName, bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type NetWorkFailureType, const FString& ErrorString);
};
