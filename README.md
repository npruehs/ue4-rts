[![license](https://img.shields.io/github/license/npruehs/ue4-rts.svg?maxAge=2592000)](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE)

# Real-Time Strategy Plugin for Unreal Engine 4

Open source Real-Time Strategy Plugin for Unreal Engine 4 developed by the original creators of [Hostile Worlds](http://www.indiedb.com/games/hostile-worlds/) for Unreal Engine 3.

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
* AI Players
* Cheats

All of this is already completely working in multiplayer as well, and has been fully exposed to scripting, enabling you to update UI and play animations and sounds.

We're going to add all of this to the Unreal Marketplace for free as well soon (tm).

For a quick look, just open `Source/RTSProject/RTSProject.uproject`,  load `Maps/SKM-DarkSpace.umap` in the editor and hit Play. We recommend taking a closer look at that map (and the rest of the example project) as reference for your own one as well.


## Setup

### Prerequisites

Real-Time Strategy Plugin for Unreal Engine 4 currently supports the following Unreal Engine Versions:

* 4.24
* 4.25

### Adding The Plugin (Blueprint Project)

1. Close your Unreal Editor.
1. [Download](https://github.com/npruehs/ue4-rts/releases) the latest release.
1. Copy the `RealTimeStrategy` folder to `Plugins` folder next to your `.uproject` file (create if necessary).
1. Start the Unreal Editor.

### Adding The Plugin (C++ Project)

1. Close your Unreal Editor.
1. [Clone](https://github.com/npruehs/ue4-rts) the repository or [download](https://github.com/npruehs/ue4-rts/releases) a release.
1. Copy the `RealTimeStrategy` folder to `Plugins` folder next to your `.uproject` file (create if necessary).
1. Right-click your .uproject file and select Re-generate Visual Studio project files.
1. If you want to create C++ subclasses of the plugin classes, add `"RealTimeStrategy"` to the `PublicDependencyModuleNames` of the `Build.cs` file of your project. Note that you might need to add other dependencies as well, depending on the respective class you want to extend. Take a look at `RealTimeStrategy.Build.cs` of the plugin for a full list of referenced engine modules.
1. Build the resulting solution in Visual Studio.
1. Start the Unreal Editor.


## Getting Started

Take a look at the [official manual](Documents/Manual/Manual.md) to get started right away!


## Bugs & Feature Requests

We are sorry that you've experienced issues or are missing a feature! After verifying that you are using the latest version and having checked whether a [similar issue](https://github.com/npruehs/ue4-rts/issues) has already been reported, feel free to [open a new issue](https://github.com/npruehs/ue4-rts/issues/new). In order to help us resolving your problem as fast as possible, please include the following details in your report:

* Steps to reproduce
* What happened?
* What did you expect to happen?

After being able to reproduce the issue, we'll look into fixing it immediately.


## Development Cycle

We know that using this plugin in production requires you to be completely sure about stability and compatibility. Thus, new releases are created using [Semantic Versioning](http://semver.org/). In short:

* Version numbers are specified as MAJOR.MINOR.PATCH.
* MAJOR version increases indicate incompatible API changes.
* MINOR version increases indicate added functionality in a backwards compatible manner.
* PATCH version increases indicate backwards compatible bug fixes.

You'll always find all available releases and their respective release notes at:

https://github.com/npruehs/ue4-rts/releases


## Contributing

You want to contribute to  Real-Time Strategy Plugin for Unreal Engine 4? Great! Take a look at [Contributing](CONTRIBUTING.md) to get started right away!


## License

Real-Time Strategy Plugin for Unreal Engine 4 is released under the [MIT License](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE).
