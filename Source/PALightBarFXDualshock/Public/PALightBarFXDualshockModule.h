//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"

class FPALightBarFXDualshockDevice;

class PALIGHTBARFXDUALSHOCK_API FPALightBarFXDualshockModule : public IInputDeviceModule
{
	TSharedPtr<IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	TSharedPtr<FPALightBarFXDualshockDevice> DualShockInputDevice;

public:

	void StartupModule() override;

	static inline FPALightBarFXDualshockModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FPALightBarFXDualshockModule>("PALightBarFXDualshock");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("PALightBarFXDualshock");
	}

	void SetLightBarColour(const int32 controllerId, const FColor& colour);
};
