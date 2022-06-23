# Action Roguelike Game (Course Project)

Project for the **[Professional Game Development in C++ and Unreal Engine](https://courses.tomlooman.com/p/unrealengine-cpp?coupon_code=COMMUNITY15&src=github)** Course

# Project Features
- Third-person Action Character Movement
- **Action System** (similar to Gameplay Ability System in design)
  - Dash Ability (Teleporting via projectile)
  - Blackhole Ability
  - Magic Projectile Attack
  - "Thorns" buff (reflecting damage)
  - Burning Damage-over-time effect
- AttributeComponent (Holding health etc.)
- **SaveGame System** for persisting progress of character and world state.
- Heavy use of Events to drive UI and gameplay reactions.
- Mix of C++ & Blueprint and how to combine these effectively.
- **GameplayTags** to mark-up Actors, Buffs, Actions.
- **Multiplayer support** for all features
- GameMode Logic
  - EQS for binding bot/powerup spawn locations.
  - Bot spawning system (bots cost points to spawn, gamemode gains points over time to spend)
  - DataTable holds bot information
  - DataAssets to hold enemy configurations
- **Asset Manager:** Async loading of data assets
- Async loading of UI icons
- AI
  - Minion AI with Behavior Trees (Roam, See, Chase, Attack, Flee/Heal)
  - C++ Custom Behavior Trees Nodes
  - EQS for attack/cover locations by AI
Powerups
- Powerup pickups to heal, gain credits/actions.
UMG
- Main menu to host/join game
- UI elements for player attributes and projected widgets for powerups and enemy health.
- C++ Localized Text

Game Assets: Licensed for use with the Unreal Engine only.