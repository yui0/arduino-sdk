FROM    centos:7 as builder

RUN     set -eux; \
        ulimit -n 1024000 && yum -y install git make libX11 libXrender libXext freetype fontconfig libusb && \
        git clone https://github.com/yui0/arduino-sdk.git && \
        tar xvJf arduino-sdk/teensy/tools/arm.tar.xz -C arduino-sdk/teensy/tools/ && \
        tar xvJf arduino-sdk/teensy/tools/arm-none-eabi.tar.xz -C arduino-sdk/teensy/tools/arm/ && \
        rm anaconda-post.log arduino-sdk/teensy/tools/*.tar.xz

        #cd /arduino-sdk/teensy/tools/ && echo "yes" | sh setup_compiler.sh

