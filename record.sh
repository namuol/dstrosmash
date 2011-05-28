#!/bin/bash
u=$(date +%N)
vfn=gameplay$u.avi
afn=gameplay$u.wav
ffn=gameplay$u.mkv

nds="wine $NOCASHGBA/NO\$GBA.EXE"
$nds dstrosmash.nds &
sleep 4
jackrec -f $afn wine_jack_out_0:out_l wine_jack_out_0:out_r & 
ffcast -r 30 -f avi -j '256x192+10+48' -o $vfn
killall -9 jackrec
mencoder $vfn -ovc copy -oac copy -audiofile $afn -delay 0.2 -o $ffn
