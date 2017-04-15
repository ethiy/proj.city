# 3DSceneModel

[![GitHub license](https://img.shields.io/badge/license-AGPL-blue.svg)](https://raw.githubusercontent.com/Ethiy/3DSceneModel/master/LICENSE)

| Environment              | Status        |
| ------------------------ |:-------------:|
| Linux                    | [![Build Status](https://travis-ci.org/Ethiy/3DSceneModel.svg?branch=master)](https://travis-ci.org/Ethiy/3DSceneModel)|
| Darwin                   |[![Build Status](https://travis-ci.org/Ethiy/3DSceneModel.svg?branch=master)](https://travis-ci.org/Ethiy/3DSceneModel)|
| Windows                  | [![Build Status](https://ci.appveyor.com/api/projects/status/855pa36o55g3hwq7?svg=true)](https://ci.appveyor.com/project/Ethiy/3DSceneModel/branch/master)|
| Coverity Scan            |[![Coverity Scan Build Status](https://scan.coverity.com/projects/11095/badge.svg)](https://scan.coverity.com/projects/3dscenemodel)|

Playing with 3D formats.


## Build

In order to build this project you need first to check for these dependencies:

 * Boost Libraries: (> v1.58.0)
 * CGAL Library: (> v4.9)
 * GDAL Library: (> v2.1.0)
 * Lib3ds Library: (> v1.3.0)
 * Imagine ++ Library:(> v4.3.1)
 * Catch Library: (> v1.6.0)

 This project builds on the three main platforms:

 * Ubuntu: (v16.04)
 * Darwin: (> xcode v8)
 * Windows: (MinGW w64)

 We explain here how to get these listed dependencies and how to build for each platform:

### Linux

You can check the [docker file] (https://github.com/Ethiy/3DSceneModel/blob/master/Dockerfile) for an example. We will use `aptitude` the available package manager for most dependencies:

This project uses only  `filesystem` and `system` boost libraries. However, since CGAL uses other libraries it may be wise to get all of them:
```bash
apt install libboost-all-dev
```

CGAL v4.9 is available in the official repositories:
```bash
apt install libcgal-dev
```

GDAL > v2.0 is not available in the official repositories. You need to add the unstable ppa before hand:
```bash
apt update && apt upgade
apt install software-properties-common # It contains `add-apt-repository` command
add-apt-repository ppa:ubuntugis/ubuntugis-unstable # adds ubuntugis/ubuntugis-unstable ppa
apt update && apt upgade
apt install libgdal-dev
```

You should know that this will also upgrade `Qgis`, for instance. You can force `aptitude` to freeze `Qgis` for instance, or you can download the source form the official release [website](https://trac.osgeo.org/gdal/wiki/DownloadSource) and build `GDAL`.
