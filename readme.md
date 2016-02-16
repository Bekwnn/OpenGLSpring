# Spring Simulation - Cloth Mesh

## Assignment 2 for Computer Animation at UVic - Evan N.

Created using the atlas framework developed by Mauricio A Rovira Galvez

The scene contains a cloth mesh and a camera which runs on a spline when you push "play". It makes use of up to 12 springs per vertex, depending on vertex position.

Cloth system rundown:

### Cloth
Inherits from the atlas Geometry class. Updates a 2D vector array of vertex positions based on the sum of each attached spring's forces. Supports varied position, number of vertices, and "pinning mode". The last determines which vertices remain stationary during simulation.

### MayaCamera
The MayaCamera from the labs, only with now when play is pressed, user control is taken away and it runs on a spline path defined by a vector array of waypoints and transition durations. The waypoint loops from the final position to the starting position indefinitely (closed loop hermite spline).

### PistonSpring
Same as the piston spring from assignment 1.

### ClothScene
Defines cloth object parameters and waypoint locations and durations. Otherwise doesn't do much.

The cloth updates a vertex array and then builds the vertex face buffer in updateGeometry(), sending it to opengl via opengl's glBufferData.

Tested to run smoothly on this computer at 25x25 grid of vertices.
