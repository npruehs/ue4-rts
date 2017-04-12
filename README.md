# Real-Time Strategy Plugin for Unreal Engine 4

## Setting up the Framewok

1. Create a player controller deriving from RTSPlayerController.
1. Create a game mode using that player controller. Compile and save.
1. Use the new game mode in the world settings.
1. Bind the action Select (e.g. to left mouse button).

## Creating Maps

* Add PlayerStarts to the map.

## Enabling Selection

* Add SelectableComponents to everything selectable.
* Listen to the OnSelectionChanged event broadcasted by the RTSPlayerController.

## Camera Setup

* Create a RTSPlayerPawn blueprint.
* Add a Camera component.
* Set the Location of the Camera component as desired (e.g. X = 0, Y = 0, Z = 600).
* Set the Rotation of the Camera component as desired (e.g. X = 0, Y = 270, Z = 0).
* Use the RTSPlayerPawn as default pawn in your game mode.
