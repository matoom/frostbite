## Development

The software is written in C/C++, using [Qt](https://www.qt.io) framework version 5.6 and is
developed in [Qt Creator IDE](https://www.qt.io/ide/).
All the Qt tools are free to use for open source projects and can be downloaded
from the [Qt website](http://qt-project.org/downloads).

### Github

The application source code is available at Github -
[https://github.com/matoom/frostbite](https://github.com/matoom/frostbite).
If you want to contribute to the Frostbite project on Github, you have to fork
the repository as described in the following guide
[https://help.github.com/articles/fork-a-repo](https://help.github.com/articles/fork-a-repo).

### Setting up the project

Following steps are necessary to set up the project in Qt Creator IDE:

* Open *Frostbite.pro* in Qt Creator;
* Load the environment settings -> No -> Select Desktop project;
* In *environment.h* -> change project path for offline mock file - `#define MOCK_DATA_PATH "C:/Projects/FrostBite/support/mock.xml";`
* in *main.cpp* -> `set MainWindow::DEBUG = true;`
* Build for debug target;
* Run project in debug mode.

### High level architecture

The entry point to the application is
[main.cpp](https://github.com/matoom/frostbite/blob/master/gui/main.cpp)
which runs the [mainwindow.cpp](https://github.com/matoom/frostbite/blob/master/gui/mainwindow.cpp)
gui.

Everything starts from the MainWindow -> MainWindow::loadClient():

* windowfacade.cpp - everything related to game windows
* toolbar.cpp - handles items on the toolbar
* commandline.cpp - the client command line
* tcpclient.cpp - everything related to game connection and data handling
* scriptservice.cpp - handles scripting processes
* menuhandler.cpp - handles client main menu logic
* xmlparserthread.cpp - parse SF protocol as XML
