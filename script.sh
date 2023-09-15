> thread.csv
> fast.csv
> sequential.csv

echo "usec,nw" >> thread.csv
for((i=1;i<12;i++)); 
    do ./HuffmanThread.out Test/test10Mega.txt $i 1 >> thread.csv; 
done
echo "usec,nw" >> fast.csv
for((i=1;i<12;i++)); 
    do ./HuffmanFast.out Test/test10Mega.txt $i 1 >> fast.csv; 
done

echo "usec,nw" >> sequential.csv
./HuffmanSeq.out Test/test10Mega.txt 1 >> sequential.csv
