# SubdivisionSurfaces-Loop
Loop scheme for subdivision surfaces
#### Introduction
This is my course project for Geometric Modeling. This project implements subdivision surfaces by Loop scheme. It is based on `"Taubin G. A signal processing approach to fair surface design[C]//Proceedings of the 22nd annual conference on Computer graphics and interactive techniques. ACM, 1995: 351-358."`

The actual masks of vertex point and edge point I used in the project:

![mask for edge point](https://github.com/tlsshh/SubdivisionSurfaces-Loop/raw/master/Screenshots/mask_edgepoint.png)
![mask for vertex point](https://github.com/tlsshh/SubdivisionSurfaces-Loop/raw/master/Screenshots/mask_vertexpoint.png)

 This project can only work on triangle meshes because I haven't added any function for triangulation. Besides, the model must be a closed model.

I'm sorry for I haven't added any comments into the code, because I just want to make a copy of my work on the internet just in case.

#### Environment
1. IDE: visual studio 2015
2. Libraries: 
	- OpenGL
		- freeglut
		- glew-2.1.0
	- OpenCV 3.2.0 

#### Instruction
1. Input file
There are some examples in `obj` directory. The files there are standard obj files (I just changed the postfix).
Only support a specific range of obj files: 
	- Triangle meshes
	- Only "v" and "f", any other commands will be ignored
	- A closed model
	- You can set vertex color in obj file, but the color will be ignored.
2. Scene roaming
	- Object rotation: 
	Move your mouse, the model will rotate around an axis. If press "shift" at the same time, the model will rotate around another axis
	- Camera translation: 
	Up or Down or Left or Right key.
3. Display mode
Press "s" to switch the mode: Mesh or Wireframe or Mesh_and_Wireframe.
4. Subdivide
Press "a" to increase the subdivision level. Press "d" to decrease the subdivision level.

#### Results
![subdivision result for teddy.obj](https://github.com/tlsshh/SubdivisionSurfaces-Loop/raw/master/Screenshots/teddy_result.jpg)
![subdivision result for cow.obj](https://github.com/tlsshh/SubdivisionSurfaces-Loop/raw/master/Screenshots/cow_result.jpg)

      
