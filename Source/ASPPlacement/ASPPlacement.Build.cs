// Fill out your copyright notice in the Description page of Project Settings.
// Incluye modificación para compilar librarías estáticas
using System;
using System.IO;
using UnrealBuildTool;

public class ASPPlacement : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../thirdpartylibraries/")); }
    }

    public ASPPlacement(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        LoadASPLibrary(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

    public bool LoadASPLibrary(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "asplibrary", "lib");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "ASPLibrary." + PlatformString + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "clingo."+PlatformString+".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "asplibrary", "include"));
        }

        Definitions.Add(string.Format("WITH_ASPLIBRARY_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    }
