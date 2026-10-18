#!/bin/bash
# score.sh FUNC cand.c... : try each candidate, print opcode-diff score (lower is better)
S="$(cd "$(dirname "$0")" && pwd)"
cd "${REPO:-$HOME/rac1-decomp}"
export WINEDEBUG=-all WINEPREFIX=/opt/wineprefix PATH=/opt/venv/bin:$PATH
F=$1; shift
for c in "$@"; do
  r=$(python tools/try_func.py $F $c --arm=s --no-budget 2>&1 | tail -1)
  if [ -f build-sn/try/$F/s/obj.o ] && ! echo "$r" | grep -q COMPILE; then
    sc=$(python $S/align.py $F build-sn/try/$F/s/obj.o 2>/dev/null | python3 -c "
import sys,re
n=0
for l in sys.stdin:
    m=re.match(r'--- \w+ ours\[(\w+):(\w+)\] retail\[(\w+):(\w+)\]',l)
    if m:
        a,b,c,d=[int(x,16) for x in m.groups()]; n+=max(b-a,d-c)//4
print(n)")
  else sc=NA; fi
  echo "$(basename $c)  score=$sc  $(echo $r | sed 's/.*: //')"
  rm -f build-sn/try/$F/s/obj.o
done
