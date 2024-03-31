# Particle Simulator
A simulation project for Distributed Computing (STDISCM) course. This involves simulating particles in a 2D space with elastic collisions. 
Developer server must be able to add particles to the simulation. The simulation must be able to calculate the collisions between particles and edges.

## (C++ Server) Packages used
- [glad](https://github.com/Dav1dde/glad)
- [sdl2](https://github.com/libsdl-org/SDL)
- [imgui](https://github.com/ocornut/imgui)

## (Java Client) Packages used
- [LWJGL](https://www.lwjgl.org/)
- [imgui-java](https://github.com/SpaiR/imgui-java)

# Message Passing
The Server and Client communicate with each other using a custom message passing protocol.

## Messages
1. `<c>uid,x,y</c>` - New client connection with uid and position x, y
2. `<m>uid,x,y,dirX,dirY</m>` - Move client with uid to x, y with direction dirX, dirY
3. `<p>x,y,angle,velocity</p>` - Add particle to the simulation with position x, y, angle and velocity
4. `<b>1,num,startX,endX,startY,endY,angle,velocity</b>` - Particle batch method 1
5. `<b>2,num,startAngle,endAngle,startX,startY,velocity</b>` - Particle batch method 2
6. `<b>3,num,startX,startY,angle,startVelocity,endVelocity</b>` - Particle batch method 3
7. `<r>either <p> or <b> message</r>` - (Used in client loader) load particle history record

- A client connects to the server with #1, and the server returns #7 that contains #3 or #4 or #5 or #6, depending on if particles were added before client joined the simulation. Otherwise, just sends "DONE".
- A client moves with #2. It updates its position in the server and direction if it is moving. It only sends if the client's direction changes.
- Whenever server adds a particles, it sends #3, #4, #5, or #6 to all clients.

# Calculations
The clients calculate the particle movements and player movements. The server simply acts as a relay for the clients.

When a client joins the server late, the server sends the client the particle history record. The client then simulates the particles from the last known state. This is because the history records contains record logs with time taken since the previous record. The client can simulate this by running the simulation with delta 1 the same number of seconds as the simulation's runtime.

Whenever a client moves, the client sends the server the new position and direction. The server then sends this information to all clients. The clients then update that player's position and direction. Should be noted that it is not always sending as it is moving. It only sends when it changes direction (e.g.: player starts moving up, or player stops moving) meaning a player can hold a direction and only send information if it stops holding. Same as if player presses once. To accommodate latency or movement calculation mismatch, the current position of the player is also sent. Therefore, if a player stops moving or changes direction, it will move the player to that spot.

# How to run project
1. Clone the repository `git clone --recurse-submodules https://github.com/erik-lance/Particle-Simulator-v3.git`

## How to run C++ Server in Visual Studio
1. Go to `CMakeLists.txt` and hit `CTRL+S` to automatically start downloading required packages.
2. Run the project with `CMakeLists.txt` as the startup file.

## How to run Java Client in IntelliJ IDEA
1. Open the project in IntelliJ IDEA.
2. Run the project with `Main.java` as the main class.

## How to run Java Client in Visual Studio Code
1. Open the folder `Client-Explorer` in Visual Studio Code.
2. Run the project with `Main.java` as the main class.
Note: You may need to install the Java Extension Pack in Visual Studio Code.
