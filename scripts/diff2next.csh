#!/bin/csh
set init=0
set final=90

set FIRST_FILE="hls.val"
set SECOND_FILE="hls.val"
# now iterate
@ i = ${init}
@ j = ${init}
@ j ++
while ($i <= ${final})
	if (-f ./${FIRST_FILE}$i) then
 		echo "comparing file ${FIRST_FILE}$i to ${SECOND_FILE}$j"
		diff ${FIRST_FILE}$i ${SECOND_FILE}$j
	endif
	@ i ++
	@ j ++
end
#
echo "Done"
