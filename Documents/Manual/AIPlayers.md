## AI Players

The plugin provides basic support for AI players as well. Currently, this doesn't go beyond fulfilling basic build orders, so you'll probably want to extend that, e.g. by determining when to attack other players, and where. At least, this should get you started:

1. Create an AI controller deriving from `RTSPlayerAIController`.
1. Set the _Player Behavior Tree Asset_ of your new player AI controller to `BT_RTSPlayerBehaviorTree` (or create your own one).
1. Set the _Player Blackboard Asset_ of your new player AI controller to `BB_RTSPlayerBlackboard` (or create your own one).
1. Set up the _Build Order_ of your new player AI controller. The AI will produce the specified actors in order, automatically skipping actors that are already available and replacing those that have been destroyed.
1. Set up the _Primary Resource Type_ of your new player AI controller. The AI will try and prevent blocking paths between its main building and resource sources of that type.
1. Add your resource types to the `PlayerResourcesComponent` of your player AI controller.
1. Check _Gives Bounty_ if killing actors owned by the AI should yield bounties (e.g. for neutral players).
1. Use your player AI controller in your game mode.
1. At your game mode, set _Num AI Players_ to the number of AI players you want to spawn.
