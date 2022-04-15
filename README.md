# Battleship
A single and multiplayer enabled version of the classic game: Battleship. Developed in C++.

## Features
* 4 Single player difficulties: Easy, Medium, Hard, Expert
* Multiplayer support for playing with friends over a peer-to-peer network
* Unique visuals and game effects


## Enabling PvP
Player vs. Player is facilitated through a peer-to-peer connection between a host user and a connecting user.

Steps to enabled PvP:
1. Change line 35 in Game.h to the `networkStartIP` of the Hosting player
2. Change line 2 `DEFAULT_PORT` in Peer.h to the [port-forwarding](https://www.lifewire.com/how-to-port-forward-4163829) enabled port of the Hosting player
3. Have the Hosting player launch the game and wait for the "Waiting for Opponent..." screen to appear
4. Have the Connecting player launch the game and connect to the Hosting player

The process of enabling PvP is extraordinarily user unfriendly and is the source of major improvements in future iterations.

## Demo
