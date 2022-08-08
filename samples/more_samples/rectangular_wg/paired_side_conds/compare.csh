#!/bin/csh
set init=$1
set final=$2

set FIRST_FILE="hls.val"
set SECOND_FILE="../2.0r0/hls.val"
# now iterate
@ i = ${init}
while ($i <= ${final})
	if (-f ./${FIRST_FILE}$i) then
 		echo "comparing file ${FIRST_FILE}$i to ${SECOND_FILE}$i"
		diff ${FIRST_FILE}$i ${SECOND_FILE}$i
	endif
	@ i ++
end
#
echo "Done"
