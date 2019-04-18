//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXAnimationPulse.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Pulse between two colours"))
class PALIGHTBARFX_API UPALightBarFXAnimation_Pulse : public UPALightBarFXAnimationBase
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
	FLinearColor StartColour = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FLinearColor EndColour = FLinearColor::Black;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float PulsesPerSecond = 2.0f;

	float Pulse(const float time) const
	{
		const int32 resolution = 1000;
		return 1.0f - abs(resolution - ((int32)(time * PulsesPerSecond * 2 * resolution) % (resolution * 2))) / float(resolution);
	}

	void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour) override
	{
		const float pulseValue = Pulse(CurrentTime);
		outColour = FMath::Lerp(StartColour, EndColour, pulseValue);
	}
};
