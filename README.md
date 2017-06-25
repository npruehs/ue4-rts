[![license](https://img.shields.io/github/license/npruehs/ue4-rts.svg?maxAge=2592000)](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE)

# Real-Time Strategy Plugin for Unreal Engine 4

Open-Source Real-Time Strategy Plugin for Unreal Engine 4 developed by the original creators of [Hostile Worlds](http://www.indiedb.com/games/hostile-worlds/) for Unreal Engine 3.

We really love the spirit of Unreal 4 moving to open-source, and we'd love to give something back.

Note that we're still in heavy development. Some things are already in place, such as

* Camera Movement & Bounds
* Initial Unit Placement
* Single Unit Selection
* Movement Orders
* Stop Orders
* Attack Orders
* Health, Damage (and Healing), Cooldown, Range
* Victory Conditions

All of this is already completely working in multiplayer as well, and has been fully exposed to scripting, enabling you to update UI and play animations and sounds.

We're now moving on to more advanced stuff, such as [units automatically returning fire using an acquisition radius, minimap or fog of war](https://github.com/npruehs/ue4-rts/issues).

Also, we're going to add it to the Unreal Marketplace for free soon (tm).

## Usage

### Setting Up The Framewok

1. Create a player controller deriving from RTSPlayerController.
1. Create a game mode deriving from RTSGameMode using that player controller. Compile and save.
1. Use the new game mode in the world settings.
1. Create a controller deriving from RTSCharacterAIController.
1. Set the Character Behavior Tree Asset of the new controller to RTSCharacterBehaviorTree.
1. Set the Character Blackboard Asset of the new controller to RTSCharacterBlackboard.

### Creating Maps

1. Add PlayerStarts to the map.
1. Add RTSCameraBoundsVolume to the map.
1. Add a NavMeshBoundsVolume to the map, and build Navigation. You may press P to review your results in the viewport.

### Enabling Selection

1. Bind the action Select (e.g. to left mouse button).
1. Add SelectableComponents to everything selectable.
1. Listen to the OnSelectionChanged event broadcasted by the RTSPlayerController.

### Setting Up The Camera

#### Creating The Camera

1. Create a RTSPlayerPawn blueprint.
1. Add a Camera component.
1. Set the Location of the Camera component as desired (e.g. X = 0, Y = 0, Z = 600).
1. Set the Rotation of the Camera component as desired (e.g. X = 0, Y = 270, Z = 0).
1. Use the RTSPlayerPawn as default pawn in your game mode.

#### Setting Up Camera Movement

1. Bind the axis MoveCameraLeftRight (e.g. to Left and Right keys).
1. Bind the axis MoveCameraUpDown (e.g. to Up and Down keys).
1. At your RTSPlayerController, set the CameraSpeed (e.g. to 500).
1. At your RTSPlayerController, set the CameraScrollThreshold (e.g. to 10).

### Adding Units

1. Create a new RTSCharacter blueprint.
1. Set skeletal mesh, position, rotation, animation, capsule size and max walk speed as usual.
1. Set pawn AI controller class to your RTSCharacterAIController.
1. Add a SelectableComponent.
1. Ensure Pawn > Auto Possess AI is set to "Placed in World or Spawned".

### Enabling Unit Orders

1. Bind the action IssueOrder (e.g. to the right mouse button). This will enable typical smart orders, such as moving when right-clicking ground, and attacking when right-clicking enemies.
1. Bind the action IssueStopOrder (e.g. to the S key).

### Setup Up Attacks

1. Add the RTSAttackableComponent and RTSHealthComponent to any actors that can be attacked.
1. Set the Current Health and Maximum Health properties of the RTSHealthComponent for these actors.
1. Add the RTSAttackComponent to any actors than can attack.
1. Add an attack to the RTSAttackComponent of these actors, setting its Cooldown, Damage and Range.

_Setting the Damage Type is optional. Remaining Cooldown is handled by the framework and just exposed for scripting purposes._

### Setup Victory Conditions

1. Set the Initial Actors for your RTSGameMode. This will spawn initial units for each player at their player start.
1. Optionally, set the Defeat Condition Actor type for your RTSGameMode. This will check whether any actors of the specified type exist for a player whenever he or she loses a unit. If no actor of the specified type remains, the player is defeated. Note that it is up to you to define how defeated players should be handled, e.g. whether you've making a 1v1, FFA or team game.

## Bugs & Feature Requests

We are sorry that you've experienced issues or are missing a feature! After verifying that you are using the latest version and having checked whether a [similar issue](https://github.com/npruehs/ue4-rts/issues) has already been reported, feel free to [open a new issue](https://github.com/npruehs/ue4-rts/issues/new). In order to help us resolving your problem as fast as possible, please include the following details in your report:

* Steps to reproduce
* What happened?
* What did you expect to happen?

After being able to reproduce the issue, we'll look into fixing it immediately.

## Contributing

You'd like to help make this plugin even more awesome? Seems like today's our lucky day! In order to maintain stability of the tool and its code base, please adhere to the following steps, and we'll be pleased to include your additions in our next release.

Note that this plugin is distributed under the [MIT License](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE). So will be your code.

### Step 1: Choose what to do

If you've got no idea how to help, head over to our [issue tracker](https://github.com/npruehs/ue4-rts/issues) and see what you'd like to do most. You can basically pick anything you want to, as long as it's not already assigned to anyone.

If you know exactly what you're missing, [open a new issue](https://github.com/npruehs/ue4-rts/issues/new) to begin a short discussion about your idea and how it fits the project. If we all agree, you're good to go!

### Step 2: Fork the project and check out the code

Real-Time Strategy Plugin for Unreal Engine 4 is developed using the [GitFlow branching model](http://nvie.com/posts/a-successful-git-branching-model/). In order to contribute, you should check out the latest develop branch, and create a new feature or hotfix branch to be merged back.

### Step 3: Implement your feature or bugfix

Real-Time Strategy Plugin for Unreal Engine 4 is based on [Unreal Engine 4.16.1](https://www.unrealengine.com/).

### Step 4: Open a pull request

Finally, [open a pull request](https://help.github.com/articles/creating-a-pull-request/) so we can review your changes together, and finally integrate it into the next release.

## License

Real-Time Strategy Plugin for Unreal Engine 4 is released under the [MIT License](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE).
