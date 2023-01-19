#!/bin/sh
#ver=570a
ver=710a
name=Setup_EmbeddedStudio_ARM_v${ver}_linux_x64.tar.gz
#dir=arm_segger_embedded_studio_${ver}_linux_x64
dir=arm_segger_embedded_studio_v${ver}_linux_x64
BASEDIR=`pwd`/arm-segger

pushd /tmp

curl -O https://dl.a.segger.com/embedded-studio/${name}
tar xvzf ${name}

cd ${dir}
./install_segger_embedded_studio --copy-files-to .

rm -rf ${BASEDIR}/
mkdir -p ${BASEDIR}/{bin,include}
cp -a ./bin/segger-cc* ${BASEDIR}/bin/
cp -a ./include ${BASEDIR}/

popd
