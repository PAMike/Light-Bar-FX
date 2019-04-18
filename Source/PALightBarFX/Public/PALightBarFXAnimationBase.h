//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Curves/CurveLinearColor.h"
#include "PALightBarFXDefinedCurve.h"
#include "PALightBarFXAnimationBase.generated.h"

class UPALightBarFXAnimationBank;

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Abstract, EditInlineNew, Blueprintable, BlueprintType)
class PALIGHTBARFX_API UPALightBarFXAnimationBase : public UObject
{
	friend UPALightBarFXAnimationBank;

	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (DisplayName = "Dev note (Editor Only)", MultiLine = true))
	FString Notes;
#endif

public:

	UFUNCTION(BlueprintCallable, Category = "Plastic Ant|Light Bar FX|Unit")
	float GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Plastic Ant|Light Bar FX|Unit")
	void OnReset(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context);

	UFUNCTION(BlueprintImplementableEvent, Category = "Plastic Ant|Light Bar FX|Unit")
	void OnRestart(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context);

	UFUNCTION(BlueprintImplementableEvent, Category = "Plastic Ant|Light Bar FX|Unit")
	void OnProcess(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour);

protected:

	float CurrentTime = 0.0f;
	int32 CurrentLoopCounter = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (ToolTip="Use -1 for infinite loop"))
	int32 LoopCounter = -1;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float DurationInSeconds = 2.0f;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Animation", meta = (DisplayName = "Time Curve (Optional)", ToolTip="This curve is used to modify the time used for blending to get various effects"))
	UPALightBarFXDefinedCurve* TimeCurve;

	float GetNormalisedTime() const { const float v = (CurrentTime / DurationInSeconds); return TimeCurve ? TimeCurve->ComputeValue(v) : v; }

	virtual void Reset(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context)
	{
		Restart(component, context);
		CurrentLoopCounter = LoopCounter;
		OnReset(component, context);
	}

	virtual void Restart(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context)
	{
		CurrentTime = 0.0f;
		OnRestart(component, context);
	}

	virtual void Process(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime, FLinearColor& outColour)
	{
		OnProcess(component, context, deltaTime, outColour);
	}

	bool UpdateTime(UPALightBarFXComponent* component, UPALightBarFXAnimationBank* context, const float deltaTime)
	{
		CurrentTime += deltaTime;

		if (CurrentTime > DurationInSeconds)
		{
			if (LoopCounter != -1)
			{
				--CurrentLoopCounter;

				if (CurrentLoopCounter <= 0)
				{
					return false;
				}

				Restart(component, context);
			}
			else
			{
				Restart(component, context);
			}
		}

		return true;
	}
};
