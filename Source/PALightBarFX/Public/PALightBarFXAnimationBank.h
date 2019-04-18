//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PALightBarFXAnimationBase.h"
#include "PALightBarFXAnimationBank.generated.h"

class FPALightBarFXEditorAnimationBankDetails;
class UPALightBarFXComponent;

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UENUM()
enum class EPALightBarFXAnimationBankType : int8
{
	Name,
	GameplayTag,
};

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable, BlueprintType)
class PALIGHTBARFX_API UPALightBarFXAnimationBank : public UDataAsset
{
	GENERATED_BODY()

	friend FPALightBarFXEditorAnimationBankDetails;
	friend UPALightBarFXComponent;

	FName CurrentAnimationName;
	FGameplayTag CurrentAnimationTag;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Light Bar FX Asset", meta = (DisplayName = "FX List Type", AllowPrivateAccess = "true", ToolTip = "Use FName or FGameplayTag to access the FX list."))
	EPALightBarFXAnimationBankType FXListType;

	UPROPERTY(EditDefaultsOnly, Category = "Light Bar FX Asset", Instanced, meta = (DisplayName="FX List", AllowPrivateAccess = "true"))
	TMap<FName, UPALightBarFXAnimationBase*> FXListName;

	UPROPERTY(EditDefaultsOnly, Category = "Light Bar FX Asset", Instanced, meta = (DisplayName = "FX List", AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UPALightBarFXAnimationBase*> FXListGameplayTag;

	void PlayAnimationByName(UPALightBarFXComponent* component, const FName& name);

	void PlayAnimationByGameplayTag(UPALightBarFXComponent* component, const FGameplayTag& gameplayTag);

	UPALightBarFXAnimationBase* GetAnimation() const;

	const FLinearColor& Tick(UPALightBarFXComponent* component, const float deltaTime);

public:

	UPROPERTY(BlueprintReadOnly, Category = "Light Bar FX Asset")
	FLinearColor CurrentColour;

	UFUNCTION(BlueprintCallable, Category = "Plastic Ant|Light Bar FX")
	UPALightBarFXAnimationBase* GetAnimationByGameplayTag(const FGameplayTag tag) const { return FXListGameplayTag.Contains(tag) ? FXListGameplayTag[tag] : nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Plastic Ant|Light Bar FX")
	UPALightBarFXAnimationBase* GetAnimationByName(const FName name) const { return FXListName.Contains(name) ? FXListName[name] : nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Plastic Ant|Light Bar FX")
	FName GetCurrentAnimationName() const { return FXListType == EPALightBarFXAnimationBankType::Name ? CurrentAnimationName : CurrentAnimationTag.GetTagName(); }
};


