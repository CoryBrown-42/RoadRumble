// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMenuInterface(IMenuInterface* MMenuInterface);

	void Setup();

	void GameFocus();

	void GoToMainMenu();
	

protected:

	UFUNCTION()
	virtual bool Initialize();
	
	
private:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitBtn;

	IMenuInterface* MenuInterface = nullptr;
	
};
