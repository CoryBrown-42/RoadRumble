// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"

#include "RumbleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API URumbleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	URumbleGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	/*This function loads the Main Menu and adds it to the viewport.
	It also validates that the menu has been successfully created before adding it.*/
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
    void InGameLoadMenu();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
	
	UFUNCTION(exec)
	void Host() override;

	UFUNCTION(exec)
	void Join(const FString& Address) override;

	UFUNCTION(exec)
	void Kill();

	
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;


private:

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);



	void CreateSession();


};
