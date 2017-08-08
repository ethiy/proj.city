FROM ubuntu:16.04
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get install -y \
            software-properties-common\
            gcc-5-base\
            libgcc-5-dev\
            g++\
            cmake\
            git\
            wget
RUN add-apt-repository -y ppa:ubuntugis/ubuntugis-unstable
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get install -y \
            libboost-filesystem-dev\
            libboost-system-dev\
            libboost-thread-dev\
            libboost-regex-dev\
            lib3ds-dev\
            libtinyxml2-dev\
            libgdal-dev\
            libgmp-dev\
            libmpfr-dev\
            libmpfrc++-dev
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
RUN cmake ../.. -DCGAL_DIR=/usr/lib/CGAL
RUN cmake ../..
RUN make -j `nproc` all
RUN tests/tests
