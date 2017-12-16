namespace UnrealBuildTool.Rules
{
	public class RTSPlugin : ModuleRules
	{
		public RTSPlugin(ReadOnlyTargetRules Target)
            : base(Target)
		{
            PrivateIncludePaths.AddRange(
                new string[]
                {
                    "RTSPlugin/Private"
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
