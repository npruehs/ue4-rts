namespace UnrealBuildTool.Rules
{
	public class RTSPlugin : ModuleRules
	{
		public RTSPlugin(TargetInfo Target)
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
                    "InputCore"
                });
		}
	}
}