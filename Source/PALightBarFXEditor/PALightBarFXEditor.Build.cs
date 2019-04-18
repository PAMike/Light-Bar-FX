//-------------------------------------------------------------------------------------------------------------------
// Created By PLASTIC ANT SOFTWARE, LTD
// Copyright 2018. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------------

using UnrealBuildTool;

public class PALightBarFXEditor : ModuleRules
{
	public PALightBarFXEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "BlueprintGraph", "PropertyEditor", "UMG", "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PALightBarFX", "GameplayTags", "EditorStyle", "Projects" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "AssetTools" });
    }
}
