## Creating Maps

### Game Mode & Geometry

1. Use your game mode in the world settings.
1. Create your level geometry and lighting as usual.

### Camera

1. Add an RTSCameraBoundsVolume to the map.
1. Use the Brush Settings to adjust the camera bounds as desired.

## Navigation

1. Add a NavMeshBoundsVolume to the map.
1. Use the Brush Settings to adjust have the nav mesh bounds encompass your whole level.
1. Build navigation. You may press P to review your results in the viewport.

### Player Starts

1. Add your player starts to the map.
1. Set the Team Index for each player start.

### Minimap

1. Add an RTSMinimapVolume to the very center of your map.
1. Set its brush size to match the extents of your playable map.
1. Set the Minimap Image to a nice top-down screenshot of your map.

### Fog Of War

1. Add an RTSVisionVolume to the very center of your map, encompassing the whole valid visible map area.
1. Set the Size In Tiles of the vision volume to match your minimap background images (e.g. 256).
1. Add a PostProcessVolume to your map, and check Ininite Extent (Unbound).
1. Add an RTSFogOfWarActor to your map.
1. Set the Fog Of War Volume reference to the post process volume created before.
1. Set the Fog Of War Material of the actor (e.g. to the M_RTSFogOfWar material shipped with the plugin).
