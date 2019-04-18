//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXAnimationBlend.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Blend between two colours"))
class PALIGHTBARFX_API UPALightBarFXAnimation_Blend : public UPALightBarFXAnimationBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FLinearColor StartColour = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FLinearColor EndColour = FLinearColor::Black;

	void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour) override
	{
		outColour = FMath::Lerp(StartColour, EndColour, GetNormalisedTime());
	}
};


