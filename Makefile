CC= g++ -std=c++11
THREAD=-lpthread
CFLAGS= -Wall  -O3

Huffman: Huffman1.cpp
	$(CC) $(CFLAGS)  -o Huffman1.out Huffman1.cpp     

HuffmanMulti: Huffman2Multithreadv2.cpp
	$(CC) $(CFLAGS) Huffman2Multithreadv2.cpp -o HuffmanMultithreadv2.out    $(THREAD) 

HuffmanFast: HuffmanFastFlow.cpp
	$(CC) $(CFLAGS) HuffmanFastFlow.cpp -o HuffmanFastFlow.out    $(THREAD)


all: huffman1 HuffmanMulti HuffmanFast



cleanall:
	rm -f Huffman1.out HuffmanMultithreadv2.out HuffmanFastFlow.out