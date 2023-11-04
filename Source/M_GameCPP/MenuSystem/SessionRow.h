// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SessionRow.generated.h"

/**
 * 
 */

UCLASS()
class M_GAMECPP_API USessionRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NameOfHostedSession;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostUser;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ConnectionFraction;
	UPROPERTY(BlueprintReadOnly)
		bool bSelected = false;

	void SetUpBindBT(class USessionMenuWD* SessionWD, uint32 Index);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

	UPROPERTY()
		class USessionMenuWD* ParentMenuWD;

	uint32 IndexSessionRow;

	UFUNCTION()
		void OnSelectClicked();
};
