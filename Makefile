CC= g++ -std=c++2a
THREAD=-lpthread
CFLAGS= -Wall -O3  

HuffmanSeq: HuffmanSeq.cpp
	$(CC)  BuildHuffman.hpp -o HuffmanSeq.out  BuildHuffman.cpp HuffmanSeq.cpp  $(CFLAGS) 

HuffmanThread: HuffmanThread.cpp
	$(CC)  BuildHuffman.hpp -o HuffmanThread.out BuildHuffman.cpp HuffmanThread.cpp     $(CFLAGS) $(THREAD) 

HuffmanFast: HuffmanFast.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanFast.out BuildHuffman.cpp  HuffmanFast.cpp    $(CFLAGS) $(THREAD)

HuffmanSeqTemp: HuffmanSeqTemp.cpp
	$(CC)   BuildHuffman.hpp -o  HuffmanSeqTemp.out BuildHuffman.cpp HuffmanSeqTemp.cpp    $(CFLAGS) 

HuffmanThreadTemp: HuffmanThreadTemp.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanThreadTemp.out BuildHuffman.cpp HuffmanThreadTemp.cpp     $(CFLAGS) $(THREAD)

HuffmanFastTemp: HuffmanFastTemp.cpp
	$(CC)   BuildHuffman.hpp -o HuffmanFastTemp.out BuildHuffman.cpp HuffmanFastTemp.cpp  $(CFLAGS) $(THREAD)	


all: HuffmanSeq HuffmanThread HuffmanFast HuffmanSeqTemp HuffmanThreadTemp HuffmanFastTemp




cleanall:
	rm -f HuffmanSeq.out HuffmanThread.out HuffmanFast.out HuffmanSeqTemp.out HuffmanThreadTemp.out  HuffmanFastTemp.out
