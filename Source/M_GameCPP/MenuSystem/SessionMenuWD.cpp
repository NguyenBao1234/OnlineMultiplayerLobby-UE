// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenuWD.h"
#include "SessionRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "UObject/ConstructorHelpers.h"

USessionMenuWD::USessionMenuWD(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>WBPSessionRowClass(TEXT("/Game/Widget/WBP_SessionRow"));
	SessionRowClass = WBPSessionRowClass.Class;
}


bool USessionMenuWD::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

		//Hosting
	if (!ensure(HostPopupBT!=nullptr)) { return false;}
	HostPopupBT->OnClicked.AddDynamic(this, &USessionMenuWD::OpenHostSessionMenu);
	if (!ensure(ConfirmHostBT!=nullptr)) { return false; }
	ConfirmHostBT->OnClicked.AddDynamic(this, &USessionMenuWD::ConfirmHostSession);
	if (!ensure(CancelHostBT!=nullptr)) { return false; }
	CancelHostBT->OnClicked.AddDynamic(this, &USessionMenuWD::OpenSessionMenuWD);

		//Joining
	if (!ensure(JoinPopupBT!=nullptr)) { return false; }
	JoinPopupBT->OnClicked.AddDynamic(this, &USessionMenuWD::OpenJoinSessionMenu);
	if (!ensure(CancelJoinBT!=nullptr)) { return false; }
	CancelJoinBT->OnClicked.AddDynamic(this, &USessionMenuWD::OpenSessionMenuWD);
	if (!ensure(ConfirmJoinBT!=nullptr)) { return false; }
	ConfirmJoinBT->OnClicked.AddDynamic(this, &USessionMenuWD::ConfirmJoinSessionMenu);
	UE_LOG(LogTemp, Warning, TEXT("AddDynamic va Initialize Thanh Cong"));
	return true;
}

	//host session
void USessionMenuWD::OpenHostSessionMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Host Session Menu"));
	MenuWDSwitcher->SetActiveWidget(HostMenuWD);
}

void USessionMenuWD::ConfirmHostSession()
{
	MenuInterface->HostSessionRoom(HostSessionName->GetText().ToString());
}


	//list session
void USessionMenuWD::SetAllSessionToList(TArray<FSessionData> SessionArray)
{
	PanelSessionList->ClearChildren();
	uint32 IndexAssignToRow = 0;
	for (FSessionData SessionAvailableData : SessionArray)
	{
		USessionRow* SessionRowWD = CreateWidget<USessionRow>(this, SessionRowClass);

		//set display infomation of SessionRowWD
		SessionRowWD->NameOfHostedSession->SetText(FText::FromString(SessionAvailableData.Name));
		SessionRowWD->HostUser->SetText(FText::FromString(SessionAvailableData.HostUsername));
		FString FractionConnectText = FString::Printf(TEXT("% d / % d"), SessionAvailableData.CurrentPlayers, SessionAvailableData.MaxPlayers);
		SessionRowWD->ConnectionFraction->SetText(FText::FromString(FractionConnectText));
		
		UE_LOG(LogTemp, Warning, TEXT("Name session found :  %s"), *SessionAvailableData.Name);
		SessionRowWD->SetUpBindBT(this, IndexAssignToRow);
			IndexAssignToRow++;
	PanelSessionList->AddChild(SessionRowWD);
	}
	StateFindSessionText->SetVisibility(ESlateVisibility::Hidden);
}

	//join session
void USessionMenuWD::SelectSessionIndex(uint32 Index)
{
	SelectedIndexRow = Index;
	UpdateChildrenRowSelect();
}

void USessionMenuWD::OpenJoinSessionMenu()
{
	MenuWDSwitcher->SetActiveWidget(JoinMenuWD);
	MenuInterface->FindSessionRoom();
	StateFindSessionText->SetVisibility(ESlateVisibility::Visible);
}

void USessionMenuWD::ConfirmJoinSessionMenu()
{
	if (SelectedIndexRow.IsSet())
	{
		MenuInterface->JoinSessionRoom(SelectedIndexRow.GetValue());
	}

}
	//Updates the selected button, And the remaining buttons become unselected
void USessionMenuWD::UpdateChildrenRowSelect()
{
	for (int i = 0; i < PanelSessionList->GetChildrenCount(); ++i)
	{
		auto RowSessionWD = Cast<USessionRow>(PanelSessionList->GetChildAt(i));
		if (RowSessionWD)
		{
			RowSessionWD->bSelected = (SelectedIndexRow.IsSet() && i == SelectedIndexRow);
		}
	}
}

//Main Menu Session Widget
void USessionMenuWD::OpenSessionMenuWD()
{
	MenuWDSwitcher->SetActiveWidget(SessionMenuWD);
}


