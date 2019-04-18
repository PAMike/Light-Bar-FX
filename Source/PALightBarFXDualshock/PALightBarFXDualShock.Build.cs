//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

using UnrealBuildTool;
using System;

public class PALightBarFXDualshock : ModuleRules
{
	public PALightBarFXDualshock(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange
        (
			new string[]
			{
                "Core",
                "CoreUObject",
                "ApplicationCore",
                "Engine",
                "Slate",
                "SlateCore",
				"RawInput",
                "InputDevice"
            }
		);
    }
}
