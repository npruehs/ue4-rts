# Real-Time Strategy Plugin for Unreal Engine 4

## Setting Up The Framewok

1. Create a player controller deriving from RTSPlayerController.
1. Create a game mode deriving from RTSGameMode using that player controller. Compile and save.
1. Use the new game mode in the world settings.
1. Create a controller deriving from RTSCharacterAIController.
1. Set the Character Behavior Tree Asset of the new controller to RTSCharacterBehaviorTree.
1. Set the Character Blackboard Asset of the new controller to RTSCharacterBlackboard.

## Creating Maps

1. Add PlayerStarts to the map.
1. Add RTSCameraBoundsVolume to the map.
1. Add a NavMeshBoundsVolume to the map, and build Navigation. You may press P to review your results in the viewport.

## Enabling Selection

1. Bind the action Select (e.g. to left mouse button).
1. Add SelectableComponents to everything selectable.
1. Listen to the OnSelectionChanged event broadcasted by the RTSPlayerController.

## Setting Up The Camera

### Creating The Camera

1. Create a RTSPlayerPawn blueprint.
1. Add a Camera component.
1. Set the Location of the Camera component as desired (e.g. X = 0, Y = 0, Z = 600).
1. Set the Rotation of the Camera component as desired (e.g. X = 0, Y = 270, Z = 0).
1. Use the RTSPlayerPawn as default pawn in your game mode.

### Setting Up Camera Movement

1. Bind the axis MoveCameraLeftRight (e.g. to Left and Right keys).
1. Bind the axis MoveCameraUpDown (e.g. to Up and Down keys).
1. At your RTSPlayerController, set the CameraSpeed (e.g. to 500).
1. At your RTSPlayerController, set the CameraScrollThreshold (e.g. to 10).

## Adding Units

1. Create a new RTSCharacter blueprint.
1. Set skeletal mesh, position, rotation, animation, capsule size and max walk speed as usual.
1. Set pawn AI controller class to your RTSCharacterAIController.
1. Add a SelectableComponent.
1. Ensure Pawn > Auto Possess AI is set to "Placed in World or Spawned".

## Enabling Unit Orders

1. Bind the action IssueOrder (e.g. to the right mouse button). This will enable typical smart orders, such as moving when right-clicking ground, and attacking when right-clicking enemies.
1. Bind the action IssueStopOrder (e.g. to the S key).

## Setup Up Attacks

1. Add the RTSAttackableComponent and RTSHealthComponent to any actors that can be attacked.
1. Set the Current Health and Maximum Health properties of the RTSHealthComponent for these actors.
1. Add the RTSAttackComponent to any actors than can attack.
1. Add an attack to the RTSAttackComponent of these actors, setting its Cooldown, Damage and Range.

_Setting the Damage Type is optional. Remaining Cooldown is handled by the framework and just exposed for scripting purposes._

## Setup Victory Conditions

1. Set the Initial Actors for your RTSGameMode. This will spawn initial units for each player at their player start.
1. Optionally, set the Defeat Condition Actor type for your RTSGameMode. This will check whether any actors of the specified type exist for a player whenever he or she loses a unit. If no actor of the specified type remains, the player is defeated. Note that it is up to you to define how defeated players should be handled, e.g. whether you've making a 1v1, FFA or team game.