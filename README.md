Abstract Particle Fountain
=====
##### Enclosed is both an OpenGL 4.1/C++ 11 source and a pre-packaged Mac OS X 10.11 binary (to ensure execution). 

The particle system is dynamically coloured based on local particle velocities states in relation to the global domain of the system.

On the code, largely modified from a similar ongoing project of mine, everything is made with maintainability and extensibility in mind and as such I make use of OOP paradigms such as innheritance and polymorphism along with extensive usage of the *Single Responsibility* principle. 

I got a little carried away on the *physical* implementation of the particle system and so by the time I had gotten on to messing around with Frame Buffer Objects there was little time left

#### Features: 
- Basic Forward Euler Integration Scheme
- Time Dependant Paticle Generation 
- Static Points of Attraction Effect
- Velocity Colouring
- Controls Interface

#### Libraries/Dependancies:
- OpenGL Extensions Lib. (**GLEW**)
- Window and Peripherals Lib. (**GLFW3**)
- Image Loading Lib. (**SOIL2**)
- 3D Mathematics Lib. (**GLM**)
- IM GUI Lib. (**ATB**)