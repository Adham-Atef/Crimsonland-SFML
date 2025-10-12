# CrimsonLand - Zombie Survival Game

## Overview

CrimsonLand is a 2D top-down zombie survival game built with C++ and SFML (Simple and Fast Multimedia Library). Players fight waves of zombies across multiple levels using various weapons in both single-player and multiplayer modes.

## Game Features

### Game Modes

- **Story Mode**: 12 progressive levels with narrative elements

  - Beach, Desert Road, City environments
  - Safe Zones for temporary respite
  - Special Missions with unique objectives
  - Army Camp challenges

- **Rush Mode**: Fast-paced survival with increasing difficulty
  - Single Player and Multiplayer variants
  - Score-based gameplay
  - Multiple map environments
  - Leaderboard system

### Weapons System

The game features a comprehensive weapon system with multiple weapon types:

- **Knife**: Melee weapon with 35 damage
- **Pistol**: Basic ranged weapon
- **Rifle**: High-damage precision weapon
- **Shotgun**: Close-range spread damage
- **Plasma Weapons**: Advanced energy-based weapons
  - Plasma Rifle
  - Plasma Shotgun
  - Plasma Pistol
  - Burst Rifle

Each weapon has unique characteristics including:

- Clip size and reload mechanics
- Damage values
- Fire rate
- Ammo consumption

### Player Features

- **Health System**: Player health management with visual indicators
- **Crosshair System**: Dynamic targeting system
- **Multiple Players**: Support for up to 2 players in multiplayer mode
- **Score System**: Points awarded for zombie eliminations
- **Double Score Power-up**: Temporary score multiplier

### Environment & Graphics

- **Multiple Maps**: Beach, Desert Road, City, Woods, Army Camp
- **Interactive Elements**: Buildings, vehicles, trains, decorative objects
- **Dynamic Lighting**: Visual effects and atmospheric elements
- **Particle Systems**: Muzzle flashes, explosions, and environmental effects

### Audio System

- **Background Music**: Immersive soundtrack
- **Sound Effects**: Weapon sounds, zombie noises, environmental audio
- **Volume Controls**: Separate music and SFX volume settings

### Game Mechanics

- **Zombie AI**: Intelligent zombie behavior with pathfinding
- **Collision Detection**: Precise hit detection for bullets and zombies
- **Spawn System**: Dynamic zombie spawning with increasing difficulty
- **Mission Objectives**: Varied goals across different levels
- **Timer Systems**: Mission time limits and zombie spawn timers

## Technical Details

### Dependencies

- **SFML 2.x**: Graphics, audio, and input handling
- **C++ Standard Library**: STL containers and algorithms
- **Random Number Generation**: For procedural elements

### File Structure

The game uses a single large C++ file (`cpp.cpp`) containing:

- Game state management
- Entity systems (Player, Zombie, Bullet, Weapon)
- Level implementations
- UI and menu systems
- Audio and graphics management

### Key Classes/Structures

- `PLAYER`: Player character with weapons and health
- `ZOMBIE`: Enemy entities with AI behavior
- `Weapon`: Weapon system with damage and ammo
- `Bullet`: Projectile physics and collision
- `levelHandler`: Game level management
- Various level-specific structures (Beachlevel, City, etc.)

## Controls

### Keyboard Controls

- **WASD/Arrow Keys**: Player movement
- **Mouse**: Aiming and shooting
- **Space**: Weapon switching
- **R**: Reload weapon
- **P**: Skip level (debug)
- **Escape**: Pause menu

### Gamepad Support

- Full controller support for both players
- Analog stick movement
- Trigger controls for shooting

## Installation & Setup

### Prerequisites

1. C++ compiler with C++11 support or later
2. SFML 2.x library
3. CMake (optional, for build automation)

### Building

1. Install SFML development libraries
2. Compile with SFML linking:
   ```bash
   g++ -std=c++11 cpp.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o crimsonland
   ```

### Required Assets

The game expects the following assets in the project directory:

- `tag.ttf` - Main font file
- `ModernDOS8x14.ttf` - UI font
- `crimsonMain.png` - Main menu background
- `logo.png` - Game logo
- Various texture files in `imgs/` directory
- Audio files: `MainSound.wav`, `Button.wav`, and weapon sound effects

## Gameplay

### Story Mode Progression

1. **Beach Level**: Introduction to basic mechanics
2. **Desert Road**: Vehicle obstacles and open combat
3. **City**: Urban environment with buildings
4. **Safe Zones**: Rest areas between major battles
5. **Woods**: Forest environment with limited visibility
6. **Army Camp**: Military-themed final area
7. **Special Missions**: Unique objectives and challenges

### Scoring System

- **Zombie Elimination**: 5 points per zombie
- **Double Score Power-up**: 2x multiplier
- **Leaderboard**: Top 10 high scores saved locally
- **Score Persistence**: Scores saved between sessions

## Multiplayer Features

- **Split-screen**: Two players on same screen
- **Independent Scoring**: Each player has separate score tracking
- **Shared Objectives**: Both players work toward same goals
- **Balanced Difficulty**: Adjusted for two-player gameplay

## Technical Architecture

### Game Loop

The game uses a standard game loop pattern:

1. Handle input events
2. Update game state
3. Render graphics
4. Manage audio

### Entity Management

- Vector-based storage for dynamic entities
- Efficient collision detection algorithms
- Memory management for spawning/despawning

### Performance Optimizations

- 60 FPS target framerate
- Efficient rendering with SFML sprites
- Optimized collision detection
- Memory pooling for frequently created objects

## Development Notes

### Code Structure

- Single-file architecture for simplicity
- Extensive use of structs for data organization
- Global state management
- Procedural level generation elements

### Future Enhancements

The codebase suggests potential for:

- Additional weapon types
- More complex AI behaviors
- Extended level content
- Enhanced visual effects

## Credits

This appears to be a student/academic project demonstrating:

- Game development with C++ and SFML
- 2D graphics programming
- Game state management
- Multiplayer game architecture
- Audio integration in games

---

**Note**: This is a standalone C++ game project and is not related to the React Wordle project in the same directory.
