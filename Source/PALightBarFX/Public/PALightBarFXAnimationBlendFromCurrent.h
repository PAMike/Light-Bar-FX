//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXAnimationBlendFromCurrent.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Blend from current to colour"))
class PALIGHTBARFX_API UPALightBarFXAnimationBlendFromCurrent : public UPALightBarFXAnimationBase
{
	GENERATED_BODY()

	FLinearColor CurrentColour = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FLinearColor Colour = FLinearColor::Black;

	void Reset(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context) override
	{
		Super::Reset(component, context);
		CurrentColour = context->CurrentColour;
	}

	void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour) override
	{
		outColour = FMath::Lerp(CurrentColour, Colour, GetNormalisedTime());
	}
};

