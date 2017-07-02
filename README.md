# Proj.city

| Technology               | Status        |
| ------------------------ |:-------------:|
| Travis CI (Linux - MacOS)| [![Build Status](https://travis-ci.org/Ethiy/3DSceneModel.svg?branch=master)](https://travis-ci.org/Ethiy/3DSceneModel)|
| Appveyor (MinGW-w64)     | [![Build Status](https://ci.appveyor.com/api/projects/status/855pa36o55g3hwq7/branch/master?svg=true)](https://ci.appveyor.com/project/Ethiy/3DSceneModel/branch/master)|
| Coverity Scan            |[![Coverity Scan Build Status](https://scan.coverity.com/projects/11095/badge.svg)](https://scan.coverity.com/projects/3dscenemodel)|
| Code Quality             |[![BCH compliance](https://bettercodehub.com/edge/badge/ethiy/proj.city?branch=dev)](https://bettercodehub.com/)

## License

[![GitHub license](https://img.shields.io/badge/license-GPL3.0-blue.svg)](/LICENSE)

This work is the result of a PhD thesis financed by `IGN France`. It is available as an open source project under the `GPL v3.0` License. If you want to have this program under a different License you should contact `IGN France`.

## Build

In order to build this project you need first to check for these dependencies:

| Library               | Version       |
| --------------------- |:-------------:|
| `boost`               | > `v1.58.0`   |
| `Cgal`                | > `v4.7-4`    |
| `Gdal`                | > `v2.0.0`    |
| `lib3ds`              | > `v1.3.0`    |
| `tinyXML2`            | > `v2.6.0`    |
| `Catch`               | > `v1.6.0`    |
| `docopt.cpp`          | > `v0.6.2`    |

 This project is tested on these three main platforms:

| Platform              | Image         |
| --------------------- |:------------- |
| `Ubuntu LTS`          | > `v16.04`    |
| `Xcode`               | > `v8.0.0`    |
| `Windows`             | = `mingw-w64` |

 We explain here how to get these listed dependencies and how to build for each platform:

### Cross Platform dependencies

* Catch:
    Catch is a header only library available on [`Github`](https://github.com/philsquared/Catch). You do not need to install it as a CMake module installs it as an external project.

* docopt.cpp:
    `docopt.cpp` is a fun argument parser library available on [`Github`](https://github.com/docopt/docopt.cpp). You do not need to install it also as a CMake module installs it as an external project.

### Ubuntu `v16.04`

You can check the project's [docker file](Dockerfile) for an inspiration. We will use `APT` the O.S. package manager for most dependencies:

| Step         | `APT`           |
|:------------:|:------------- |
| Update       | `apt update && apt upgrade -y` |
| boost        | This project uses only  `filesystem` and `system` boost libraries. However, since `Cgal` uses other libraries it may be wise to get all of them:<br>`apt install libboost-all-dev` |
| Cgal         | `apt install libcgal-dev` |
| Gdal         | `Gdal` > `v2.0.0` is not available in the official repositories. The easiest way to get a newer version would be through the unstable `ubuntugis` ppa:<br>`add-apt-repository ppa:ubuntugis/ubuntugis-unstable`<br>`apt update && apt upgrade`*<br>`apt install libgdal-dev`|
| lib3ds       | `apt install lib3ds-dev` |
| tinyXML2       | `apt install libtinyxml-dev` |

>\*\
You should know that this will also upgrade `QGis`. You can force `APT` to freeze `QGis` for instance, or you can download the source form the official release [website](https://trac.osgeo.org/gdal/wiki/DownloadSource) and build `Gdal`.

### MacOS

You can check the project's [travis file](/.travis.yml) for an inspiration. There are two available package managers. `brew` generally has the latest updates while `port` focuses more on stability. Personally, I prefer the later.

| Step         | `brew`        | `port`        |
|:------------:|:------------- |:------------- |
| Installation | `/usr/bin/ruby -e $(curl -fsSL https://raw.githubusercontent.com/`<br>`Homebrew/install/master/install)` | You should follow the instructions on [`port` official website](https://www.macports.org/install.php) |
| Update       | `brew update` | `port selfupdate && port upgrade outdated` |
| boost        | `brew install boost` | `port install boost` |
| Cgal         | `brew install cgal` | `port install cgal` |
| Gdal         | You can find the latest release in this [website](http://www.kyngchaos.com/software/frameworks) | `port install gdal` |
| lib3ds       | `brew install lib3ds` | `port install lib3ds13` |
| TinyXML2     | `brew install tinyxml2` | `port install tinyxml2` |

### MinGW-w64

| Step         | `pacman`      |
|:------------:|:------------- |
| Installation  | Check the [MSYS2 official website](http://www.msys2.org). `MinGW-w64` is provided with `MSYS2` |
| Update       | `pacman --noconfirm -Syu pacman` |
| boost        | `pacman --noconfirm -S mingw-w64-x86_64-boost` |
| Cgal         | `pacman --noconfirm -S mingw-w64-x86_64-cgal` |
| Gdal         | `pacman --noconfirm -S mingw-w64-x86_64-gdal`|
| lib3ds       | You should download and build the library locally. Check out the [Appveyor configuration](appveyor.yml/#L35) for more details |
| TinyXML2     | `pacman --noconfirm -S mingw-w64-x86_64-tinyxml2`|
