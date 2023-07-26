#!/bin/bash

freq=0
e=0
build=0
freqmedia=0
eqmedia=0
buildmedia=0
if [ -d "log" ]; then
   cd log
   file=$(ls)
   n=$(grep -o "F" "$file" | wc -l)


    for k in $(grep "F" "$file" | cut -c 3-); do
      freq=$freq+$k
   done 
    for k in $(grep "e" "$file" | cut -c 3-); do
      e=$e+$k
   done 
    for k in $(grep "b" "$file" | cut -c 3-); do
      build=$build+$k
   done  

   freq=$(bc <<< $freq)
   e=$(bc <<< $e)
   build=$(bc <<< $build)

    freqmedia=$freq/$n
    freqmedia=$(bc <<< $freqmedia)
    echo "size media calcolo della frequenza" $freqmedia

    buildmedia=$build/$n
    buildmedia=$(bc <<< $buildmedia)
    echo "size media costruzione albero" $buildmedia

    eqmedia=$e/$n
    eqmedia=$(bc <<< $eqmedia)
    echo "size media encoding" $eqmedia

    

fi