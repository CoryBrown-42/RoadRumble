// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "QuitMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UQuitMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize(); 

private:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* YesBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* NoBtn;

	UFUNCTION()
	void YesPressed();

	UFUNCTION()
	void NoPressed();
	
};
