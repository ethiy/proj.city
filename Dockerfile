FROM ubuntu:16.04
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get install -y \
            software-properties-common\
            gcc-5-base\
            libgcc-5-dev\
            g++\
            cmake\
            git
RUN add-apt-repository -y ppa:ubuntugis/ubuntugis-unstable
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get install -y \
            libboost-filesystem-dev\
            libboost-system-dev\
            libboost-regex-dev\
            lib3ds-dev\
            libtinyxml2-dev\
            libcgal-dev\
            libcgal-qt5-dev\
            libqt5opengl5-dev\
            libgdal-dev
WORKDIR /home
RUN git clone https://github.com/Ethiy/3DSceneModel.git
WORKDIR 3DSceneModel/
RUN git checkout build-system-trial
RUN mkdir build && mkdir build/linux
WORKDIR build/xenial
RUN cmake ../..
RUN cmake ../..
RUN make -j4 all
RUN ./tests/tests
RUN rm *.off *.3ds *.gml *.xsd *.geotiff
