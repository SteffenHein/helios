#!/bin/csh
set init=$1
set final=$2

set FIRST_FILE="par.init"
set SECOND_FILE="par.init"
# now iterate
@ ii = ${init}
@ jj = "0"
while ($ii <= ${final})
	if (-f ./${FIRST_FILE}$ii) then
		mv ${FIRST_FILE}$ii ${SECOND_FILE}$jj
		@ jj ++
	endif
	@ ii ++
end
#
echo "Done"
