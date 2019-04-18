//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Curves/CurveLinearColor.h"
#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXAnimationCurveAsset.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Colour Curve Asset"))
class PALIGHTBARFX_API UPALightBarFXAnimationCurveAsset : public UPALightBarFXAnimationBase
{
	GENERATED_BODY()

#if WITH_EDITOR
	bool CanEditChange(const UProperty* inProperty) const override
	{
		const bool isEditable = Super::CanEditChange(inProperty);

		if (inProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, DurationInSeconds) ||
			inProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, TimeCurve))
		{
			return false;
		}

		return isEditable;
	}
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UCurveLinearColor* ColourCurve;

	void Reset(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context) override
	{
		Super::Reset(component, context);

		float startTime, endTime;
		ColourCurve->GetTimeRange(startTime, endTime);

		DurationInSeconds = (endTime - startTime);
	}

	void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour) override
	{
		outColour = ColourCurve->GetLinearColorValue(CurrentTime);
	}
};



