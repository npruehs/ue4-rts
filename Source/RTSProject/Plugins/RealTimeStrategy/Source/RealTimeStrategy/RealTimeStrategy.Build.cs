namespace UnrealBuildTool.Rules
{
	public class RealTimeStrategy : ModuleRules
	{
		public RealTimeStrategy(ReadOnlyTargetRules Target)
            : base(Target)
		{
#if UE_4_21_OR_LATER
            PrivatePCHHeaderFile = "Classes/RealTimeStrategyPCH.h";
#endif
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
                    "Landscape"
                });
		}
	}
}
