# Electricity

Electricity is one of the fundamental requirements for structures to operate. Each structure defines:
* if it connects to the power grid;
* if it can produce energy; 
* how much energy it consumes (expressed in kW, an integer);
* energy transmission radius.

The last 3 are present iff structure is connected to the grid. Energy consumption may change over time. Disconnected structures (other than transmission lines) are ignored in the following sections.

## Consumers

If structure can connect to the power grid and cannot produce energy, it is a power consumer. Consumed energy should be non-negative. At any time, consumer has energy provided or not, without middle states. Lack of energy modifies consumer behavior.

Examples: basic factory, public utility building, train station.

## Producers

If structure can produce energy, it is a producer. Producers may produce or consume energy, thus consumed energy can be negative. Producer may start consuming energy instead of producing, and even consume energy initially.

Examples: any power plant, house with a photovoltaic cell.

### Power plant types

**TODO**: Think of power plants

## Power transmission

Subnet is a set of structures connected to grid that make a connected graph (where neighborhood means laying in transmission radius). Note that this does not impose aligning to grid.

For a subnet, if energy production is greater than consumption, then all consumers are working. If consumption is greater, than some buildings are disconnected at random to make up for unfilled demand.

When new structure is placed, subnets may merge. When structure is deleted, it's subnet has to be reevaluated.

**TODO**: Performance considerations. Will running BFS on each deletion do? Maybe keep neighbor count in the graph and regenerate only when demand not met after producer deletion?

### Power lines

Power lines are a special type of a structure which connects subnets. If subnet has surplus energy, it transfers extra amounts to other subnets. Connected subnet can consume energy from multiple parent subnets. Unused energy transfers to other subnets.

**TODO**: What happens when there are two subnets with producers, connected by a power line and one subnet is not meeting the demand?

## Power access visualization

Data layer keeps electricity access for each tile. Then this tileset can be drawn on the terrain surface for each tile.

**TODO**: Should this data layer be binary? It may look blocky if tiles are comparable to buildings.

## References and examples

* [IGN SimCity Wiki Guide: "Power"](https://www.ign.com/wikis/simcity/Power)
* [Factorio Blog: "Friday Facts #115 - The power switch"](https://www.factorio.com/blog/post/fff-115)
* [Power lines in Cities: Skylines](https://www.youtube.com/watch?v=rGInUXK9UEE)
* [SimCity Wiki: List of power plants](https://simcity.fandom.com/wiki/List_of_power_plants)
* [Gamedev SE question about Prison Architect](https://gamedev.stackexchange.com/q/115327/48182)
* [Gamedev SE question about Minecraft](https://gamedev.stackexchange.com/q/109129/48182)