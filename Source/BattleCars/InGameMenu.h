// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	

protected:

	UFUNCTION()
	virtual bool Initialize();
	
	
private:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitBtn;

	UFUNCTION()
	void QuitPressed();

	UFUNCTION()
	void ResumePressed();

	
};
