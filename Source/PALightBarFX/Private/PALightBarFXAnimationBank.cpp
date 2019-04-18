//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "PALightBarFXAnimationBank.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXAnimationBank::PlayAnimationByName(UPALightBarFXComponent* component, const FName& name)
{
	if (FXListType == EPALightBarFXAnimationBankType::GameplayTag)
	{
		CurrentAnimationTag = FGameplayTag::RequestGameplayTag(name);
	}
	else
	{
		CurrentAnimationName = name;
	}

	UPALightBarFXAnimationBase* currentAnimation = GetAnimation();
	if (currentAnimation)
	{
		currentAnimation->Reset(component, this);
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXAnimationBank::PlayAnimationByGameplayTag(UPALightBarFXComponent* component, const FGameplayTag& gameplayTag)
{
	if (FXListType == EPALightBarFXAnimationBankType::Name)
	{
		CurrentAnimationName = gameplayTag.GetTagName();
	}
	else
	{
		CurrentAnimationTag = gameplayTag;
	}

	UPALightBarFXAnimationBase* currentAnimation = GetAnimation();
	if (currentAnimation)
	{
		currentAnimation->Reset(component, this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"UPALightBarFXAnimationBank: GameplayTag '%s' doesn't exist in the FX list.", *CurrentAnimationName.ToString());
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UPALightBarFXAnimationBase* UPALightBarFXAnimationBank::GetAnimation() const
{
	if (FXListType == EPALightBarFXAnimationBankType::GameplayTag)
	{
		if (CurrentAnimationTag != FGameplayTag::EmptyTag)
		{
			if (FXListGameplayTag.Contains(CurrentAnimationTag) == false)
			{
				UE_LOG(LogTemp, Warning, L"UPALightBarFXAnimationBank: GameplayTag '%s' doesn't exist in the FX list.", *CurrentAnimationName.ToString());
				return nullptr;
			}

			return FXListGameplayTag[CurrentAnimationTag];
		}
	}
	else
	{
		if (CurrentAnimationName != NAME_None)
		{
			if (FXListName.Contains(CurrentAnimationName) == false)
			{
				UE_LOG(LogTemp, Warning, L"UPALightBarFXAnimationBank: Name '%s' doesn't exist in the FX list.", *CurrentAnimationName.ToString());
				return nullptr;
			}

			return FXListName[CurrentAnimationName];
		}
	}

	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
const FLinearColor& UPALightBarFXAnimationBank::Tick(UPALightBarFXComponent* component,const float deltaTime)
{
	UPALightBarFXAnimationBase* Animation = GetAnimation();
	if (Animation)
	{
		if (Animation->UpdateTime(component, this, deltaTime))
		{
			Animation->Process(component, this, deltaTime, CurrentColour);
		}
		else
		{
			CurrentAnimationName = NAME_None;
			CurrentAnimationTag = FGameplayTag::EmptyTag;
		}
	}

	return CurrentColour;
}






