> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<12;i++)); 
    do ./Huffmanthread.out Test/test10Mega.txt $i 0 >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<12;i++)); 
    do ./HuffmanFastFlow.out Test/test10Mega.txt $i 0 >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./HuffmanSeq.out Test/test10Mega.txt 0 >> sequential.csv
