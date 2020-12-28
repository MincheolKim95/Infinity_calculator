#!/bin/bash

declare -i score=0
declare -i i=1

echo "[테스트]를 시작합니다."
echo " "

for i in 1 2 3
do
	if echo `./inf_calc ./input/INPUT$i` | grep -q `cat ./output/OUTPUT$i` 2> /dev/null; then
		echo "[TEST $i] OK"
		# shellcheck disable=SC2037
		score+=($i*2)-1

	else 
		echo "[TEST $i] FAILED"
	fi
done

echo " "
echo "[점수]: $score/100"
echo " "
