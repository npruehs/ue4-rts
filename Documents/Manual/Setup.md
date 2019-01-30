## Setup

Make sure View Plugin Content is enabled in your view options.

### Game Framework Setup

1. Create a player controller deriving from RTSPlayerController.
1. Create a player state deriving from RTSPlayerState.
1. Create a game state deriving from RTSGameState.
1. Create a HUD deriving from RTSHUD.
1. Create a game mode deriving from RTSGameMode.
1. Create an AI controller deriving from RTSPlayerAIController.
1. Use your player controller, player state, game state, HUD and player AI controller in your game mode.
1. Create an AI controller deriving from RTSPawnAIController.
1. Set the Pawn Behavior Tree Asset of the new controller to BT_RTSPawnBehaviorTree.
1. Set the Pawn Blackboard Asset of the new controller to BB_RTSPawnBlackboard.
1. Create a player start deriving from RTSPlayerStart.
1. Create one or more resource types deriving from RTSResourceType.
1. Add the resource types to the PlayerResourcesComponent of your player controller.

### Camera Setup

#### Creating The Camera

1. Create a pawn blueprint.
1. Add a Camera component.
1. Set the Location of the Camera component as desired (e.g. X = 0, Y = 0, Z = 1500).
1. Set the Rotation of the Camera component as desired (e.g. X = 0, Y = -75, Z = 0).
1. Use the pawn as Default Pawn Class in your game mode.

#### Setting Up Camera Movement

1. Bind the axis MoveCameraLeftRight (e.g. to Left and Right keys).
1. Bind the axis MoveCameraUpDown (e.g. to Up and Down keys).
1. Bind the axis ZoomCamera (e.g. to the mouse wheel axis).
1. At your RTSPlayerController, set the CameraSpeed (e.g. to 1000).
1. At your RTSPlayerController, set the CameraScrollThreshold (e.g. to 20).
1. At your RTSPlayerController, set CameraZoomSpeed, MinCameraDistance and MaxCameraDistance as desired.

### Input Setup

At Edit > Project Settings > Engine > Input ...

#### Selection

1. Bind the action Select (e.g. to left mouse button).
1. Bind the actions SaveControlGroup0 to SaveControlGroup9 (e.g. to CTRL+0 to CTRL+9).
1. Bind the actions LoadControlGroup0 to LoadControlGroup9 (e.g. to 0 to 9).

#### Orders

1. Bind the action IssueOrder (e.g. to the right mouse button). This will enable typical smart orders, such as moving when right-clicking ground, and attacking when right-clicking enemies.
1. Bind the action IssueStopOrder (e.g. to the S key).

#### Health Bars

1. Bind the action ShowHealthBars (e.g. to the LeftAlt key).

#### Construction

1. Bind the action ConfirmBuildingPlacement (e.g. to Left Mouse Button).
1. Bind the action CancelBuildingPlacement (e.g. to Right Mouse Button).
1. Bind the action CancelConstruction (e.g. to Escape).
1. Bind the action ShowConstructionProgressBars (e.g. to the LeftAlt key).
