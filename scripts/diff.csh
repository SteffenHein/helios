#!/bin/csh
set init=0
set final=1000

set FIRST_FILE="hls.val"
set SECOND_FILE="hls.val"
# now iterate

@ i = ${init}
while ( $i <= ${final} )
	if (-f ./${FIRST_FILE}$i) then
 		echo "comparing file ${FIRST_FILE}$i to ${SECOND_FILE}$i""prv"""
		diff ${FIRST_FILE}$i ${SECOND_FILE}$i".prv"
	endif
	@ i ++
end
#
echo "Done"
