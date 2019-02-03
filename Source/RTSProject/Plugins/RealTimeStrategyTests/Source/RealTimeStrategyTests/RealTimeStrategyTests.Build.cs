namespace UnrealBuildTool.Rules
{
	public class RealTimeStrategyTests : ModuleRules
	{
		public RealTimeStrategyTests(ReadOnlyTargetRules Target)
            : base(Target)
		{
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            bEnforceIWYU = true;

            PrivateIncludePaths.AddRange(
                new string[]
                {
                    "RealTimeStrategyTests/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "UnrealEd",
                    "RealTimeStrategy"
                });
		}
	}
}
