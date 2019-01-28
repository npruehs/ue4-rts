## Creating Buildings

1. See Creating Units (except for movement and AI).
1. Add an RTSConstructionSiteComponent and set the ConstructionTime.
1. Set the Construction Costs to any resources required for construction.
1. Set the Construction Cost Type to to Pay Immediately if all costs should be paid in full when starting construction, or to Pay Over Time for continuously paying costs (similar to Command & Conquer).
1. Set the Refund Factor to the factor to multiply refunded resources with after cancelation.
1. Set the Consumes Builders flag if builders working at the construction site should be destroyed when finished (similar to Zerg in StarCraft).
1. Set Max Assigned Builders if you want to require a builder to work at the construction site to make progress, and/or to allow multi-building (similar to Age of Empires).
1. Set the Progress Made Automatically and Progress Made Per Builder factors.
1. Set the Start Immediately flag unless you want to trigger construction start from script.
1. Add an RTSContainerComponent if you want builders to enter the building site while building. Its capacity value will be automatically set at runtime to reflect Max Assigned Builders of the construction site.
