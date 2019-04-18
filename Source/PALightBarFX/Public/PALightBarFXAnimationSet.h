//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXAnimationSet.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Set"))
class PALIGHTBARFX_API UPALightBarFXAnimation_Set : public UPALightBarFXAnimationBase
{
	GENERATED_BODY()

#if WITH_EDITOR
	bool CanEditChange(const UProperty* inProperty) const override
	{
		const bool isEditable = Super::CanEditChange(inProperty);

		if (inProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, TimeCurve))
		{
			return false;
		}

		return isEditable;
	}
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FLinearColor Colour = FColor::White;

	void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour) override
	{
		outColour = Colour;
	}
};
