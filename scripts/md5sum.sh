#!/bin/sh
CHECKSUM=$1
RMFILES="*"$1
RELEASE="helios-v1.0r1"
#
cd ../bin
rm -f ${RMFILES}
md5sum * > "bin"${CHECKSUM}
cd ../default
rm -f ${RMFILES}
md5sum * > "default"${CHECKSUM}
cd ../doc
rm -f ${RMFILES}
md5sum * > "doc"${CHECKSUM}
cd ../math
rm -f ${RMFILES}
md5sum * > "math"${CHECKSUM}
cd ../objects
rm -f ${RMFILES}
md5sum * > "objects"${CHECKSUM}
cd ../samples
rm -f ${RMFILES}
md5sum * > "samples"${CHECKSUM}
cd ../scripts
rm -f ${RMFILES}
md5sum * > "scripts"${CHECKSUM}
cd ../src
rm -f ${RMFILES}
md5sum * > "src"${CHECKSUM}
cd ../tools
rm -f ${RMFILES}
md5sum * > "tools"${CHECKSUM}
cd ../work
rm -f ${RMFILES}
md5sum * > "work"${CHECKSUM}
cd ../
rm -f ${RMFILES}
md5sum * > ${RELEASE}${CHECKSUM}
