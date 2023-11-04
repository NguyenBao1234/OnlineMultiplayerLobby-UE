// Fill out your copyright notice in the Description page of Project Settings.


#include "M_GameInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine/Engine.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/SessionMenuWD.h"


const  FName SESSION_NAME = TEXT("Game"); //must be "Game" because SourceFile have only handle for "Game" is SessionName
const FName KeyName_SESSIONSETTING = TEXT("NameRoomSettingSession");

UM_GameInstance::UM_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GI NoConstruct"));
	
	ConstructorHelpers::FClassFinder<UUserWidget>WBPSessionMenu(TEXT("/Game/Widget/WBP_LobbySessionMenu"));
	SessionMenuWDClass = WBPSessionMenu.Class;
	/*
	ConstructorHelpers::FClassFinder<UUserWidget>WBPPauseMenu(TEXT(''));
	PauseMenuWDClass = WBPPauseMenu.Class;
	*/
}


void UM_GameInstance::Init()
{

	Subsystem = IOnlineSubsystem::Get();
	SessionInterface = Subsystem->GetSessionInterface();

	if (SessionInterface.IsValid()) {
	
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UM_GameInstance::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UM_GameInstance::OnCompleteFindSession);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UM_GameInstance::OnDestroyedSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UM_GameInstance::OnJoinSessionComplete);
	}

	UE_LOG(LogTemp, Warning, TEXT("DI occur and Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	GetEngine()->OnNetworkFailure().AddUObject(this, &UM_GameInstance::OnNetworkFailure);
}




//BlueprintCallable
void UM_GameInstance::LoadMenuSessionWD()
{
	UE_LOG(LogTemp, Warning, TEXT("GI Load session Menu WD"));
	
	SessionMenuWD = CreateWidget<USessionMenuWD>(this, SessionMenuWDClass);
	SessionMenuWD->SetUpWD();
	SessionMenuWD->SetMenuInterface(this);

}

void UM_GameInstance::LoadPauseGameWD()
{
			//pause game widget
}






//Implement MenuInterface
	//Open Map MenuLobby
void UM_GameInstance::LoadLobbyMenuMap()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/LobbyMenuMap", ETravelType::TRAVEL_Absolute);
}

	//Find online session search
void UM_GameInstance::FindSessionRoom()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

	//join session (triu tuong), load map after join complete success
void UM_GameInstance::JoinSessionRoom(uint32 IndexWDRow)
{
	/*if (SessionInterface.IsValid()) { return; }
	if (SessionSearch.IsValid()) { return; }
	*/
	UE_LOG(LogTemp, Warning, TEXT("Session ID you join is :%s"), *SessionSearch->SearchResults[IndexWDRow].GetSessionIdStr())
	if (SessionMenuWD != nullptr)
	{
		SessionMenuWD->TearDownWD();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[IndexWDRow]);
}

	//create session (triu tuong), load map after host complete success
void UM_GameInstance::HostSessionRoom(FString NameRoom)
{
	UE_LOG(LogTemp, Warning, TEXT("GI Host session Menu WD"));
	if (SessionInterface.IsValid())
	{
		DisplayNameRoom = NameRoom;
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
			if (ExistingSession != nullptr)
			{
				SessionInterface->DestroySession(SESSION_NAME);//destroy and reCreate again
			}
			else
			{
				CreateSessionAgain();
			}	
	}
}




void UM_GameInstance::CreateSessionAgain()
{
	FOnlineSessionSettings SessionSettings;

	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{SessionSettings.bIsLANMatch = true;}
	else
		{SessionSettings.bIsLANMatch = false;}
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(KeyName_SESSIONSETTING, DisplayNameRoom, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}




//for Bind AddObject of SessionInterface
	//CreateComplete
void UM_GameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	SessionMenuWD->TearDownWD();

	if (!ensure(GetWorld() != nullptr)) { return; }
	GetWorld()->ServerTravel("/Game/Maps/LobbyMap?listen");
}

	//find complete
void UM_GameInstance::OnCompleteFindSession(bool Success)
{
	
	if (Success && SessionSearch.IsValid())
	{
		TArray<FSessionData>SessionArray;
		for (const FOnlineSessionSearchResult& FoundSession : SessionSearch->SearchResults)
		{
			FSessionData DataRoom;
			DataRoom.MaxPlayers = FoundSession.Session.SessionSettings.NumPublicConnections;
			DataRoom.CurrentPlayers = DataRoom.MaxPlayers - FoundSession.Session.NumOpenPublicConnections;
			DataRoom.HostUsername = FoundSession.Session.OwningUserName;

			FString DisplayNameSession;
			if(FoundSession.Session.SessionSettings.Get(KeyName_SESSIONSETTING, DisplayNameSession))
			{
				DataRoom.Name = DisplayNameSession;
			}
			else { DataRoom.Name = "Untitle"; }
			SessionArray.Add(DataRoom);
		}
		SessionMenuWD->SetAllSessionToList(SessionArray);
	}
	else if(Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("No result search"));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Failed Find Session")); }
}

	//Destroy complete
void UM_GameInstance::OnDestroyedSessionComplete(FName SessionName, bool Success)
{
	if (Success) { CreateSessionAgain(); }
}


	//join session complete
void UM_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	UE_LOG(LogTemp, Warning, TEXT("JoinSessionComplete, Dia chi den session game la %s"), *Address);
}

	// Network lost conection ( network failure )
void UM_GameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type NetWorkFailureType, const FString& ErrorString)
{
	LoadLobbyMenuMap();
}