#!/bin/bash

freq=0
e=0
build=0
readmedia=0
writemedia=0
freqmedia=0
eqmedia=0
buildmedia=0
read=0
write=0
if [ -d "log5Mega" ]; then
   cd log5Mega
   file=$(ls log5seq.txt)
   n=$(grep -o "F" "$file" | wc -l)

   for k in $(grep "r" "$file" | cut -c 3-); do
      read=$read+$k
   done

   for k in $(grep "F" "$file" | cut -c 3-); do
      freq=$freq+$k
   done 
   
   for k in $(grep "e" "$file" | cut -c 3-); do
      e=$e+$k
   done 
   
   for k in $(grep "b" "$file" | cut -c 3-); do
      build=$build+$k
   done

   for k in $(grep "w" "$file" | cut -c 3-); do
      write=$write+$k
   done  

   freq=$(bc <<< $freq)
   e=$(bc <<< $e)
   build=$(bc <<< $build)
   read=$(bc <<< $read)
   write=$(bc <<< $write)

   freqmedia=$freq/$n
   freqmedia=$(bc <<< $freqmedia)
   echo "size media calcolo della frequenza" $freqmedia

   buildmedia=$build/$n
   buildmedia=$(bc <<< $buildmedia)
   echo "size media costruzione albero" $buildmedia

   eqmedia=$e/$n
   eqmedia=$(bc <<< $eqmedia)
   echo "size media encoding" $eqmedia

   readmedia=$read/$n
   readmedia=$(bc <<< $readmedia)
   echo "size media read" $readmedia

   writemedia=$write/$n
   writemedia=$(bc <<< $writemedia)
   echo "size media write" $writemedia

    

fi
