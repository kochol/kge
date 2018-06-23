KGE is designed to work on several platfroms

# Requirements
SDL2 http://libsdl.org/download-2.0.php (this may be helpful: http://askubuntu.com/a/344528/66689)
Cg toolkit from here: https://developer.nvidia.com/cg-toolkit
DevIL from here: http://openil.sourceforge.net
GLEW from here: http://glew.sourceforge.net/

## Under Linux operating system:
These packages were also required:
	sdl2-dev build-essential libncurses5-dev libncursesw5-dev


# Installation
## Linux (Tested on Ubuntu 13.10 64-bit)
these are the steps that I took to install KGE on a 64 bit Ubuntu 13.10 installation

	mkdir ~/git
	cd ~/git
	git clone https://github.com/kochol/kge.git

If you do not want to use git you can obtain a zip copy of KGE from: https://github.com/kochol/kge
Download the zip file and extract them in ~/git/ folder.

	cd kge
	sudo apt-get install build-essential libncurses5-dev libncursesw5-dev libsdl2-dev libdevil-dev libglew-dev

installed Cg toolkit from here: https://developer.nvidia.com/cg-toolkit
(look for the .deb file for Ubuntu)

	./waf configure build -v

