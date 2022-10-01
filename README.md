# ECM (Embedded-Cockpit Manager)

This SW solution is meant to run on the raspberry pi situated in the cockpit of the Sojuz Simulator.
This controls all outputs (currenlty LEDs) and in future the inputs (buttons, joysticks) and eventually the two main displays.


***************************
To Build
***************************
1. create a build folder inside the directory where the CMakeLists.txt is saved.
	mkdir build
2. move insie the build directory
	cd build
3. run sequentially
	cmake ..
	cmake --build .

	