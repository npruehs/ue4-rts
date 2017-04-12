# Real-Time Strategy Plugin for Unreal Engine 4

## Setting up the Framewok

1. Create a player controller deriving from RTSPlayerController.
1. Create a game mode using that player controller. Compile and save.
1. Use the new game in the world settings.
1. Bind the action Select (e.g. to left mouse button).

## Creating Maps

* Add PlayerStarts to the map.

## Enabling Selection

* Add SelectableComponents to everything selectable.

## Camera Setup

* Create a RTSPlayerPawn blueprint.
* Add a RTSCamera component.
* Set the Rotation of the RTSCamera component as desired (e.g. X = 0, Y = 270, Z = 0).
* Set the (RTS) Camera Distance of the RTSCamera component as desired (e.g. 600).
* Use the RTSPlayerPawn as default pawn in your game mode.

