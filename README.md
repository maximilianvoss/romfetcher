# ROM Fetcher
ROM Fetcher is a small utility which helps you to fetch ROMs from existing websites. For the moment only 
Romsmania.cc is supported, but additional will follow. The UI is built for Emulationstation and is easy to navigate
with a gamepad.  
ROM Fetcher is implemented in propr C using SDL2, it may run on all platforms like Retropie, Linux and Windows. 

**Hotkeys**:  
`A`: Select  
`B`: Back  
`X`: Enter for the keyboard  
`Y`: Backspace for the keyboard  
`Start` + `Select`: Quit Rom Fetcher from any position  

## Installing the latest version of Retrostone 2
```bash
dpkg -i romfetcher-1.0.0-armhf.deb
```

## Installing the latest version from source
### Fetching Dependencies
```bash
sudo apt-get update
sudo apt-get -y install libsdl2-ttf-dev libsdl2-image-dev libsdl2-dev libcurl4-openssl-dev
```
 
### Compiling the latest code
```bash
git clone https://github.com/maximilianvoss/romfetcher.git
cmake -G "Unix Makefiles"
make
sudo make install
```

### Troubleshooting
#### Missing dependencies: mesa-common-dev & libegl1-mesa-dev
I experienced on the latst RetrOrangePPie 4.3 that these dependencies can't be installed 
due to a clash in versions. The easierst workaround is to force the packages to be installed
regardless of version.

```bash
sudo apt-get update
apt-get download mesa-common-dev
sudo dpkg --force-all -i mesa-common-dev_*_armhf.deb
apt-get download libegl1-mesa-dev
sudo dpkg --force-all -i libegl1-mesa-dev_*_armhf.deb

rm libegl1-mesa-dev_*_armhf.deb 
rm mesa-common-dev_*_armhf.deb 
```
