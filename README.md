# ECM (Embedded-Cockpit Manager)

This SW solution is meant to run on the raspberry pi situated in the cockpit of the Sojuz Simulator.
This controls all outputs (currenlty LEDs) and in future the inputs (buttons, joysticks) and eventually the two main displays.


*********************************************************
To MANUALLY Build and run (without using visual studio).
*********************************************************
1. open a terminal and move to the ecm main folder containing this README.md. using:
	cd ../
2. Create a build folder and configue the project using:
	cmake -S . -B build
3. Build the project inside the build folder using
	cmake --build build
4. call:
	./build/apps/embedded_cockpit_manager