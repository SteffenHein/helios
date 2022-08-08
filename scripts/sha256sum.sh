#!/bin/sh
CHECKSUM=$1
RMFILES="*"$1
RELEASE="helios-v1.0r1"
#
cd ../bin
rm -f ${RMFILES}
openssl sha256 * > "bin"${CHECKSUM}
cd ../default
rm -f ${RMFILES}
openssl sha256 * > "default"${CHECKSUM}
cd ../doc
rm -f ${RMFILES}
openssl sha256 * > "doc"${CHECKSUM}
cd ../math
rm -f ${RMFILES}
openssl sha256 * > "math"${CHECKSUM}
cd ../objects
rm -f ${RMFILES}
openssl sha256 * > "objects"${CHECKSUM}
cd ../samples
rm -f ${RMFILES}
openssl sha256 * > "samples"${CHECKSUM}
cd ../scripts
rm -f ${RMFILES}
openssl sha256 * > "scripts"${CHECKSUM}
cd ../src
rm -f ${RMFILES}
openssl sha256 * > "src"${CHECKSUM}
cd ../tools
rm -f ${RMFILES}
openssl sha256 * > "tools"${CHECKSUM}
cd ../work
rm -f ${RMFILES}
openssl sha256 * > "work"${CHECKSUM}
cd ../
rm -f ${RMFILES}
openssl sha256 * > ${RELEASE}${CHECKSUM}
