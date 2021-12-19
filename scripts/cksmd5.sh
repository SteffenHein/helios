#!/bin/sh
CHECKSUM=$1
RMFILES=${CHECKSUM}" *.md5"
#
cd ../ports/helios
rm -f distinfo
md5sum * > distinfo
cd ../ # directory ../ports
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../CVS
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../bin
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../doc
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../expm
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
cd ../scripts
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../src
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../tools
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
cd ../
rm -f ${RMFILES}
md5sum * > ${CHECKSUM}
