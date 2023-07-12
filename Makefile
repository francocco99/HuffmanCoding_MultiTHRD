CC= gcc -std=c++11
THREAD=-lpthread
CFLAGS= -Wall  -O3

huffman1: Huffman1.cpp
	$(CC) $(CFLAGS)    Huffman1.cpp -o Huffman1.out   

huffmanMultith: HuffmanMultithreadv2.cpp
	$(CC) $(CFLAGS) HuffmanMultithreadv2.cpp -o HuffmanMultithreadv2.out    $(THREAD) 



cleanall:
	rm -f Huffman1.out HuffmanMultithreadv2.out