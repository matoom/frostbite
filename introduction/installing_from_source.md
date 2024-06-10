# Installing from source

There are six steps to building your own executable:

1. [Download](https://github.com/matoom/frostbite/releases) the .zip or .tar.gz archives
3. Extract the source.
4. Move the extracted project to any preferred location within your home directory.
2. Install the Qt5 depencies.
5. Prepare the build for your computer.
6. Build the project.

## Detailed Instructions for Fedora 40

```shell
$ cd $HOME/Downloads # You may alternatively change into any directory within $HOME
$ sudo dnf install wget tar qt5-*-devel -y
$ wget https://github.com/matoom/frostbite/archive/refs/tags/[version].tar.gz
$ tar -xzf [version].tar.gz
$ cd frostbite-[version]
$ qmake-qt5
$ make
```
For those using the Gnome desktop environment you may add a branded
icon triggering the binary by adding a file named
`frostbite.desktop` to `$HOME/.local/share/applications`:

```
Type=Application
Version=1.0
Name=Frostbite
Comment=Dragonrealms MUD Frontend
Icon=[full path to project directory]/gui/images/shield.png
Exec=[full path to project directory]/Frostbite
Terminal=false
```
