#[File: helios-v1.0r1/makefile.gnu]
#------------------------------------------------------------------------------#
#                                                                              #
#  Makefile for the helios program package on a [ Debian ] GNU/Linux           #
#  operating system using a GCC/EGC compiler and GNU Make [ or CMake ]         #
#  and [ optionally ] lncurses                                                 #
#                                                                              #
#  (C) SHEIN; Munich, December 2021                            Steffen Hein    #
#  [ Update: December 28, 2021 ]                            <contact@sfenx.de> #
#                                                                              #
#------------------------------------------------------------------------------#
# executable program name: helios                                              #
#                                                                              #
# Version required:	v1.0r1                                                 #
# Date created:		December 17, 2021                                      #
# Whom:			Steffen Hein <contact@sfenx.de>                   #
#                                                                              #
# $Id: Makefile, v1.0r0  2021-12-17 18:50:40 helios Exp $                          #
#                                                                              #
#------------------------------------------------------------------------------#
DISTNAME=	helios-${RELEASE}
RELEASE=        v1.0r1	
#CATEGORIES=	physics
#MASTER_SITES=	ftp://steffen-hein.org/pub/cad/

#MAINTAINER=	Steffen Hein <contact@sfenx.de>

#MAKEFILE=	makefile.gnu
#NO_PACKAGE=	"must provide without modifications"
#------------------------------------------------------------------------------#
# The executable program name [modify, if wanted]:
HELIOS_EXEC=	helios.do
#------------------------------------------------------------------------------#
# The executable program will be installed into ${HELIOS_RESD}bin
HELIOS_RESD=	${HOME}/
#HELIOS_RESD=	/usr/local/
#------------------------------------------------------------------------------#
#
# Compiler flags:
#	-ansi		-- stick to ANSI C
#	-g		-- debugging mode
#	-Wall		-- low level warnings 
#	-O		-- optimization level 1
#	-O3		-- optimization level 3
#       -ffast-math	-- fast floating point operations [ can pose problems
#                          in conjunction with other optimizations ]
#
#------------------------------------------------------------------------------#
#
# The C compiler
#CCOMPILER=	/usr/local/bin/egcc
#CCOMPILER=	/usr/local/bin/gcc
#CCOMPILER=	/usr/bin/gcc
CCOMPILER=	cc
#
# Compiler options [ please modify ]:
HELIOS_CC=${CCOMPILER} -ansi -std=c99 -pedantic -Wall -pipe
#
#
# using flags [ please modify ]:
#HELIOS_CFLG=	-ffast-math # tentative [ don't use together with O, O2, O3 ]
#HELIOS_CFLG=	-D_Debian -D_CCBUG -D_Forced
HELIOS_CFLG=	-O2 -D_Debian -D_CCBUG -D_Forced
#
#
# Find sources in directory: [Define path]
HELIOS_SRC=	./src/
HELIOS_CONF=	./
HELIOS_MATH=	./math/
HELIOS_OBJ=	./objects/
HELIOS_TOOLS=	./tools/
HELIOS_DEFLT=	./default/
#
# Compile into directory: [Define path]
HELIOS_BIN=	./bin/
#
# Install into directory: [Define path]
HELIOS_INST=	${HELIOS_RESD}bin/
#
# libraries: 
# [ lincurses_g: the debugging version of the ncurses library ]
# HELIOS_LIB=	-lm # -lncurses_g
HELIOS_LIB=	-lm -lncurses
# HELIOS_LIB=	-lm 
#------------------------------------------------------------------------------#
#.SILENT:
#------------------------------------------------------------------------------#
include mk.gnu
#------------------------------------------------------------------------------#
