# Battleship
A single and multiplayer enabled version of the classic game: Battleship. Developed in C++.

## Features
* 4 single player difficulties: Easy, Medium, Hard, Expert
* Multiplayer support for playing with friends over a peer-to-peer network
* Unique visuals and game effects

## Enabling PvP
Player vs. Player is facilitated through a peer-to-peer connection between a hosting user and a connecting user.

Steps to enable PvP:
1. Change line 35 in Game.h to the `networkStartIP` of the Hosting player
2. Change line 2 `DEFAULT_PORT` in Peer.h to the [port-forwarding](https://www.lifewire.com/how-to-port-forward-4163829) enabled port of the Hosting player
3. Have the Hosting player launch the game and wait for the "Waiting for Opponent..." screen to appear
4. Have the Connecting player launch the game and connect to the Hosting player

The process of enabling PvP is extraordinarily user-unfriendly and is the source of major improvement in future iterations.

## Demo  
* **Single Player**

  https://user-images.githubusercontent.com/52103944/163855760-28e6deb9-a340-4241-a829-57b8d89528bd.mp4

* **Multiplayer**

  Player A | Player B
  :-: | :-:
  <video src="https://user-images.githubusercontent.com/52103944/164942738-97e3b31a-d797-40ec-9cfe-f1db1ee54727.mp4" width=200/> | <video src="https://user-images.githubusercontent.com/52103944/164942816-cfd0703d-d780-48d7-b6e7-518ef26dd0aa.mp4" width=180/>

## Development

### Build
This project was developed using Visual Studio. The build dependent libraries are included in the /lib folder found in the root of this repository. The project can be built by opening the [Battleship Solution File](https://github.com/YourRoyalLinus/Battleship/blob/project_organization/Battleship.sln) and running `Build Solution` from Visual Studio.

### Build Dependencies
* glm
* glad
* stb-master
* cereal-1.3.0
* SDL2-2.0.14
* SDL2_image-2.0.5

### Project Organization
The files within the /battleship directory will look extremely unorganized when viewing from GitHub or your local file system. This is due to Visual Studio using [Visual Studio Filters](https://docs.microsoft.com/en-us/cpp/build/reference/vcxproj-filters-files?view=msvc-170) to organize the solution. Inside of the solution explorer, the source and include files will be organized in an expectable way.
