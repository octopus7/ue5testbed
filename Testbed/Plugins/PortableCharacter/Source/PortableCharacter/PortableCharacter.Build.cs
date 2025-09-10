using UnrealBuildTool;

public class PortableCharacter : ModuleRules
{
    public PortableCharacter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "HeadMountedDisplay"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            // Add private dependencies here if needed
        });
    }
}

