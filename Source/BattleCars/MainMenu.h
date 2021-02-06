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

	UPROPERTY(meta = (BindWidget))
	class UButton* BackBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SubJoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressField;
	

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinServer();

	IMenuInterface* MenuInterface = nullptr;
	

};
