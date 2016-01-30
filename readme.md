# Spring Simulation - Cloth Mesh

## Assignment 1 for Computer Animation at UVic - Evan N.

Created using the atlas framework developed by Mauricio A Rovira Galvez

The scene contains 2 cubes: one with a piston spring and one with an angular spring. The scene and the spring constraints are implemented in 3D rather than 2D.

I left a debug statement that spits out the cubes positions. I thought it might be good to have.

Spring system rundown:

### Cube
Inherits from the atlas Geometry class. Updates its position based on what its ObjectController tells it.

### ObjectController
A controller to handle object movement. Has an update function which returns a vector to be added to an object's position.
Movement can be added in 3 ways:
addMove(): flat amount of movement to be carried out next frame
addVelocity(): adds a certain amount of constant velocity to the controller
addAccel(): adds a certain amount of constant acceleration to the controller

### PistonSpring
A constraint which holds the spring parameters. GetForce() takes the position of two objects and their velocities. Returns a force vector.

### AngularSpring
A constraint which holds the spring parameters. GetForce takes the position of an object, its velocity, and an anchor. Returns a force vector.

The spring constraints are handled in sceneUpdate(). The ObjectController is handled in updateGeometry().

As a word of caution, AngularSpring uses some janky vector math... accurate! but janky. I explained it in comments.
