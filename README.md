# Cyclic Dungeon Generation Prototype in SDL2 - Procedural Generation

Graph grammar can be used for a variety of purposes, such as level generation, item generation, and quest structure generation. The inspiration for this project stems from the game Unexplored, with the primary goal of gaining a deeper understanding of Graph Grammar, Cyclic Dungeon Generation, and procedural generation.

![The Application](/application.PNG)

## Level Elements
The levels are dynamic and include various elements like minibosses, rewards, traps, and lock & keys that are placed throughout the level based on the chosen cyclic rules. These elements are designed in an abstract manner, allowing for endless possibilities. For instance, minibosses can range from giant spiders to cyborgs, while rewards can include anything from magic swords to speed-enhancing potions. Traps can be diverse, from explosive barrels to hidden pitfalls. The lock and key can come in many forms, such as a lever, keycode, or hazmat suit. All of these factors depend on the biome, level difficulty, character level, and other game parameters.

## Cyclic Rules
A cycle is a circular loop of linked dungeon rooms designed to create a certain gameplay flow, such as a lock and key or hidden shortcut. By nesting several cycles together, a dungeon's builder can create a flowchart representing an engaging non-linear space. Each cycle described in this document has four elements: a start, a goal, arcs, and insertion points.

This prototype only supports one main rule: it specifies the length of both arcs (short or long), the type of element connected to each arc (miniboss, trap, lock & key), and the end goal of the mission (kill target, secret document, or a reward). Each element has unique data, such as rarity, reward type, and the number of rooms the miniboss patrols, but unfortunately. Unfortunately, the unique data is not being displayed in the prototype. However, it has support for it.

![Cyclic Level](/cyclic-level.png)
