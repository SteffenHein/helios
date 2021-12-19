# [ file: mk.gnu ]
# Update: December 17, 2021
#------------------------------------------------------------------------------
setup: perm install
	@echo "===> Cleaning for ${HELIOS_BIN}"
	rm -f ${HELIOS_BIN}${HELIOS_EXEC}
	@echo "===> ${DONE}"

install: ${HELIOS_INST}${HELIOS_EXEC}

all: setup

${HELIOS_EXEC}: ${HELIOS_BIN}${HELIOS_EXEC}

${HELIOS_INST}${HELIOS_EXEC}: ${HELIOS_BIN}${HELIOS_EXEC}
	mv ${HELIOS_BIN}${HELIOS_EXEC} ${HELIOS_INST}${HELIOS_EXEC}
	@echo "===> S-Linking helios to "${HELIOS_EXEC}
	rm -f ${HELIOS_INST}helios
	rm -f ${HELIOS_INST}codis
	ln -s ${HELIOS_INST}${HELIOS_EXEC} ${HELIOS_INST}helios
	ln -s ${HELIOS_INST}${HELIOS_EXEC} ${HELIOS_INST}codis
	@echo "===> ${DONE}"

${HELIOS_BIN}${HELIOS_EXEC}: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_MATH}consts.h \
${HELIOS_OBJ}lotos.o \
${HELIOS_OBJ}dotos.o \
${HELIOS_OBJ}rndoff.o \
${HELIOS_OBJ}txcnsl.o \
${HELIOS_OBJ}dsplay.o \
${HELIOS_OBJ}cpylne.o \
${HELIOS_OBJ}shutil.o \
${HELIOS_OBJ}rdunit.o \
${HELIOS_OBJ}dotos.o \
${HELIOS_OBJ}rtbis.o \
${HELIOS_OBJ}fdjac.o \
${HELIOS_OBJ}gssjpv.o \
${HELIOS_OBJ}metals.o \
${HELIOS_OBJ}dielct.o \
${HELIOS_OBJ}solids.o \
${HELIOS_OBJ}fluids.o \
${HELIOS_OBJ}losses.o \
${HELIOS_OBJ}kelvin.o \
${HELIOS_OBJ}nusselt.o \
${HELIOS_OBJ}grazet.o \
${HELIOS_OBJ}granus.o \
${HELIOS_OBJ}trnorm.o \
${HELIOS_OBJ}hcndct.o \
${HELIOS_OBJ}graphp.o \
${HELIOS_OBJ}values.o \
${HELIOS_OBJ}intlze.o \
${HELIOS_OBJ}hlswrk.o \
${HELIOS_OBJ}hlsdrv.o \
${HELIOS_OBJ}helios.o
	@echo "===> Building ${HELIOS_EXEC} ..."
	${HELIOS_CC} ${HELIOS_CFLG} \
${HELIOS_OBJ}lotos.o \
${HELIOS_OBJ}dotos.o \
${HELIOS_OBJ}rndoff.o \
${HELIOS_OBJ}dsplay.o \
${HELIOS_OBJ}rdunit.o \
${HELIOS_OBJ}txcnsl.o \
${HELIOS_OBJ}cpylne.o \
${HELIOS_OBJ}shutil.o \
${HELIOS_OBJ}rtbis.o \
${HELIOS_OBJ}fdjac.o \
${HELIOS_OBJ}gssjpv.o \
${HELIOS_OBJ}metals.o \
${HELIOS_OBJ}dielct.o \
${HELIOS_OBJ}solids.o \
${HELIOS_OBJ}fluids.o \
${HELIOS_OBJ}losses.o \
${HELIOS_OBJ}kelvin.o \
${HELIOS_OBJ}nusselt.o \
${HELIOS_OBJ}grazet.o \
${HELIOS_OBJ}granus.o \
${HELIOS_OBJ}trnorm.o \
${HELIOS_OBJ}hcndct.o \
${HELIOS_OBJ}graphp.o \
${HELIOS_OBJ}values.o \
${HELIOS_OBJ}intlze.o \
${HELIOS_OBJ}hlswrk.o \
${HELIOS_OBJ}hlsdrv.o \
${HELIOS_OBJ}helios.o \
-o ${HELIOS_BIN}${HELIOS_EXEC} ${HELIOS_LIB}

