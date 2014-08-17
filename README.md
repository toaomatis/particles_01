Particles
============

Gravity Simulator based on C POSIX library and visualized by OpenGL

## History
I'm an adjective fan of planets, stars, galaxies and the cosmos in total. So I try to watch as much as science documentary serie there are.
During one of the episodes of *Through the Wormhole* a computer program was shown which simulated the agglomeration of (dark))matter. At that point I dicided I had to create such a program myself. 
After a couple of hours of coding, my Java application was done, but the performance was terrible. I dicided to ditch the high-level programming language and rewrote the application in C.
While the calculations require lots of CPU power, I added thread pools to spread and balance the calcuations over more cores and added mutexes and conditionals to synchronize the simulation and signal the window when a redraw was required.
```
“And some things that should not have been forgotten were lost. History became legend. Legend became myth. And for one and a half year, the simulation passed out of all knowledge.”
― J.R.R. Tolkien, The Lord of the Rings
```
Recently I watched the *Cosmos: A Spacetime Odyssey* and exactly the same simulation was shown. I remembered the software I once wrote and dicided to share it with the world. But beware, there are lots of work-in-progresses!

## Whislist
* 3D simulation
* Zoom, pan, pitch and rotate the simulation, this requires a 3D scene. 
* Saving and loading of the current state
* On the fly parameter adjustmens
* Scale window
* Port to OpenCL
* Whatever you come up with :-)

## Prerequisites 
Development is currently done in Ubuntu 12.04 and the following packages are required:
> sudo apt-get install build-essential freeglut3-dev libgl1-mesa-dev libglew1.6-dev libglu1-mesa-dev libpth20

## Compiling 
Browse to the folder you just checked out and run the following command
> make

Everything should go fine and after the make command completes, an executable named particles_01 should be available. 

## Starting the application
Simply type 
> ./particles_01

in your shell and some logging should appear followd by the main screen with lots of particles in in. 

## Running, pausing and terminating the simulator
The following keyboard keys are implemented:
* Esc - Terminates the simulation and exits the application
* Space - Starts or pauses the simulation
* d - Enables or disables debug logging to the console
* t - Shows or hides trace paths from the particles

## Changing the simulator parameters
There are a lot of parameters that can be changed to alter the simulation, e.g.:
* The bounds of the screen: 
** Wrap - When a particle leaves one edge of the screen, it wraps arround the shortest distance
** Bounce - When a particle hits an edghe of the screen, it is bounced back in the opposite direction
** None - The screen is only the viewport of the simulation
* The action between particles on collission:
** None - Particles won't collide 
** Collide - Both particles will change course based on their momentum
** Mush - The paricles will be combined and the new particle will change course based on the combined momentum
* The number of particles 
* World Gravity (how strong the ground will attract each particle)
* Mutual gravity between particles 
* The mass of the particles 
* The length of the trace of a particle 
* The restitution (friction) when a particle interact with another particle or the bounds of the screen
Unfortunalty, these parameters are all hardcoded in particle.c, so after you alter them, you need to recompile the source(s) and restart the application to take effect. 
