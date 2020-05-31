## Creating Resource Sources

In case you missed that step earlier, make your to set up your resource types as explained in [Setup](Setup.md). Then, create resource sources as follows:

1. See [Creating Units](Units.md) (Appearance only; can be a standard actor).
1. Add an `RTSResourceSourceComponent`.
1. Set the resource type and maximum and current resources of the source.
1. Set the gathering factor for increasing the yield of any gatherers (e.g. golden minerals in StarCraft).
1. If you want gatherers to enter the resource source (e.g. Refinery in StarCraft), check _Gatherer Must Enter_, set the _Gatherer Capacity_, and add an `RTSContainerComponent`.
