# Installing from source

There are six steps to building your own executable:

1. Install the Qt5 depencies and build.
2. [Download](https://github.com/matoom/frostbite/releases) the .zip or .tar.gz archives
3. Extract the source.
4. Move the extracted project to any preferred location within your home directory.
5. Prepare the build for your computer.
6. Build the project.

## Detailed Instructions for Fedora 40

> Be sure to substitute the desired version for [version] in commands below.
> The current version is `v1.17.0-beta`.
>
> Also, if you are copy-pasting, do not include the `$` prompt.

```shell
$ sudo dnf install -y wget tar qt5-*
$ cd $HOME/Downloads # You may alternatively change into any directory within $HOME
$ wget https://github.com/matoom/frostbite/archive/refs/tags/[version].tar.gz
$ tar -xzf [version].tar.gz
$ cd frostbite-[version]
$ qmake-qt5
$ make
$ ./Frostbite # This command should launch the newly built binary
```

## Detailed Instructions for Ubuntu 23.10

> Be sure to substitute the desired version for [version] in commands below.
> The current version is `v1.17.0-beta`.
>
> Also, if you are copy-pasting, do not include the `$` prompt.

```shell
$ sudo apt install -y wget tar g++ make qt*5-dev
$ cd $HOME/Downloads # You may alternatively change into any directory within $HOME
$ wget https://github.com/matoom/frostbite/archive/refs/tags/[version].tar.gz
$ tar -xzf [version].tar.gz
$ cd frostbite-[version]
$ qmake
$ make
$ ./Frostbite # This command should launch the newly built binary
```

## Optional - Adding an Application Icon in Gnome Desktop Environment

For those using the Gnome desktop environment you, may add a branded
icon triggering the binary by adding a file named
`frostbite.desktop` to `$HOME/.local/share/applications`:

> Be sure to substitute the desired the file system location where you extracted the source
> code archive for [Full path to the project directory] in commands below.

```
Type=Application
Version=1.0
Name=Frostbite
Comment=Dragonrealms MUD Frontend
Icon=[full path to project directory]/gui/images/shield.png
Exec=[full path to project directory]/Frostbite
Terminal=false
```
