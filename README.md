[![license](https://img.shields.io/github/license/npruehs/ue4-rts.svg?maxAge=2592000)](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE)

# Real-Time Strategy Plugin for Unreal Engine 4

Open-Source Real-Time Strategy Plugin for Unreal Engine 4 developed by the original creators of [Hostile Worlds](http://www.indiedb.com/games/hostile-worlds/) for Unreal Engine 3.

We really love the spirit of Unreal 4 moving to open-source, and we'd love to give something back.

Note that we're still in heavy development. Some things are already in place, such as

* Camera Movement & Bounds
* Initial Unit Placement
* Single- and Multi-Unit Selection
* Movement Orders
* Stop Orders
* Attack Orders
* Health, Damage (and Healing), Cooldown, Range
* Victory Conditions
* Projectiles
* Unit Target Acquisition
* Control Groups
* Health Bars

All of this is already completely working in multiplayer as well, and has been fully exposed to scripting, enabling you to update UI and play animations and sounds.

We're now moving on to more advanced stuff, such as [minimap or fog of war](https://github.com/npruehs/ue4-rts/issues).

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

1. Add RTSPlayerStarts to the map.
1. Add RTSCameraBoundsVolume to the map.
1. Add a NavMeshBoundsVolume to the map, and build Navigation. You may press P to review your results in the viewport.

### Setting Up The Camera

#### Creating The Camera

1. Create a RTSPlayerPawn blueprint.
1. Add a Camera component.
1. Set the Location of the Camera component as desired (e.g. X = 0, Y = 0, Z = 1500).
1. Set the Rotation of the Camera component as desired (e.g. X = 0, Y = 270, Z = 0).
1. Use the RTSPlayerPawn as default pawn in your game mode.

#### Setting Up Camera Movement

1. Bind the axis MoveCameraLeftRight (e.g. to Left and Right keys).
1. Bind the axis MoveCameraUpDown (e.g. to Up and Down keys).
1. At your RTSPlayerController, set the CameraSpeed (e.g. to 1000).
1. At your RTSPlayerController, set the CameraScrollThreshold (e.g. to 20).

### Adding Units

1. Create a new RTSCharacter blueprint.
1. Set skeletal mesh, position, rotation, animation, capsule size and max walk speed as usual.
1. At the CharacterMovement component, you may set the Max Acceleration property to a high value to avoid stuttering when changing move orders.
1. Set pawn AI controller class to your RTSCharacterAIController.
1. Add a SelectableComponent.
1. Ensure Pawn > Auto Possess AI is set to "Placed in World or Spawned".

### Selecting Units

#### Enabling Selection

1. Bind the action Select (e.g. to left mouse button).
1. Add SelectableComponents to everything selectable.
1. Listen to the OnSelectionChanged event broadcasted by the RTSPlayerController.

#### Showing Selection Frames

1. Create a new HUD deriving from RTSHUD.
1. Use the new RTSHUD in your game mode.
1. In your RTSHUD, implement the DrawSelectionFrame event as desired.

Example: Drawing selection frame as transparent rectangle with borders

![Draw Selection Frame](Documents/Manual/Images/SelectionFrameBlueprint.png)

#### Enabling Control Groups

1. Bind the actions SaveControlGroup0 to SaveControlGroup9 (e.g. to CTRL+0 to CTRL+9).
1. Bind the actions LoadControlGroup0 to LoadControlGroup9 (e.g. to 0 to 9).

#### Showing Selection

1. For each character, set the Decal Material of its SelectionCircleDecalComponent.

### Enabling Unit Orders

1. Bind the action IssueOrder (e.g. to the right mouse button). This will enable typical smart orders, such as moving when right-clicking ground, and attacking when right-clicking enemies.
1. Bind the action IssueStopOrder (e.g. to the S key).

### Setup Up Attacks

1. Add the RTSAttackableComponent and RTSHealthComponent to any actors that can be attacked.
1. Set the Current Health and Maximum Health properties of the RTSHealthComponent for these actors.
1. Add the RTSAttackComponent to any actors than can attack.
1. Add an attack to the RTSAttackComponent of these actors, setting its Cooldown, Damage, Range, Acquisition Radius and Chase Radius.

_Setting the Damage Type is optional. Remaining Cooldown is handled by the framework and just exposed for scripting purposes._

### Adding Projectiles

1. Create a new RTSProjectile blueprint.
1. Set the Impact Threshold property.
1. Set the Replicates property to true (this might change in the future, as we don't really care about exact projectile positions on client-side).
1. For all RTSAttackComponents that should use this projectile, reference the new projectile.

### Setup Victory Conditions

1. Set the Initial Actors for your RTSGameMode. This will spawn initial units for each player at their player start.
1. Optionally, set the Defeat Condition Actor type for your RTSGameMode. This will check whether any actors of the specified type exist for a player whenever he or she loses a unit. If no actor of the specified type remains, the player is defeated. Note that it is up to you to define how defeated players should be handled, e.g. whether you've making a 1v1, FFA or team game.

### Add HUD

#### Showing Health Bars

1. In your RTSHUD, set AlwaysShowHealthBars, ShowHoverHealthBars, ShowSelectionHealthBars and ShowHotkeyHealthBars as desired.
1. If you checked ShowHotkeyHealthBars, bind the action ShowHealthBars (e.g. to the LeftAlt key).
1. In your RTSHUD, implement the DrawHealthBar event as desired.

Example: Drawing health bars as rectangles with borders

![Draw Health Bars](Documents/Manual/Images/HealthBarBlueprint.png)

#### Showing Hovered Unit Effects

1. In your RTSHUD, implement the DrawHoveredActorEffect event as desired.

Example: Drawing names of unit owners

![Draw Player Name](Documents/Manual/Images/HoveredActorEffectBlueprint.png)

### Setup Minimap

#### Minimap Background Layer

1. Create a new Asset > Materials & Textures > Render Target (e.g. called MinimapBackgroundRenderTarget).
1. Set its Compression Settings to User Interface and Texture Group to UI.
1. Create a Material from your Render Target.
1. Set its Material Domain to User Interface and make sure the texture is connected to Final Color.
1. Add a SceneCapture2D at the very center of your map (X = 0, Y = 0).
1. Set its height to a reasonable distance (e.g. Z = 2000).
1. Rotate it, making it face your map (e.g. Rotation Y = -90).
1. In the Scene Capture section, assign your render target to the Texture Target of the SceneCapture2D.
1. Disable Capture Every Frame.
1. Still in the Scene Capture section, in the hidden settings, at General Show Flags, disable everything that doesn't make sense for a minimap background (e.g. check BSP, Landscape, Static Meshes only).

#### Minimap Units Layer

1. Add a MinimapVolume at the very center of your map.
1. Set its brush size to match the extents of your playable map.
1. Create a new RTSMinimapWidget blueprint.
1. Set the minimap background image to your minimap background render target material.
1. Set the minimap background image size to a reasonable value (e.g. 256 x 256).
1. Setup brushes for own units, enemy units and neutral units (e.g. image = WhiteSquareTexture, size = 4 x 4, tint = green, red, yellow, respectively).
1. Add your RTSMinimapWidget to your UI, with a matching size (e.g. 256 x 256).
1. Set its Behaviour > Visibility to Visible if the player should be able to move the camera and give orders using the minimap.

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

Real-Time Strategy Plugin for Unreal Engine 4 is dual-licensed:

* everything beneath Source\RTS\Content\StarterContent is governed by the [Unreal® Engine End User License Agreement](https://www.unrealengine.com/eula) 
* everything else is released under the [MIT License](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE)
