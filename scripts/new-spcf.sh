#!/bin/sh
PORTNAME=helios
RELEASE=v1.0r1
DISTNAME=${PORTNAME}-${RELEASE}

# switch into package directory
cd ../../

# Create archive of specially selected sources:
tar -czf spcf-${RELEASE}.tgz \
${DISTNAME}/bin \
${DISTNAME}/doc \
${DISTNAME}/default \
${DISTNAME}/math \
${DISTNAME}/objects \
${DISTNAME}/tools \
${DISTNAME}/scripts \
${DISTNAME}/CONFIG.H \
${DISTNAME}/INSTALL \
${DISTNAME}/LICENSE \
${DISTNAME}/Makefile \
${DISTNAME}/README \
${DISTNAME}/SETUP \
${DISTNAME}/makefile.gnu \
${DISTNAME}/makefile.win \
${DISTNAME}/mk.gnu \
${DISTNAME}/mk.win \