${HELIOS_OBJ}lotos.o: ${HELIOS_MATH}lotos.c
	@echo "===> Building lotos(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}lotos.c \
-o ${HELIOS_OBJ}lotos.o

${HELIOS_OBJ}dotos.o: ${HELIOS_MATH}dotos.c
	@echo "===> Building dotos(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}dotos.c \
-o ${HELIOS_OBJ}dotos.o

${HELIOS_OBJ}dsplay.o: \
${HELIOS_MATH}dsplay.c \
${HELIOS_OBJ}lotos.o \
${HELIOS_OBJ}dotos.o 
	@echo "===> Building dsplay(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}dsplay.c \
-o ${HELIOS_OBJ}dsplay.o

${HELIOS_OBJ}rndoff.o: \
${HELIOS_MATH}rndoff.c
	@echo "===> Building rndoff(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}rndoff.c \
-o ${HELIOS_OBJ}rndoff.o

${HELIOS_OBJ}rdunit.o: \
${HELIOS_MATH}rdunit.c
	@echo "===> Building rdunit(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}rdunit.c \
-o ${HELIOS_OBJ}rdunit.o

${HELIOS_OBJ}txcnsl.o: \
${HELIOS_MATH}txcnsl.c \
${HELIOS_OBJ}lotos.o \
${HELIOS_OBJ}dotos.o
	@echo "===> Building txcnsl(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}txcnsl.c \
-o ${HELIOS_OBJ}txcnsl.o

${HELIOS_OBJ}cpylne.o: \
${HELIOS_TOOLS}cpylne.c 
	@echo "===> Building cpylne(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_TOOLS}cpylne.c \
-o ${HELIOS_OBJ}cpylne.o

${HELIOS_OBJ}shutil.o: \
${HELIOS_MATH}shutil.c
	@echo "===> Building shutil(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}shutil.c \
-o ${HELIOS_OBJ}shutil.o

${HELIOS_OBJ}rtbis.o: \
${HELIOS_MATH}rtbis.c
	@echo "===> Building rtbis(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}rtbis.c \
-o ${HELIOS_OBJ}rtbis.o

${HELIOS_OBJ}fdjac.o: \
${HELIOS_MATH}fdjac.c
	@echo "===> Building fdjac(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}fdjac.c \
-o ${HELIOS_OBJ}fdjac.o

${HELIOS_OBJ}gssjpv.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}gssjpv.c
	@echo "===> Building gssjpv(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}gssjpv.c \
-o ${HELIOS_OBJ}gssjpv.o

${HELIOS_OBJ}fluids.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}fluids.h \
${HELIOS_SRC}fluids.c
	@echo "===> Building fluids(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}fluids.c \
-o ${HELIOS_OBJ}fluids.o

${HELIOS_OBJ}dielct.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}dielct.h \
${HELIOS_SRC}dielct.c
	@echo "===> Building dielct(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}dielct.c \
-o ${HELIOS_OBJ}dielct.o

${HELIOS_OBJ}solids.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}solids.h \
${HELIOS_SRC}solids.c
	@echo "===> Building solids(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}solids.c \
-o ${HELIOS_OBJ}solids.o

${HELIOS_OBJ}metals.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}metals.h \
${HELIOS_SRC}metals.c
	@echo "===> Building metals(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}metals.c \
-o ${HELIOS_OBJ}metals.o

${HELIOS_OBJ}hcndct.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}hcndct.h \
${HELIOS_SRC}hcndct.c
	@echo "===> Building hcndct(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}hcndct.c \
-o ${HELIOS_OBJ}hcndct.o

${HELIOS_OBJ}graphp.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_OBJ}rndoff.o \
${HELIOS_SRC}types.h \
${HELIOS_SRC}graphp.c
	@echo "===> Building graphp(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}graphp.c \
-o ${HELIOS_OBJ}graphp.o

