// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRow.h"
#include "SessionMenuWD.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void USessionRow::SetUpBindBT(USessionMenuWD* SessionWD, uint32 Index)
{
	ParentMenuWD = SessionWD;
	IndexSessionRow = Index;
	RowButton->OnClicked.AddDynamic(this, &USessionRow::OnSelectClicked);
}
void USessionRow::OnSelectClicked()
{
	ParentMenuWD->SelectSessionIndex(IndexSessionRow);
}
