#!/bin/csh
set init=0
set final=100

set COPIED_FILE="hls.val"
set TARGET_FILE="hls.val"
set SUFFIX=".prv"
# now iterate

@ i = ${init}
while ( $i <= ${final} )
   if (-f ./${COPIED_FILE}$i) then
	echo "copying file ${COPIED_FILE}$i to ${TARGET_FILE}$i${SUFFIX}"
	cp ${COPIED_FILE}$i ${TARGET_FILE}$i${SUFFIX}
   endif
   @ i ++
end
#
echo "Done"
