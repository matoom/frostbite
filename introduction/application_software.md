# Application Software

All necessary libraries required to run the application are included in the software package. This includes runtime libraries 
needed by the QT framework and other compiler specific libraries. The default installation has all the necessary
pre-configured settings ready for the first time run but can be changed according to personal preference. 

The software package does not include Ruby runtime.

Ruby runtime installation is required to run scripts in the frontend however the client is still
available to use without Ruby or the native scripting support.
The installation of ruby runtime is described in more detail in the [installation](/installation) page.

The application software can be downloaded from the [installation](/installation) page.

The mechanics in scripting module implementation restrict from running more than one application per copy.

## Application Structure and Files

Runtime resources for the application executable come in a pre-defined structure to facilitate its normal
functions. It is important that the pre-packaged folder structure remains unchanged but configuration
and data files can be added or changed.

To view application files On Max OS X right click on the "Frostbite.app" and open "Show Package Contents".
From there: Contents > MacOS.

* /..
* ![Small image of a folder](../assets/img/folder.png) logs
> The client log folder.
* ![Small image of a folder](../assets/img/folder.png) maps
> Map files
* ![Small image of a folder](../assets/img/folder.png) profiles
> Contains client settings profiles
* ![Small image of a folder](../assets/img/folder.png) scripts
> Designated folder for application scripts
* ![Small image of a folder](../assets/img/folder.png) snapshots
> Snapshots taken in client window (saved as HTML)
* ![Small image of a folder](../assets/img/folder.png) sounds
> The designated folder for sound files used by highlights and alerts.
* ![Small image of executable](../assets/img/exe.png) Frostbite(.exe) - application executable
* ![Small image of a file](../assets/img/ini.png) api.ini - contains the api server port
* ![Small image of a file](../assets/img/ini.png) client.ini - settings file for general client settings
* ![Small image of a file](../assets/img/ini.png) highlights.ini - settings file for color highlights
* ![Small image of a file](../assets/img/ini.png) log.ini - client logger settings - log4qt
* ![Small image of a file](../assets/img/ini.png) macros.ini - settings file for macros
* ![Small image of a file](../assets/img/ini.png) general.ini - general purpose settings file
