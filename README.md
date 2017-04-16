# Real-Time Strategy Plugin for Unreal Engine 4

## Setting Up The Framewok

1. Create a player controller deriving from RTSPlayerController.
1. Create a game mode using that player controller. Compile and save.
1. Use the new game mode in the world settings.

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
