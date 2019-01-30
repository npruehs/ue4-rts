## Creating Units

### Appearance & Movement

1. Create a new RTSPawn or RTSCharacter blueprint.
1. Add a static or skeletal mesh and setup its location, rotation and scale as usual.
1. Setup collision (e.g. Capsule Collision) as usual.
1. Add a movement component (e.g. FloatingPawnMovement or CharacterMovement) and 
set up its speed properties as usual.
1. Set the pawn AI controller class to your RTSPawnAIController.
1. Add an RTSSelectableComponent, and set its selection circle material (e.g. to M_RTSSelectionCircle) and selection sound.
1. Add an RTSOwnerComponent.
1. Ensure Pawn > Auto Possess AI is set to "Placed in World or Spawned".
1. You may want to disable the collision of your mesh and rely on its capsule instead.
1. Add an RTSNameComponent and set its localized name if you want to show it in any kind of ingame UI. 
1. Add an RTSDescriptionComponent and set its localized text if you want to show it in any kind of ingame UI. 
1. Add an RTSPortraitComponent and set its portrait if you want to show it in any kind of ingame UI.

### Combat

#### Health & Damage

1. Add an RTSAttackableComponent and RTSGameplayTagsComponent to any actors that can be attacked.
1. Add the Status.Permanent.CanBeAttacked tag to the RTSGameplayTagsComponent.
1. Set the Maximum Health of the RTSHealthComponent.
1. Add your RTSHealthBarWidgetComponent.

1. Add the RTSAttackComponent to any actors than can attack.
1. Add an attack to the RTSAttackComponent of these actors, setting its Cooldown, Damage, Range, Acquisition Radius and Chase Radius.

_Setting the Damage Type is optional._

#### Projectiles

If you don't specify a projectile, the damage will be applied immediately. In order to setup a projectile for the unit:

1. Create an actor deriving from RTSProjectile.
1. Add a static mesh and any visual effects.
1. At the ProjectileMovement component, set its Initial Speed (e.g. to 1000).
1. At the RTSAttackComponent, reference the new projectile in your attack.

### Production

1. Add the RTSProductionCostComponent to everything you want to be produced.
1. Set the Production Time for these products.
1. Set the Resources to any resources required for production.
1. Set the Production Cost Type to to Pay Immediately if all costs should be paid in full when starting production, or to Pay Over Time for continuously paying costs (similar to Command & Conquer).
1. Set the Refund Factor to the factor to multiply refunded resources with after cancelation.

### Construction

1. Add an RTSBuilderComponent to any actors you want to be able to construct buildings.
1. Set the Constructible Building Classes for these builders.
1. Set the Enter Construction Site if you want the builder to be unavailable while building (similar to Orcs in WarCraft).
