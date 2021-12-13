FROM verilator/verilator:latest

RUN cat /etc/*-release

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/New_York

RUN apt-get update && \
    apt-get install -y wget git file sed cmake gtkwave autoconf automake

RUN aclocal --version
ENV SYSTEMC_HOME=/usr/local/systemc/2.3.2
RUN wget https://accellera.org/images/downloads/standards/systemc/systemc-2.3.2.tar.gz && \
    tar xvf systemc-2.3.2.tar.gz && \
    cd systemc-2.3.2 && \
    mkdir build && \
    cd build && \
    mkdir -p /usr/local/systemc/2.3.2/ && \
    export CC='gcc -std=gnu11' && \
    export CXX='g++ -std=gnu++11' && \
    #sed s/^nb_put/this->nb_put/ --inline ../src/tlm_core/tlm_1/tlm_analysis/tlm_analysis_fifo.h && \
    ../configure --enable-pthreads --enable-shared --prefix=$SYSTEMC_HOME && \
    make -j`nproc` && \
    make install && \
    export SYSTEMC_HOME=$SYSTEMC_HOME

RUN ls $SYSTEMC_HOME
RUN apt-get install libtool -y

RUN wget https://www.accellera.org/images/downloads/standards/systemc/uvm-systemc-1.0-beta1.tar.gz && \
    tar xvf uvm-systemc-1.0-beta1.tar.gz && \
    cd uvm-systemc-1.0-beta1 && \
    export CC='gcc -std=gnu11' && \
    export CXX='g++ -std=gnu++11' && \
    ./config/bootstrap && \
    mkdir build && cd build && \
    ../configure --enable-shared --with-systemc=$SYSTEMC_HOME --prefix=$SYSTEMC_HOME && \
    make -j`nproc` && \
    make install

RUN wget https://www.accellera.org/images/downloads/standards/systemc/scv-2.0.1.tar.gz && \
    tar xvf scv-2.0.1.tar.gz && \
    cd scv-2.0.1 && \
    export CC='gcc -std=gnu11' && \
    export CXX='g++ -std=gnu++11' && \
    mkdir build && cd build && \
    ../configure --enable-shared --with-systemc=$SYSTEMC_HOME --prefix=$SYSTEMC_HOME && \
    make -j`nproc` && \
    make install

RUN apt-get install -y googletest libgmock-dev libgtest-dev googletest-tools

RUN git clone https://github.com/tymonx/logic.git

RUN cd logic && \
    mkdir build && cd build && \
    cmake ../ && \
    cmake --build . --target all

