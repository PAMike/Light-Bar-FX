//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#include "PALightBarFXEditorModule.h"
#include "PALightBarFXEditorAnimationBankDetails.h"
#include "PALightBarFXAnimationBank.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "PALightBarFXEditorModule"

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXEditorModule::StartupModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		{
			propertyModule.RegisterCustomClassLayout(UPALightBarFXAnimationBank::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPALightBarFXEditorAnimationBankDetails::MakeInstance));
			propertyModule.NotifyCustomizationModuleChanged();
		}
	}

	StyleSet = MakeShareable(new FSlateStyleSet("PALightBarFXAnimationBankStyle"));

	const FString ContentDir = IPluginManager::Get().FindPlugin("PALightBarFX")->GetBaseDir();

	StyleSet->SetContentRoot(ContentDir);

	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(L"Resources/Icon128", L".png"), FVector2D(128.0f, 128.0f));

	if (ThumbnailBrush)
	{
		StyleSet->Set("ClassThumbnail.PALightBarFXAnimationBank", ThumbnailBrush);
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
void FPALightBarFXEditorModule::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPALightBarFXEditorModule, PALightBarFXEditor)