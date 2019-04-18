//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"
#include "Modules/ModuleManager.h"

class FPALightBarFXEditorModule : public IModuleInterface
{
	TSharedPtr<FSlateStyleSet> StyleSet;

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
