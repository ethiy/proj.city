# Proj.city


| Technology               | Status        |
| ------------------------ |:-------------:|
| Travis CI (Linux - MacOS)| [![Build Status](https://travis-ci.org/Ethiy/3DSceneModel.svg?branch=master)](https://travis-ci.org/Ethiy/3DSceneModel)|
| Appveyor (MinGW-w64)     | [![Build Status](https://ci.appveyor.com/api/projects/status/855pa36o55g3hwq7/branch/master?svg=true)](https://ci.appveyor.com/project/Ethiy/3DSceneModel/branch/master)|
| Coverity Scan            |[![Coverity Scan Build Status](https://scan.coverity.com/projects/11095/badge.svg)](https://scan.coverity.com/projects/3dscenemodel)|
| Code Quality             |[![BCH compliance](https://bettercodehub.com/edge/badge/ethiy/proj.city?branch=dev)](https://bettercodehub.com/)
|

## License

[![GitHub license](https://img.shields.io/badge/license-GPL3.0-blue.svg)](https://raw.githubusercontent.com/Ethiy/3DSceneModel/master/LICENSE)

This work is the result of a PhD thesis financed by `IGN France`. It is available as an open source project under the `GPL v3.0` License. If you want to have this program under a different License you should contact `IGN France`.

## Build

In order to build this project you need first to check for these dependencies:

* Boost Libraries: (> `v1.58.0`)
* CGAL Library: (> `v4.7-4`)
* GDAL Library: (> `v2.0`)
* lib3ds Library: (`v1.3.0`)
* Catch Library: (> `v1.6.0`)
* docopt.cpp Library: (> `v0.6.2`)

 This project is tested on these three main platforms:

* Ubuntu: (`v16.04`)
* Darwin: (> `xcode v8`)
* Windows: (MinGW w64)

 We explain here how to get these listed dependencies and how to build for each platform:

### Cross Platform dependencies

* Catch:
    Catch is a header only library available on [`Github`](https://github.com/philsquared/Catch). You do not need to install it as a CMake module installs it as an external project.

* Docopt.cpp:
    `docopt.cpp` is a fun argument parser library available on [`Github`](https://github.com/docopt/docopt.cpp). You do not need to install it also as a CMake module installs it as an external project.


### Ubuntu `v16.04`

You can check the project's [docker file](https://github.com/Ethiy/3DSceneModel/blob/master/Dockerfile) for an inspiration. We will use `aptitude` the available package manager for most dependencies:

* Boost:
    This project uses only  `filesystem` and `system` boost libraries. However, since CGAL uses other libraries it may be wise to get all of them:

    ```bash
    apt install libboost-all-dev
    ```

* CGAL:
    CGAL `v4.7-4` is available in the official repositories:

    ```bash
    apt install libcgal-dev
    ```

* GDAL:
    GDAL > `v2.0` is not available in the official repositories.

    The easiest way to get a newer version would be through the unstable `ubuntugis` ppa:

    ```bash
    apt update && apt upgade
    apt install software-properties-common # If not installed, it contains `add-apt-repository` command
    add-apt-repository ppa:ubuntugis/ubuntugis-unstable # adds ubuntugis/ubuntugis-unstable ppa
    apt update && apt upgade
    apt install libgdal-dev
    ```

    You should know that this will also upgrade `Qgis`. You can force `aptitude` to freeze `Qgis` for instance, or you can download the source form the official release [website](https://trac.osgeo.org/gdal/wiki/DownloadSource) and build `GDAL`.

* Lib3ds:
    lib3ds `v1.3.0` is available in the official repositories:

    ```bash
    apt install lib3ds-dev
    ```

### Darwin

You can check the project's [travis file](https://github.com/Ethiy/3DSceneModel/blob/master/.travis.yml) for an inspiration. There are two available package managers. `brew` generally has the latest updates while `port` focuses more on stability. Personnally, I prefer the later.

| Dependency   | `brew`        | `port`        |
|:------------:|:-------------:|:-------------:|
| Installation | `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"` | You should follow the instructions on [`port` official website](https://www.macports.org/install.php) |
| Update       | `brew update` | `port selfupdate && port upgrade outdated` |
| Boost        | `brew install boost` | `port install boost` |
| CGAL         | `brew install cgal` | `port install cgal` |
| GDAL         | You can find the lastest release in this [website](http://www.kyngchaos.com/software/frameworks) | `port install gdal` |
| lib3ds       | `brew install lib3ds` | `port install lib3ds` |

### MinGW w64
