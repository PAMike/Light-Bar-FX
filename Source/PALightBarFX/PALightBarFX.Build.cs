//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

using UnrealBuildTool;
using System;

public class PALightBarFX : ModuleRules
{
	public PALightBarFX(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange
        (
			new string[]
			{
                "CoreUObject",
                "Core",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
                "GameplayTags",
                "InputDevice",
                "PALightBarFXDualshock",
            }
		);
    }
}
