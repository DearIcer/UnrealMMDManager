// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMDManager : ModuleRules
{
	public MMDManager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Editor/Blutility/Private",
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Developer/AssetTools/Private"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetTools",
				"Blutility",
				"Core", 
				"CoreUObject", 
				"Engine", 
				"EditorScriptingUtilities",
				"InputCore" ,
				"PhysicsCore",
				"UMG",
				"UMGEditor",
				"UnrealEd","ContentBrowser"
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
