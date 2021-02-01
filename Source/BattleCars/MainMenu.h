// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuInterface.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:

	void SetMenuInterface(IMenuInterface* MMenuInterface);

	void Setup();

	void GameFocus();


protected:

	UFUNCTION()
	virtual bool Initialize();


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBtn;

	UFUNCTION()
	void HostServer();

	IMenuInterface* MenuInterface = nullptr;
	

};
