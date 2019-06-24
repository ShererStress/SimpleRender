GA Final Project - SimpleRender

A C++ program that utilizes Win32 API functionality to animate a bouncing and rotating cube. Packaged as an executable file - it will only work on a Windows machine!

Associated repos:
https://github.com/ShererStress/SimpleRenderFrontend
https://github.com/ShererStress/SimpleRenderBackend

The program creates a new window where the drawing takes place. A button is also contained in the window.
After doing an initial drawing of the cube, the window awaits two different types of procedures:
1. A timer, which causes the cube to redraw every millisecond.
2. A button click, which reverses the direction the cube rotates in. 

Whenever the cube is redrawn, the program calculates the new position and orientation of the cube's vertices based on it's previous position and orientation data.



I need to credit the authors of both of these tutorials - the tutorials were both extremely helpful in getting things to run.
https://www.cprogramming.com/tutorial/opengl_first_windows_app.html
http://www.winprog.org/tutorial/simple_window.html

