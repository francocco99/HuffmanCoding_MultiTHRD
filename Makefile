CC= g++ -std=c++2a
THREAD=-lpthread
CFLAGS= -Wall -O3  

Huffman: HuffmanSeq.cpp
	$(CC) $(CFLAGS)  -o HuffmanSeq.out HuffmanSeq.cpp   $(THREAD)  

HuffmanThread: Huffmanthread.cpp
	$(CC) $(CFLAGS) Huffmanthread.cpp -o Huffmanthread.out    $(THREAD) 

HuffmanFast: HuffmanFastFlow.cpp
	$(CC) $(CFLAGS) HuffmanFastFlow.cpp -o HuffmanFastFlow.out    $(THREAD)

HuffmanSeqTemp: HuffmanSeqTemp.cpp
	$(CC) $(CFLAGS) HuffmanSeqTemp.cpp -o HuffmanSeqTemp.out    $(THREAD)

HuffmanThreadTemp: HuffmanthreadTemp.cpp
	$(CC) $(CFLAGS) HuffmanthreadTemp.cpp -o HuffmanthreadTemp.out    $(THREAD)

HuffmanFastTemp: HuffmanFastTemp.cpp
	$(CC) $(CFLAGS) HuffmanFastTemp.cpp -o HuffmanFastTemp.out    $(THREAD)	


all: Huffman HuffmanThread HuffmanFast HuffmanSeqTemp HuffmanThreadTemp HuffmanFastTemp

script: Huffman HuffmanMulti HuffmanFast
	./script.sh

scriptfast: Huffman HuffmanMulti HuffmanFast
	./scriptfast.sh


cleanall:
	rm -f Huffman1.out HuffmanMultithreadv2.out HuffmanFastFlow.out
