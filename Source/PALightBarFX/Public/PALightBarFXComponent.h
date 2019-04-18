//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Gameframework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h"

#include "PALightBarFXAnimationBank.h"
#include "PALightBarFXComponent.generated.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS()
class PALIGHTBARFX_API UPALightBarFXComponentDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UImage* DebugImage;

	UPROPERTY()
	UTextBlock* DebugText;

	bool Initialize() override
	{
		bool retVal = Super::Initialize();

		auto verticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), L"VBox");
		if (verticalBox)
		{
			WidgetTree->Modify();
			WidgetTree->RootWidget = verticalBox;

			verticalBox->Modify();

			DebugImage = NewObject<UImage>(UImage::StaticClass());
			DebugImage->SetVisibility(ESlateVisibility::Visible);

			//DebugText = NewObject<UTextBlock>(UTextBlock::StaticClass());
			//DebugText->SetColorAndOpacity(FLinearColor::White);
			//DebugText->SetVisibility(ESlateVisibility::Visible);
			//DebugText->SetText(FText::FromString(L"TEST!!!!"));

			auto slot1 = verticalBox->AddChildToVerticalBox(DebugImage);
			//auto slot2 = verticalBox->AddChildToVerticalBox(DebugText);
		}

		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup = Custom, meta = (BlueprintSpawnableComponent, DisplayName="Light Bar FX"))
class PALIGHTBARFX_API UPALightBarFXComponent : public UActorComponent
{
	friend UPALightBarFXAnimationBank;

	GENERATED_UCLASS_BODY()
		
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UPALightBarFXComponentDebugWidget* DebugWidget;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX", meta = (AllowPrivateAccess = "true"))
	bool EnableDebugWidget = false;
#endif

	float UpdateFrequencyCounter = 0.0f;
	FLinearColor CurrentColour = FLinearColor::Blue;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UPALightBarFXAnimationBank* CurrentFXBank;

	UPROPERTY(EditAnywhere, Category = "Light Bar FX", meta = (AllowPrivateAccess = "true"))
	float UpdateFrequency;

	void BeginPlay() override;
	void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) override;
	void SetControllerLightColour(APlayerController* PlayerController, const FColor& Color) const;

public:

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX")
	UPALightBarFXAnimationBank* GetCurrentBank() const { return CurrentFXBank; }

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX")
	FLinearColor GetCurrentColour() const { return CurrentColour; }

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX", meta = (DisplayName="Play FX Animation By FName"))
	void PlayAnimationByName(UPALightBarFXAnimationBank* AnimBank, const FName fxName);

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX", meta = (DisplayName = "Play FX Animation By FGameplayTag"))
	void PlayAnimationByGameplayTag(UPALightBarFXAnimationBank* AnimBank, const FGameplayTag fxName);

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX")
	void Clear(const FLinearColor& defaultColour);

	UFUNCTION(BlueprintCallable, Category = "Light Bar FX")
	FText GetInformation() const;
};


