WSL INSTALLATION _______________

Install Windows Subsystem for Linux (WSL) Installation 

1. Open PowerShell as an Administrator.
2. Run the following command to enable the WSL feature: wsl --install
3. After the installation is complete, restart your computer if prompted.
4. Once restarted, open the Microsoft Store and search for a Linux distribution (e.g., Ubuntu).
5. Install the desired Linux distribution.
6. After installation, launch the Linux distribution from the Start menu and complete the setup process (create a user account and password).

DEPENDENCIES __________________

Run the following commands in linux terminal ->
	
 	sudo apt-get update
	sudo apt-get install build-essential
	sudo apt-get install libglew-dev
	sudo apt-get install libglm-dev
	sudo apt install libglfw3-dev
	sudo apt install libfreetype6-dev


Verify that all header files and libs for these installations are in /usr/include to avoid include directive errors

Move FreeType installation to directory include statements of project files expect if not automatic with installation 

	cd /usr/include
  	cd freetype2
  	sudo mv * /usr/include
  	cd ..
  	sudo rm freetype2

NOTES __________________________

If installation did not result in header files being automatically added to /usr/include, the include statements will throw errors. 

COMPILATION ____________________

Naviagate to directory with main.cpp using cd command (ysw/src/main.cpp)
In terminal enter the following commands
	
 	g++ main.cpp TextRenderer.cpp -o run -lGL -lGLEW -lglfw -lfreetype
	./run
