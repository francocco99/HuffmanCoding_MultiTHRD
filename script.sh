> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<12;i++)); 
    do ./Huffmanthread.out Test/test4Mega.txt $i >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<12;i++)); 
    do ./HuffmanFastFlow.out Test/test4Mega.txt $i >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./HuffmanSeq.out Test/test4Mega.txt >> sequential.csv
