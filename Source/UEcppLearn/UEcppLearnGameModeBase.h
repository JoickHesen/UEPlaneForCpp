// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UEcppLearnGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UECPPLEARN_API AUEcppLearnGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		int32 ponits;

};
