CC= g++ -std=c++2a
THREAD=-lpthread
CFLAGS= -Wall -O3  

Huffman: HuffmanSeq.cpp
	$(CC)  BuildHuffman.hpp -o HuffmanSeq.out  BuildHuffman.cpp HuffmanSeq.cpp  $(CFLAGS) 
HuffmanThread: Huffmanthread.cpp
	$(CC)  BuildHuffman.hpp -o Huffmanthread.out BuildHuffman.cpp Huffmanthread.cpp     $(CFLAGS) $(THREAD) 

HuffmanFast: HuffmanFastFlow.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanFastFlow.out BuildHuffman.cpp  HuffmanFastFlow.cpp    $(CFLAGS) $(THREAD)

HuffmanSeqTemp: HuffmanSeqTemp.cpp
	$(CC)   BuildHuffman.hpp -o  HuffmanSeqTemp.out BuildHuffman.cpp HuffmanSeqTemp.cpp    $(CFLAGS) $(THREAD)

HuffmanThreadTemp: HuffmanthreadTemp.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanthreadTemp.out BuildHuffman.cpp HuffmanthreadTemp.cpp     $(CFLAGS) $(THREAD)

HuffmanFastTemp: HuffmanFastTemp.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanFastTemp.out BuildHuffman.cpp HuffmanFastTemp.cpp  $(CFLAGS) $(THREAD)	


all: Huffman HuffmanThread HuffmanFast HuffmanSeqTemp HuffmanThreadTemp HuffmanFastTemp

script: Huffman HuffmanMulti HuffmanFast
	./script.sh




cleanall:
	rm -f Huffman1.out HuffmanMultithreadv2.out HuffmanFastFlow.out