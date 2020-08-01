namespace UnrealBuildTool.Rules
{
	public class RealTimeStrategy : ModuleRules
	{
		public RealTimeStrategy(ReadOnlyTargetRules Target)
            : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            bEnforceIWYU = true;

            PrivateIncludePaths.AddRange(
                new string[]
                {
                    "RealTimeStrategy/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "AIModule",
                    "UMG",
                    "Slate",
                    "SlateCore",
                    "RHI",
                    "RenderCore",
                    "Landscape",
                    "GameplayTags",
                    "NavigationSystem"
                });

            // Setup Gameplay Debugger.
            if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
            {
                PrivateDependencyModuleNames.Add("GameplayDebugger");
                PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
            }
		}
	}
}
