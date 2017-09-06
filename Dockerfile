FROM ubuntu:16.04

RUN apt-get -y update
RUN apt-get -y upgrade

RUN apt-get install -y software-properties-common

RUN add-apt-repository -y ppa:ubuntugis/ppa
RUN apt-get -y update
RUN apt-get -y upgrade
            
RUN apt-get install -y \
            gcc-5-base\
            libgcc-5-dev\
            g++

RUN apt-get install -y \
            clang-4.0

RUN clang++ --version

RUN apt-get install -y \
            cmake\
            git\
            wget

RUN apt-get install -y \
            libboost-all-dev
            
RUN apt-get install -y \
            libkml-dev\
            libgdal-dev

RUN apt-get install -y \
            libgmp-dev\
            libmpfr-dev\
            libmpfrc++-dev

RUN apt-get install -y \
            lib3ds-dev\
            libtinyxml2-dev

WORKDIR /home
RUN wget https://github.com/CGAL/cgal/archive/releases/CGAL-4.10.tar.gz
RUN mkdir CGAL-4.10 && tar xf CGAL-4.10.tar.gz -C CGAL-4.10 --strip-components 1
WORKDIR CGAL-4.10
RUN mkdir build
WORKDIR build
RUN cmake .. -DCMAKE_INSTALL_PREFIX=/usr
RUN make -j `nproc` install && make install_FindCGAL
WORKDIR /home
RUN git clone https://github.com/Ethiy/proj.city.git
WORKDIR proj.city/
RUN git checkout build-system-trial
RUN mkdir build && mkdir build/linux
WORKDIR build/xenial
