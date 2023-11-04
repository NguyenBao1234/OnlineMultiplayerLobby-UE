// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWD.h"
#include "SessionMenuWD.generated.h"

/**
 * 
 */
USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

class UButton;

UCLASS()
class M_GAMECPP_API USessionMenuWD : public UMenuWD
{
	GENERATED_BODY()
public:
	USessionMenuWD(const FObjectInitializer& ObjectInitializer);
	void SelectSessionIndex(uint32 Index);
	void SetAllSessionToList(TArray<FSessionData> SessionArray); //Duoc goi khi Game hoan thanh viec tim Session, va tham so la ket qua cua mang Session tim duoc
	

protected:
	virtual bool Initialize();

private:

//Widget
	//for Host session
	UPROPERTY(meta = (BindWidget))
		UButton* HostPopupBT;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelHostBT;
	UPROPERTY(meta = (BindWidget))
		UButton* ConfirmHostBT;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* HostSessionName; //truyen bien nay tu WD chu duoc go vao sang cho Host() cua interface

	//for Join Session
	UPROPERTY(meta = (BindWidget))
		UButton* JoinPopupBT;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelJoinBT;
	UPROPERTY(meta = (BindWidget))
		UButton* ConfirmJoinBT;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StateFindSessionText;

	//for switch 3 menu widget
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuWDSwitcher;
	UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenuWD;
	UPROPERTY(meta = (BindWidget))
		UWidget* SessionMenuWD;
	UPROPERTY(meta = (BindWidget))
		UWidget* HostMenuWD;

	//for list session
	UPROPERTY(meta = (BindWidget))
		UPanelWidget* PanelSessionList;
	TOptional<uint32> SelectedIndexRow;
	TSubclassOf<UUserWidget> SessionRowClass;

	void UpdateChildrenRowSelect();

//Event 
	//event bind with Host Session
	UFUNCTION()
	void OpenHostSessionMenu();
	UFUNCTION()
	void ConfirmHostSession();

	//Home widget if cancel any widget
	UFUNCTION()
	void OpenSessionMenuWD();

	//event bind with Join Session
	UFUNCTION()
	void OpenJoinSessionMenu();
	UFUNCTION()
	void ConfirmJoinSessionMenu();


};