${HELIOS_OBJ}values.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}vtempr.h \
${HELIOS_SRC}store_val.h \
${HELIOS_SRC}values.c
	@echo "===> Building values(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}values.c \
-o ${HELIOS_OBJ}values.o

${HELIOS_OBJ}temprs.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}temprs.c
	@echo "===> Building temprs(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}temprs.c \
-o ${HELIOS_OBJ}temprs.o

${HELIOS_OBJ}losses.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}losses.h \
${HELIOS_SRC}losses.c
	@echo "===> Building losses(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}losses.c \
-o ${HELIOS_OBJ}losses.o

${HELIOS_OBJ}kelvin.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}kelvin.h \
${HELIOS_SRC}kelvin.c
	@echo "===> Building kelvin(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}kelvin.c \
-o ${HELIOS_OBJ}kelvin.o

${HELIOS_OBJ}nusselt.o: \
${HELIOS_MATH}nusselt.c
	@echo "===> Building nusselt(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}nusselt.c \
-o ${HELIOS_OBJ}nusselt.o

${HELIOS_OBJ}grazet.o: \
${HELIOS_MATH}grazet.c
	@echo "===> Building grazet(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}grazet.c \
-o ${HELIOS_OBJ}grazet.o

${HELIOS_OBJ}granus.o: \
${HELIOS_MATH}granus.c
	@echo "===> Building granus(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}granus.c \
-o ${HELIOS_OBJ}granus.o

${HELIOS_OBJ}trnorm.o: \
${HELIOS_MATH}trnorm.c
	@echo "===> Building trnorm(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_MATH}trnorm.c \
-o ${HELIOS_OBJ}trnorm.o

${HELIOS_OBJ}hlsdrv.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}hlsdrv.c \
${HELIOS_SRC}hlsdrv.h \
${HELIOS_SRC}inputs.h \
${HELIOS_DEFLT}deflt_opr.h \
${HELIOS_DEFLT}deflt_mat.h \
${HELIOS_DEFLT}deflt_par.h
	@echo "===> Building hlsdrv(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}hlsdrv.c \
-o ${HELIOS_OBJ}hlsdrv.o

${HELIOS_OBJ}intlze.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}intlze.h \
${HELIOS_SRC}intlze.c
	@echo "===> Building intlze(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}intlze.c \
-o ${HELIOS_OBJ}intlze.o

${HELIOS_OBJ}hlswrk.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}hlswrk.h \
${HELIOS_SRC}hlswrk.c
	@echo "===> Building hlswrk(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}hlswrk.c \
-o ${HELIOS_OBJ}hlswrk.o

${HELIOS_OBJ}heliostest.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}heliostest.c
	@echo "===> Building heliostest(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}heliostest.c \
-o ${HELIOS_OBJ}heliostest.o

${HELIOS_OBJ}helios.o: \
${HELIOS_CONF}CONFIG.H \
${HELIOS_SRC}types.h \
${HELIOS_SRC}helios.c
	@echo "===> Building helios(*)"
	${HELIOS_CC} ${HELIOS_CFLG} -c ${HELIOS_SRC}helios.c \
-o ${HELIOS_OBJ}helios.o

# clean binary programs [ in directores ..._BIN ] and all object files:
clean: perm
	@echo "===> Cleaning for ${HELIOS_BIN}"
	rm -f ${HELIOS_BIN}${HELIOS_EXEC}
	@echo "===> Cleaning objects"
	rm -f ${HELIOS_OBJ}*.o
	@echo "===> ${DONE}"

# Deinstall executable
deinstall: perm
	@echo "===> Deinstalling ${HELIOS_EXEC}"
	rm -f ${HELIOS_INST}${HELIOS_EXEC}

# Reinstall executable
reinstall: install
	@echo "===> Reinstalling ${HELIOS_EXEC}"
	@echo "===> ${DONE}"

# Give write permissions to objects dir
perm:
	-@chmod -f u+wx,g+wx,o+wx ${HELIOS_OBJ}
	-@chmod -f u+wx,g+wx,o+wx ${HELIOS_BIN}
#------------------------------------------------------------------------------
DONE=OK, done !
