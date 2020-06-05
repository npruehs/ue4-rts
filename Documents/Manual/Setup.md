## Setup

### Game Framework Setup

_Real-Time Strategy Plugin for Unreal Engine 4_ extends the [game framework](https://docs.unrealengine.com/en-US/Gameplay/Framework/index.html) provided by Unreal Engine with features that are common in real-time strategy games.

For this, we embrace the principle of _composition over inheritance_: Most features are implemented by the means of components to add to your actors, to allow for the most flexibility on your side when building your game, especially when it comes to combining the plugin with other third-party software.

Some things have been carefully added to the existing game framework however, as designed by Epic, and require a few steps to set up.

The plugin also ships with a few assets that are designed to get you started, e.g. with unit AI, but feel free to modify or replace them as you see fit.

Make sure _View Plugin Content_ is enabled in your view options.

1. Create a player controller deriving from `RTSPlayerController`.
1. Create a player state deriving from `RTSPlayerState`.
1. Create a game state deriving from `RTSGameState`.
1. Create a HUD deriving from `RTSHUD`.
1. Create a game mode deriving from `RTSGameMode`.
1. Use your player controller, player state, game state, and HUD in your game mode.
1. Create an AI controller deriving from `RTSPawnAIController`.
1. Set the _Pawn Behavior Tree Asset_ of the new pawn AI controller to `BT_RTSPawnBehaviorTree`.
1. Set the _Pawn Blackboard Asset_ of the new pawn AI controller to `BB_RTSPawnBlackboard`.
1. Create a player start deriving from `RTSPlayerStart`.
1. Create one or more resource types deriving from `RTSResourceType`.
1. Add the resource types to the `PlayerResourcesComponent` of your player controller.


### Camera Setup

Usually, players control a single [pawn](https://docs.unrealengine.com/en-US/Gameplay/Framework/Pawn/index.html) in Unreal Engine. However, in the case of real-time strategy games, players control many units from a camera perspective far out. Thus, the plugin works best when using a simple pawn with a normal camera whose location reflects what the player wants to see right now. Individual units are not directly possessed by the player controllers, but just "owned" by them.

#### Creating The Camera

1. Create a pawn blueprint.
1. Add a `Camera` component.
1. Set the _Location_ of the `Camera` component as desired (e.g. X = 0, Y = 0, Z = 1500).
1. Set the _Rotation_ of the `Camera` component as desired (e.g. X = 0, Y = -75, Z = 0).
1. Use the pawn as _Default Pawn Class_ in your game mode.

#### Setting Up Camera Movement

1. Bind the axis `MoveCameraLeftRight` (e.g. to Left and Right keys).
1. Bind the axis `MoveCameraUpDown` (e.g. to Up and Down keys).
1. Bind the axis `ZoomCamera` (e.g. to the mouse wheel axis).
1. At your `RTSPlayerController`, set the _Camera Speed_ (e.g. to 1000).
1. At your `RTSPlayerController`, set the _Camera Scroll Threshold_ (e.g. to 20).
1. At your `RTSPlayerController`, set _Camera Zoom Speed_, _Min Camera Distance_ and _Max Camera Distance_ as desired.


### Input Setup

Many default [input](https://docs.unrealengine.com/en-US/Gameplay/Input/index.html) actions for real-time strategy games are already provided by the plugin. Given that you use an RTSPlayerController in your game mode, you can bind the following actions which should speak for themselves. Clearly, all of these bindings are optional.

At _Edit > Project Settings > Engine > Input_ ...

#### Selection

1. Bind the action `Select` (e.g. to left mouse button).
1. Bind the actions `SaveControlGroup0` to `SaveControlGroup9` (e.g. to CTRL+0 to CTRL+9).
1. Bind the actions `LoadControlGroup0` to `LoadControlGroup9` (e.g. to 0 to 9).
1. Bind the action `AddSelection` (e.g. to Left Shift).
1. Bind the action `ToggleSelection` (e.g. to Left Ctrl).

#### Orders

1. Bind the action `IssueOrder` (e.g. to the right mouse button). This will enable typical smart orders, such as moving when right-clicking ground, and attacking when right-clicking enemies.
1. Bind the action `IssueStopOrder` (e.g. to the S key).

#### Health Bars

1. Bind the action `ShowHealthBars` (e.g. to the LeftAlt key).

### Production

1. Bind the action `CancelProduction` (e.g. to Escape).
1. Bind the action `ShowProductionProgressBars` (e.g. to the LeftAlt key).

#### Construction

1. Bind the action `ConfirmBuildingPlacement` (e.g. to Left Mouse Button).
1. Bind the action `CancelBuildingPlacement` (e.g. to Right Mouse Button).
1. Bind the action `CancelConstruction` (e.g. to Escape).
1. Bind the action `ShowConstructionProgressBars` (e.g. to the LeftAlt key).


### Gameplay Tags

The plugin makes use of [gameplay tags](https://docs.unrealengine.com/en-US/Gameplay/Tags/index.html) for enabling condition-based gameplay (such as whether a unit can be attacked or not). 

At _Edit > Project Settings > Project > Gameplay Tags_, add `DT_RTSGameplayTags` to the _Gameplay Tag Table List_.
