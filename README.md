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
* Minimap
* Teams
* Constructions
* Unit Production/Tech Research
* Resource Gathering
* Fog of War

All of this is already completely working in multiplayer as well, and has been fully exposed to scripting, enabling you to update UI and play animations and sounds.

We're going to add all of this to the Unreal Marketplace for free as well soon (tm).

For a quick look, just open RTSSampleGame/Maps/RTSGameSampleMap.umap in the editor and hit Play. We recommend taking a closer look at that map as reference for your own ones as well.

## Adding The Plugin

Note that the plugin currently requires a C++ Unreal project, which in turn requires a working compiler.

1. Clone the repository.
1. Close the Unreal Editor.
1. Copy the RTSPlugin folder to Plugins folder next to your .uproject file.
1. Right-click your .uproject file and select Re-generate Visual Studio project files.
1. Build the resulting solution in Visual Studio.
1. Start the Unreal Editor.
1. Enable the plugin in Edit > Plugins > RTS.

## Usage

### Adding Units

1. Setup your animations. (If you're new to the Unreal animation system, we can readily recommend the tutorial at https://docs.unrealengine.com/latest/INT/Programming/Tutorials/FirstPersonShooter/4/index.html)

### Selecting Units

#### Enabling Selection

1. Bind the action AddSelection (e.g. to Left Shift).
1. Bind the action ToggleSelection (e.g. to Left Ctrl).

### Setup Minimap

#### Minimap Background Layer

1. Create a new Asset > Materials & Textures > Render Target (e.g. called MinimapBackgroundRenderTarget).
1. Set its Compression Settings to User Interface and Texture Group to UI.
1. Create a Material from your Render Target.
1. Set its Material Domain to User Interface and make sure the texture is connected to Final Color.

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

#### Minimap Fog of War Layer

1. Setup fog of war (see below).
1. Set the FogOfWarMaterial of your RTSMinimapWidget to M_RTSFogOfWarMinimap.
1. In your player controller blueprint (or whichever owns the minimap widget), when setting up (e.g. in BeginPlay):
    1. Use the blueprint function RTSPlayerController::GetTeamInfo to get the team of the local player.
    1. Use the blueprint function GetVisionInfoForTeam to get vision info for the local player.
    1. Call SetupVisionInfo for the minimap widget.
1. In your player controller blueprint (or whichever owns the minimap widget), handle the OnVisionInfoAvailable event and call SetupVisionInfo for the minimap widget. This is required for properly setting up vision info on clients where replication may cause a different initialization order.

### Add Resource Gathering

1. Create an RTSResourceType blueprint for each resource in your game.
1. Add the resource types to your player controller to enable players to keep resources in stock. Listen for the OnResourcesChanged event to update your UI.
1. Create a unit (see above) and add an RTSResourceSourceComponent for each type of resource node in your game (e.g. gold mine, tree).
    1. Set the resource type and maximum and current resources of the source.
    1. Set the gathering factor for increaing the yield of any gatherers (e.g. golden minerals).
    1. Add an RTSContainerComponent, check Gatherer Must Enter and set the Gatherer Capacity if you want gatherers to enter the resource source (e.g. Refinery in StarCraft).
1. Create a unit (see above) and add an RTSResourceDrainComponent for each type of building gatherers may return resources to.
    1. Set the resource types to all resources accepted by the drain.
1. Create an RTSGathererComponent to any unit that should be able to gather resources.
    1. Add any resource type the gatherer should be able to gather to Gathered Resources.
        1. Gathering works similar to attacks, with "damage" and "cooldown". Set Amount Per Gathering to the value to add to the gatherers inventory each time the cooldown is finished.
        1. Set the Cooldown to the time between two gatherings.
        1. Set the Capacity to the amount of resources the gatherer can carry before returning to a resource drain.
        1. Check Needs Return To Drain if the gatherer needs to move to another actor for returning resources (e.g. Age of Empires). Uncheck if they should return all gathered resources immediately when hitting the capacity limit (e.g. Undead in WarCraft).
        1. Set Range as desired.
    1. Add all Resource Source Actor Classes the gatherer may gather from (e.g. Undead in Warcraft need Haunted Gold Mine).
    1. Set the Resource Sweep Radius to the radius in which the gatherer should look for similar resources if their current source is depleted.

### Setup Fog of War

1. Add the RTSVision component to your units and set their Sight Radius (e.g. 1000).
1. Add an RTSVisionVolume to your map, encompassing the whole valid visible map area (e.g. 4096x4096).
1. Set the Size Per Tile of the vision volume (e.g. 16).
1. Add a PostProcessVolume to your map, encompassing the whole valid camera area.
1. Add an RTSFogOfWarActor to your map.
1. Set the Fog Of War Material of the actor (e.g. to the M_RTSFogOfWar material shipped with the plugin).
1. Set the Fog Of War Volume reference to the post process volume created before.

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
