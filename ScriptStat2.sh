#!/bin/bash

freq=0
e=0
build=0
freqmedia=0
eqmedia=0
buildmedia=0
overhead=0
overheadmedia=0
writemedia=0
readmedia=0
ASCIImedia=0
oe=0
oc=0
oa=0
a=0
w=0
r=0;
if [ -d "log5Mega" ]; then
   cd log5Mega
   file=$(ls log5thread.txt)
   n=$(grep -o "F" "$file" | wc -l)


   for k in $(grep "F" "$file" | cut -c 3-); do
      freq=$freq+$k
   done 
   
   for k in $(grep "enc" "$file" | cut -c 5-); do
      e=$e+$k
   done
   
   for k in $(grep "b" "$file" | cut -c 3-); do
      build=$build+$k
   done  
   
   for k in $(grep "oe" "$file" | cut -c 4-); do
      oe=$oe+$k
   done
   
   for k in $(grep "oc" "$file" | cut -c 4-); do
      oc=$oc+$k
   done  

   for k in $(grep "oa" "$file" | cut -c 4-); do
      oa=$oa+$k
   done  
   
   for k in $(grep "w" "$file" | cut -c 3-); do
      w=$w+$k
   done  
   
   for k in $(grep "r" "$file" | cut -c 3-); do
      r=$r+$k
   done

   for k in $(grep "As" "$file" | cut -c 4-); do
      a=$a+$k
   done    
   
   freq=$(bc <<< $freq)
   build=$(bc <<< $build)
   w=$(bc <<< $w)
   r=$(bc <<< $r)
   e=$(bc <<< $e)
   a=$(bc <<< $a)
   oe=$(bc <<< $oe)
   oc=$(bc <<< $oc)
   oa=$(bc <<< $oa)

   readmedia=$r/$n
   readmedia=$(bc <<< $readmedia)
   echo "size media calcolo della lettura" $readmedia

   freqmedia=$freq/$n
   freqmedia=$(bc <<< $freqmedia)
   echo "size media calcolo della frequenza" $freqmedia

   buildmedia=$build/$n
   buildmedia=$(bc <<< $buildmedia)
   echo "size media costruzione albero" $buildmedia

   eqmedia=$e/$n
   eqmedia=$(bc <<< $eqmedia)
   echo "size media encoding" $eqmedia

   overheadmedia=$oe/$n
   overheadmedia=$(bc <<< $overheadmedia)
   echo "size media overhead encoding" $overheadmedia

   overheadmedia=$oc/$n
   overheadmedia=$(bc <<< $overheadmedia)
   echo "size media overhead frequency" $overheadmedia

   overheadmedia=$oa/$n
   overheadmedia=$(bc <<< $overheadmedia)
   echo "size media overhead ASCII" $overheadmedia
   
   writemedia=$w/$n
   writemedia=$(bc <<< $writemedia)
   echo "size media writing" $writemedia

   ASCIImedia=$a/$n
   ASCIImedia=$(bc <<< $ASCIImedia)
   echo "size media ASCII calculation:" $ASCIImedia

    

fi