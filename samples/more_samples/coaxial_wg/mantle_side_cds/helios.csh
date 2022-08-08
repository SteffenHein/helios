#!/bin/csh
set init=$1
set final=$2

set LOG_FILE="par.log"
set INIT_FILE="par.init"
# now iterate

@ i = ${init}
while ( $i <= ${final} )
	if (-f ./${INIT_FILE}$i) then
		echo "calling helios ""$i"
		helios -g -n $i
 		cp ${LOG_FILE}$i ${INIT_FILE}$i
	endif
	@ i ++
end
#
echo "Done"
