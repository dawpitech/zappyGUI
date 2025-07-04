# Zappy GUI

A graphical client for the Zappy network game, providing real-time visualization of the game world and player activities.

## Overview

The Zappy GUI is a C++ application that connects to the Zappy server to display a visual representation of the game world. It shows the tile-based map, resources, players, and various game events in real-time.

## Features

- **Real-time Game Visualization**: Watch the game unfold with live updates
- **2D Map Display**: Clear visualization of the game world with tiles and resources
- **Player Tracking**: Monitor player positions, levels, and actions
- **Resource Monitoring**: View resource distribution across the map
- **Team Information**: Display team names and player counts
- **Game Events**: Visual feedback for incantations, broadcasts, and other actions

## Requirements

- C++ compiler with C++11 support or higher
- SFML library (recommended for 2D rendering)
- Network connectivity to Zappy server

## Compilation

Use CMake to compile the GUI:

```bash
cmake -S . -B .build/
cmake --build .build
```

## Usage

### Basic Usage

```bash
./zappy_gui -p port -h machine
```

### Command Line Options

| Option | Description | Required |
|--------|-------------|----------|
| `-p port` | Port number of the Zappy server | Yes |
| `-h machine` | Hostname/IP address of the server | Yes |

### Example

```bash
./zappy_gui -p 4242 -h localhost
```

## Architecture

### Connection Protocol

1. The GUI connects to the server using TCP sockets
2. Authenticates by sending `GRAPHIC` as the team name
3. Receives initial world state and configuration
4. Continuously receives updates from the server

### Communication Protocol

The GUI communicates with the server using a specific protocol. Key message types include:

#### Server to Client Messages

| Command | Format | Description |
|---------|--------|-------------|
| `msz` | `msz X Y\n` | Map size |
| `bct` | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` | Tile content |
| `mct` | Multiple `bct` messages | Complete map content |
| `tna` | `tna N\n` | Team names |
| `pnw` | `pnw #n X Y O L N\n` | New player connection |
| `ppo` | `ppo #n X Y O\n` | Player position |
| `plv` | `plv #n L\n` | Player level |
| `pin` | `pin #n X Y q0 q1 q2 q3 q4 q5 q6\n` | Player inventory |
| `pic` | `pic X Y L #n #n ...\n` | Incantation start |
| `pie` | `pie X Y R\n` | Incantation end |
| `seg` | `seg N\n` | End of game |

#### Client to Server Commands

| Command | Format | Description |
|---------|--------|-------------|
| `msz` | `msz\n` | Request map size |
| `bct` | `bct X Y\n` | Request tile content |
| `mct` | `mct\n` | Request complete map |
| `tna` | `tna\n` | Request team names |
| `ppo` | `ppo #n\n` | Request player position |
| `plv` | `plv #n\n` | Request player level |
| `pin` | `pin #n\n` | Request player inventory |
| `sgt` | `sgt\n` | Request time unit |
| `sst` | `sst T\n` | Set time unit |

## Resource Types

The game features 7 types of resources:

| ID | Resource | Symbol |
|----|----------|---------|
| 0 | Food | q0 |
| 1 | Linemate | q1 |
| 2 | Deraumere | q2 |
| 3 | Sibur | q3 |
| 4 | Mendiane | q4 |
| 5 | Phiras | q5 |
| 6 | Thystame | q6 |

## Player Information

### Orientation
- 1: North (N)
- 2: East (E)
- 3: South (S)
- 4: West (W)

### Levels
Players can advance through 8 levels (1-8), with level 8 being the maximum.

## Game Events

The GUI displays various game events:

- **Player Movement**: Real-time position updates
- **Resource Collection**: Visual feedback when players pick up items
- **Incantations**: Special effects during elevation rituals
- **Broadcasts**: Visual indicators for player communications
- **Ejections**: Animation when players are pushed from tiles
- **Egg Laying**: Display of new eggs on the map
- **Player Deaths**: Removal of players from the visualization

## Implementation Guidelines

### Buffering
- Implement proper input/output buffering for network communication
- Handle partial messages and message queuing
- Ensure non-blocking network operations

### Optimization
- Use efficient data structures for map representation
- Implement selective updates (only redraw changed tiles)
- Optimize rendering performance for large maps

### Error Handling
- Handle network disconnections gracefully
- Validate incoming protocol messages
- Provide meaningful error messages to users

## Development Notes

### SFML Integration
If using SFML for 2D rendering:

```cpp
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// Basic window setup
sf::RenderWindow window(sf::VideoMode(800, 600), "Zappy GUI");
```

### Threading Considerations
- Use separate threads for network I/O and rendering if needed
- Ensure thread-safe access to shared game state
- Consider using event-driven architecture

## Testing

- Test with the reference server implementation
- Verify protocol compliance with provided specifications
- Test with various map sizes and player counts
- Validate performance with high-frequency updates

## Troubleshooting

### Common Issues

1. **Connection Failed**: Check server address and port
2. **Protocol Errors**: Verify message format compliance
3. **Performance Issues**: Optimize rendering and update loops
4. **Display Problems**: Check graphics library installation

### Debug Mode
Enable verbose logging to troubleshoot protocol communication:

```bash
./zappy_gui -p 4242 -h localhost --debug
```

## Contributing

When contributing to the GUI:

1. Follow the existing code style and architecture
2. Ensure protocol compliance
3. Test thoroughly with various scenarios
4. Document any new features or changes
5. Maintain compatibility with the reference server

## License

This project is part of the Epitech B-YEP-400 curriculum.
