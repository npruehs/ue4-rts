## Scripting

Occasionally, you want to create additional gameplay (especially when creating a story campaign). This section highlights additional functions you can call from blueprints, as well as events you may handle.

Feel free to explore the plugin yourself by looking at what other functions and events each component provides, and open an issue if you're missing something.


### RTSContainerComponent
#### Functions

| Node | Description |
| --- | --- |
| ![Load Actor](Images/LoadActor.png) | Adds the specified actor to this container. |
| ![Unload Actor](Images/UnloadActor.png) | Removes the specified actor from this container. |


### RTSConstructionSiteComponent
#### Events

| Event | Description |
| --- | --- |
| ![On Construction Finished](Images/OnConstructionFinished.png) | Event when the construction timer has expired. |


### RTSGameMode
#### Functions

| Node | Description |
| --- | --- |
| ![Transfer Ownership](Images/TransferOwnership.png) | Sets the specified player as the owner of the passed actor. |


### RTSHealthComponent
#### Events

| Event | Description |
| --- | --- |
| ![On Health Changed](Images/OnHealthChanged.png) | Event when the current health of the actor has changed. |
| ![On Killed](Images/OnKilled.png) | Event when the actor has been killed. |


### RTSPawnAIController
#### Functions

| Node | Description |
| --- | --- |
| ![Issue Attack Order](Images/IssueAttackOrder.png) | Makes the pawn attack the specified target. |
| ![Issue Begin Construction Order](Images/IssueBeginConstructionOrder.png) |  Makes the pawn construct the specified building at the passed location. |
| ![Issue Gather Order](Images/IssueGatherOrder.png) | Makes the pawn gather resources from the specified source. |
| ![Issue Move Order](Images/IssueMoveOrder.png) | Makes the pawn move towards the specified location. |
| ![Issue Stop Order](Images/IssueStopOrder.png) | Makes the pawn stop all actions immediately. |

#### Events

| Event | Description |
| --- | --- |
| ![On Order Changed](Images/OnOrderChanged.png) | Event when the pawn has received a new order. |


### RTSPlayerResourcesComponent
#### Functions

| Node | Description |
| --- | --- |
| ![Add Resources](Images/AddResources.png) | Adds the specified resources to the stock of this player. |

#### Events

| Event | Description |
| --- | --- |
| ![On Resources Changed](Images/OnResourcesChanged.png) | Event when the current resource stock amount for the player has changed. |


### RTSProductionComponent
#### Events

| Event | Description |
| --- | --- |
| ![On Production Finished](Images/OnProductionFinished.png) | Event when the production timer has expired. |
