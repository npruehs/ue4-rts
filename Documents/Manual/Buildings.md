## Creating Buildings

From the perspective of the plugin, buildings are just units with a few additional components. There's no special class for buildings; their setup has just been moved to this manual section because that many people would explicitly look for that. In fact, you can mix and match the setup outlined in this section with all of the other sections. This allows you to create truly deep gameplay, such as units that serve as resource sources, or produce other units.


### Construction

1. See [Creating Units](Units.md) (Appearance, Health & Damage, Projectiles).
1. Add an `RTSConstructionSiteComponent` and set the _Construction Time_.
1. Set the _Construction Costs_ to any resources required for construction.
1. Set the _Construction Cost Type_ to to _Pay Immediately_ if all costs should be paid in full when starting construction, or to _Pay Over Time_ for continuously paying costs (similar to Command & Conquer).
1. Set the _Refund Factor_ to the factor to multiply refunded resources with after cancelation.
1. Set the _Consumes Builders_ flag if builders working at the construction site should be destroyed when finished (similar to Zerg in StarCraft).
1. Set _Max Assigned Builders_ if you want to require a builder to work at the construction site to make progress, and/or to allow multi-building (similar to Age of Empires).
1. Set the _Progress Made Automatically_ and _Progress Made Per Builder_ factors.
1. Set _Initial Health Percentage_ to a value between 0 and 1 to specify how much health the construction site should start with.
1. Set the _Start Immediately_ flag unless you want to trigger construction start from script.
1. Add an `RTSContainerComponent` if you want builders to enter the building site while building. Its capacity value will be automatically set at runtime to reflect _Max Assigned Builders_ of the construction site.
1. Add your `RTSConstructionProgressBarWidgetComponent` (see [User Interface](UserInterface.md)).

### Production

1. Add an `RTSProductionComponent` to any actors you want to be able to produce units or research technology.
1. Add everything you want to produce or research to the _Available Products_ for these factories.
1. Set the _Queue Count_, specifying how many products can be produced in parallel.
1. Set the _Capacity Per Queue_, specifying how many products can be produced one after another.
1. Add your `RTSProductionProgressBarWidgetComponent` (see [User Interface](UserInterface.md)).

_Note that, technically, producing units does not differ from researching technology. You can create actor blueprints without physical representation for each technology to research, and add them as products. Then, you can check whether any player owns an actor of that technology for checking a tech tree._

### Resource Drain

1. Add an `RTSResourceDrainComponent` for each type of building gatherers may return resources to.
1. Set the resource types to all resources accepted by the drain.

### Defense

1. If your building has an `RTSAttackComponent`, check _Preview Attack Range_ if you want the attack range of your building to be previewed while placing the building.

### Vision

1. At the `RTSVisible`component of your building, check _Don't Hide After Seen_ if you want your building to stay visible even through for of war.

### Projectile Impacts

1. While your building is under attack, you might want to prevent every single projectile hitting the exact same location. You can add a `RTSProjectileTargetComponent` to your actor, and specify the _Target Sockets_ to have projectile fly towards. These sockets have to be added to your static mesh using the built-in Unreal socket manager.
2. If your building has multiple mesh components, add a _component tag_ to the desired mesh component and specify the same tag at your `RTSProjectileTargetComponent`.
