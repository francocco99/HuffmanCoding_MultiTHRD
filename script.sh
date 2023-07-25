> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<12;i++)); 
    do ./Huffmanthread.out Huge.txt $i >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<12;i++)); 
    do ./HuffmanFastFlow.out Huge.txt $i >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./HuffmanSeq.out Huge.txt >> sequential.csv
