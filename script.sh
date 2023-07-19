> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<13;i++)); 
    do ./HuffmanMultithreadv2.out text4.txt $i >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<13;i++)); 
    do ./HuffmanFastFlow.out text4.txt $i >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./Huffman1.out text4.txt >> sequential.csv