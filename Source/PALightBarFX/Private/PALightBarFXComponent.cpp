//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "PALightBarFXComponent.h"
#include "PALightBarFXDualshockModule.h"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
UPALightBarFXComponent::UPALightBarFXComponent(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	SetComponentTickEnabled(true);
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
FText UPALightBarFXComponent::GetInformation() const
{
	if (CurrentFXBank)
	{
		return FText::FromString(FString::Printf(L"Bank: %s, Name/Tag: %s", *CurrentFXBank->GetName(), *CurrentFXBank->GetCurrentAnimationName().ToString()));
	}
	return FText::FromString(FString::Printf(L"Bank: None, Name/Tag: None"));
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::PlayAnimationByName(UPALightBarFXAnimationBank* fxAsset, const FName fxName)
{
	CurrentFXBank = fxAsset;
	if (CurrentFXBank)
	{
		CurrentFXBank->CurrentColour = CurrentColour;
		CurrentFXBank->PlayAnimationByName(this, fxName);
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::PlayAnimationByGameplayTag(UPALightBarFXAnimationBank* fxAsset, const FGameplayTag fxTag)
{
	CurrentFXBank = fxAsset;
	if (CurrentFXBank)
	{
		CurrentFXBank->CurrentColour = CurrentColour;
		CurrentFXBank->PlayAnimationByGameplayTag(this, fxTag);
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::Clear(const FLinearColor& defaultColour)
{
	CurrentFXBank = nullptr;
	CurrentColour = defaultColour;
	PlayerController->SetControllerLightColor(CurrentColour.ToFColor(false));
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = CastChecked<APlayerController>(GetOwner());

#if WITH_EDITORONLY_DATA
	if (EnableDebugWidget)
	{
		DebugWidget = CreateWidget<UPALightBarFXComponentDebugWidget>(GetWorld(), UPALightBarFXComponentDebugWidget::StaticClass());
		DebugWidget->AddToViewport();
	}
#endif
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::SetControllerLightColour(APlayerController* PlayerController, const FColor& Color) const
{
#if PLATFORM_PS4
	PlayerController->SetControllerLightColor(color);
#else

	if (PlayerController->Player == nullptr)
	{
		return;
	}

	const int32 ControllerId = CastChecked<ULocalPlayer>(PlayerController->Player)->GetControllerId();

	FPALightBarFXDualshockModule* lightBarFXDualshockModule = static_cast<FPALightBarFXDualshockModule*>(FModuleManager::Get().GetModule(L"PALightBarFXDualshock"));
	lightBarFXDualshockModule->SetLightBarColour(ControllerId, Color);

#endif
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void UPALightBarFXComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	UpdateFrequencyCounter += deltaTime;

	check(PlayerController);

	if (CurrentFXBank)
	{
		CurrentColour = CurrentFXBank->Tick(this, deltaTime);
	}

#if WITH_EDITORONLY_DATA
	if (EnableDebugWidget)
	{
		CurrentColour.A = 1.0f;
		DebugWidget->DebugImage->SetColorAndOpacity(CurrentColour);
	}
#endif

	if (UpdateFrequencyCounter >= UpdateFrequency)
	{
		UpdateFrequencyCounter = 0.0f;
		SetControllerLightColour(PlayerController, CurrentColour.ToFColor(false));
	}
}

