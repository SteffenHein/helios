#!/bin/sh
CHECKSUM=$1
RMFILES=${CHECKSUM}" *.md5"
#
cd ../ # directory ../ports
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../bin
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../default
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../doc
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../math
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../objects
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../prv
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../samples
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../scripts
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../src
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../tools
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../work
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
