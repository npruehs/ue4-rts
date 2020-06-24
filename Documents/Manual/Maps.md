## Creating Maps

For the plugin, you'll design and create your maps the same way you're used to when using Unreal Engine, for the most part. This section serves as a short checklist for you, and highlights some setup that is supposed to make it easier for you to get started. Some steps are mandatory for some features of the plugin to work, however, such as vision.


### Game Mode & Geometry

1. Use your game mode in the world settings.
1. Create your level geometry and lighting as usual.

### Camera

1. Add an `RTSCameraBoundsVolume` to the map.
1. Use the _Brush Settings_ to adjust the camera bounds as desired.

### Navigation

1. Add a `NavMeshBoundsVolume` to the map.
1. Use the _Brush Settings_ to adjust have the nav mesh bounds encompass your whole level.
1. Build navigation. You may press P to review your results in the viewport.

### Player Starts

1. Add `RTSPlayerStart`s to the map.
1. Set the _Team Index_ for each player start.

### Minimap

1. Add an `RTSMinimapVolume` to the very center of your map.
1. Set its brush size to match the extents of your playable map.
1. Set the _Minimap Image_ to a nice top-down screenshot of your map.

### Fog Of War

1. Add an `RTSVisionVolume` to the very center of your map, encompassing the whole valid visible map area.
1. Set the _Size In Tiles_ of the vision volume to match your minimap background images (e.g. 256).
1. Set the _Tile Height_ of the vision volume to the height of a single height level of your map, in cm (e.g. 250).
1. Set the _Height Level Trace Channel_ if you want special geometry to affect your height levels, only.
1. Add a `PostProcessVolume` to your map, and check _Infinite Extent (Unbound)_.
1. Add an `RTSFogOfWarActor` to your map.
1. Set the _Fog Of War Volume_ reference to the post process volume created before.
1. Set the _Fog Of War Material_ of the actor (e.g. to the `M_RTSFogOfWar` material shipped with the plugin).

### Pre-Placed Units

1. Add any actors that should initially on the battlefield.
1. For each of these actors, at the `RTSOwnerComponent`, set the _Initial Owner Player Index_ to specify which player should own them.
1. When pre-placing buildings, at the `RTSConstructionSiteComponent`, set their _State_ to _Finished_ if they should be ready from the beginning.
