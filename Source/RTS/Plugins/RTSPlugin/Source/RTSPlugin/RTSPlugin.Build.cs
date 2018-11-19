namespace UnrealBuildTool.Rules
{
	public class RTSPlugin : ModuleRules
	{
		public RTSPlugin(ReadOnlyTargetRules Target)
            : base(Target)
		{
#if UE_4_21_OR_LATER
            PrivatePCHHeaderFile = "Classes/RTSPluginPCH.h";
#endif
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
