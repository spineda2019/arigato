# Core Game Architecture

While "architecture" may be a bit of a strong word, this document is really just
for the purpose of concisely documented how the project here is organized.

## Structure
The "core" project should be (at least at the time of writing) be split into 2
parts:

* ecs
* impl

The idea is that the ecs subfolder holds, well, the ECS. Code here should be
game agnostic. Not that I'm going to necessarily make more than one, but
architecturally the stuff in ecs (e.g. the ECS) should be reusable, even by a
theoretical future game of which I have no idea how it looks. That means, in
general for `ecs`:

* Code here does not know what a "player", a "character", a "monster", or any
  other type of thing in your game even is.
* Such specifics are _identified_ by the ECS with an ID, but its the user of
  the ECS's job to bookkeep what ID maps to what in the game.
* Thus, if the game, for some reason, wants the ECS to tell it when something
  interesting happens (perhaps between 2 IDs, perhaps), the user needs to
  somehow register an _observer_ (no idea yet how this will look here).
* Thus, in git parlance, `ecs` is plumbing, and knows nothing of the game's
  porcelain.

Expectedly, that means the opposites are generally true for `impl` (aptly named
as it is the _implementation_ of the _specific_ game):

* Code here is responsible for discerning between the IDs the ECS hands back,
  including what is a "player", a "character", a "monster", etc.
* Of course, the ECS is still responsible for creating the entities requested by
  the game.
* Thus, in git parlance, `game` is the porcelain. At least for the most part.
