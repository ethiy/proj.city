#! /usr/bin/env bash

mkdir 3rdParty && cd 3rdParty
curl -O https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/lib3ds/lib3ds-1.3.0.zip
unzip lib3ds-1.3.0.zip
rm lib3ds-1.3.0.zip
cd lib3ds-1.3.0/
./configure
make -j4 all && make install

cd ..
curl -O https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.9/CGAL-4.9.zip
unzip CGAL-4.9.zip
rm CGAL-4.9.zip
cd CGAL-4.9/
mkdir build && cd build
cmake ../..
make -j4 all
make install && make install_FindCGAL
