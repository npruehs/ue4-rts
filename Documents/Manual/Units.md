## Creating Units

As mentioned before, most features of the plugin are implemented by the means of components to add to your actors. Thus, for adding new units (or buildings), you'll usually create a pawn or character, and add various components, depending on the capabitilies you want the unit to have. This approach enables you to combine features just as you like, for instance having buildings that can attack or units that resources can be returned to.


### Appearance

1. Create a new pawn or character blueprint.
1. Check the _Replicates_ flag.
1. Add a static or skeletal mesh and setup its location, rotation and scale as usual.
1. Setup collision (e.g. Capsule Collision) as usual. You may want to disable the collision of your mesh and rely on its capsule instead.
1. Setup your animations. (If you're new to the Unreal animation system, we can readily recommend the tutorial at https://docs.unrealengine.com/latest/INT/Programming/Tutorials/FirstPersonShooter/4/index.html)
1. Add an `RTSNameComponent` and set its localized name if you want to show it in any kind of ingame UI. 
1. Add an `RTSDescriptionComponent` and set its localized text if you want to show it in any kind of ingame UI. 
1. Add an `RTSPortraitComponent` and set its portrait if you want to show it in any kind of ingame UI.
1. Add an `RTSSelectableComponent`, and set its selection circle material (e.g. to `M_RTSSelectionCircle`) and selection sound.
1. Add an `RTSOwnerComponent`. This will be used to store (and replicate) the owner of the unit for all players (e.g. for showing team colors).
1. Add your `RTSHoveredActorWidgetComponent` (see [User Interface](UserInterface.md)).


### Movement

1. Add a movement component (e.g. `FloatingPawnMovement` or `CharacterMovement`) and set up its speed properties as usual. The plugin also ships with a `RTSPawnMovementComponent` that basically just adds rotation updates to the built-in `FloatingPawnMovement`.


### Vision

1. Add the `RTSVision` component to your units and set their _Sight Radius_ (e.g. 1000).


### Combat

#### AI

As mentioned before, units are not directly possessed by player controllers in the plugin. Instead, every unit is possessed by an AIController that will carry out orders issued by the players.

1. Set the pawn AI controller class to your `RTSPawnAIController`.
1. Ensure _Pawn > Auto Possess AI_ is set to _Placed in World or Spawned_.

#### Health & Damage

1. Add an `RTSAttackableComponent` and `RTSGameplayTagsComponent` to any actors that can be attacked.
1. Add the `Status.Permanent.CanBeAttacked` tag to the `RTSGameplayTagsComponent`.
1. Set the _Maximum Health_ of the `RTSHealthComponent`.
1. If you want to play animations or visual effects when the actor dies, set _Actor Death Type_ to _Stop Gameplay_. In that case, you're responsible of destroying the actor yourself as soon as all of your visual clues have finished playing.
1. Add your `RTSHealthBarWidgetComponent` (see [User Interface](UserInterface.md)).

1. Add the `RTSAttackComponent` to any actors than can attack.
1. Add an attack to the `RTSAttackComponent` of these actors, setting its _Cooldown, Damage, Range, Acquisition Radius_ and _Chase Radius_.

_Setting the Damage Type is optional._

#### Projectiles

If you don't specify a projectile, the damage will be applied immediately. In order to setup a projectile for the unit:

1. Create an actor deriving from `RTSProjectile`.
1. Add a static mesh and any visual effects.
1. At the `ProjectileMovement` component, set its _Initial Speed_ (e.g. to 1000).
1. At the `RTSAttackComponent`, reference the new projectile in your attack.


### Production Costs

1. Add the `RTSProductionCostComponent` to everything you want to be produced.
1. Set the _Production Time_ for these products.
1. Set the _Resources_ to any resources required for production.
1. Set the _Production Cost Type_ to to _Pay Immediately_ if all costs should be paid in full when starting production, or to _Pay Over Time_ for continuously paying costs (similar to Command & Conquer).
1. Set the _Refund Factor_ to the factor to multiply refunded resources with after cancelation.
1. Add the `RTSRequirementsComponent` if the actor should have any requirements, and set the _Required Actors_.


### Construction

1. Add an `RTSBuilderComponent` to any actors you want to be able to construct buildings.
1. Set the _Constructible Building Classes_ for these builders.
1. Check _Enter Construction Site_ if you want the builder to be unavailable while building (similar to Orcs in WarCraft). In that case, add a `RTSContainableComponent` as well.


### Gathering

1. Add an `RTSGathererComponent` to any actor that should be able to gather resources.
1. Add any resource type the gatherer should be able to gather to _Gathered Resources_.
    1. Gathering works similar to attacks, with "damage" and "cooldown". Set _Amount Per Gathering_ to the value to add to the gatherers inventory each time the cooldown is finished.
    1. Set the _Cooldown_ to the time between two gatherings.
    1. Set the _Capacity_ to the amount of resources the gatherer can carry before returning to a resource drain.
    1. Check _Needs Return To Drain_ if the gatherer needs to move to another actor for returning resources (e.g. Age of Empires). Uncheck if they should return all gathered resources immediately when hitting the capacity limit (e.g. Undead in WarCraft).
    1. Set _Range_ as desired.
1. Add all _Resource Source Actor Classes_ the gatherer may gather from (e.g. Undead in Warcraft need Haunted Gold Mine).
1. Set the _Resource Sweep Radius_ to the radius in which the gatherer should look for similar resources if their current source is depleted.


### Bounties

1. Add an `RTSBountyComponent` to any actors you want to grant bounties to killing players.
1. Set the _Bounty_ for these actors.
