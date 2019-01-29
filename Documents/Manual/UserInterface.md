## Creating The User Interface

#### Showing Selection Frames

In your HUD, implement the DrawSelectionFrame and HideSelectionFrame events as desired.

Example:

1. Create a widget for drawing the selection frame.

![Selection Frame Widget](Images/SelectionFrameWidget.png)

2. Add your widget to any kind of user interface your player controller knows about.
3. In that user interface, provide a function for showing the selection frame.

![UI - Show Selection Frame](Images/UIShowSelectionFrame.png)

3. In the user interface, provide a function for hiding the selection frame.

![UI - Hide Selection Frame](Images/UIHideSelectionFrame.png)

4. In your HUD, forward the DrawSelectionFrame event to your UI.

![HUD - Show Selection Frame](Images/HUDDrawSelectionFrame.png)

5. In the HUD, forward the HideSelectionFrame event to your UI.

![HUD - Hide Selection Frame](Images/HUDHideSelectionFrame.png)

### Showing Selected Unit Status

1. Create a new widget blueprint.
1. Create the widget where appropriate (e.g. BeginPlay of your player controller) and add it to your viewport.
1. Listen to the OnSelectionChanged event broadcasted by the RTSPlayerController and update your UI.
