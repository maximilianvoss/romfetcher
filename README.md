[![Build Status](https://travis-ci.org/maximilianvoss/romfetcher.svg?branch=master)](https://travis-ci.org/maximilianvoss/romfetcher)      
  
  
# ROM Fetcher
ROM Fetcher is a small utility which helps you to fetch ROMs from existing websites directly on your Retropie, Retrostone
or whatever is based on Retropie/RetrOrangePie. The UI is built for Emulationstation and is easy to navigate
with a gamepad.  
ROM Fetcher is implemented in proper C using SDL2, it may run also on other platforms.

Following ROM databases are supported:  
* https://romsmania.cc/
* https://wowroms.com/
* https://roms-download.com/
* https://romsemulator.net/
* https://romhustler.org/

Please note: This software is using the mentioned sites to download ROMs. You are responsible to have valid licenses
to use the software. Rom Fetcher does not hold, provide, sell or distribute licenses.

After installation you will find a menu entry in Retropie > Rom Fetcher.

**Hotkeys**:  
`A`: Select  
`B`: Back  
`X`: Enter for the keyboard  
`Y`: Backspace for the keyboard  
`Start` + `Select`: Quit Rom Fetcher from any position  

## Installing the latest version on Retrostone 2
Download the latest version: from https://github.com/maximilianvoss/romfetcher/releases/latest  
Copy it onto your Retrostone 2 and run following command: 
```bash
dpkg -i romfetcher-*-armhf.deb
```

## Installing the latest version from source
### Fetching Dependencies
```bash
sudo apt-get update
sudo apt-get -y install libsdl2-ttf-dev libsdl2-image-dev libsdl2-dev libcurl4-openssl-dev libsqlite3-dev libcurl4-openssl-dev
```
 
### Compiling the latest code
```bash
git clone https://github.com/maximilianvoss/romfetcher.git
cd romfetcher
cmake -G "Unix Makefiles"
make
sudo make install
```

## Post Processing of downloads
It seems some of the pages offer the downloads as 7z or RAR files. To automatically extract them, please install the fitting toolset:
```bash 
sudo apt-get -y install p7zip unrar-free
```
Decompression of ZIP files is disabled by default as Emulationstation is capable of handling them.  
If you prefer to enable ZIP decompression please do following:
```bash
sqlite3 ~/.romfetcher/romfetcher.db
```
Execute following SQL command in sqlite3:
```sql
UPDATE postprocessors SET active=1 WHERE filesuffix=".zip";
```
**REMARK:** The downloaded original files won't be deleted after decompression.

### Troubleshooting
#### Missing dependencies: mesa-common-dev & libegl1-mesa-dev
I experienced on the latest RetrOrangePie 4.3 that these dependencies can't be installed 
due to a clash in versions. The easiest workaround is to force the packages to be installed
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
