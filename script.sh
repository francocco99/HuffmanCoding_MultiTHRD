> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<30;i++)); 
    do ./HuffmanMultithreadv2.out text3.txt $i >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<30;i++)); 
    do ./HuffmanFastFlow.out text3.txt $i >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./Huffman1.out text3.txt >> sequential.csv