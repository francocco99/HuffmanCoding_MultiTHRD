# HuffmanCoding_MultiTHRD
## Project Parallel and Distributed Sytems : Paradigms and Models, 2022-2023

Huffman code is an optimal prefix code that is commonly used for lossless data compression. 
Three different implementations are presented: a *sequential* one, a *parallelized version* leveraging the multi-threading tools provided by **C++** and one that utilizes the **Fast Flow** framework.
In addition to writing the decompressed file, the elapsed times for performing the calculation are printed. In the ***.temp*** implementations, the times printed are divided into various stages to calculate Huffman coding.

![Alt text](https://media.geeksforgeeks.org/wp-content/uploads/20220906180456/6.png)

In addition to the various implementations, there is also a **.py** file for creating performance graphs for the different implementations, as well as **.sh** files, one for running the three implementations a certain number of times and one for calculating time statistics.
### Instruction To Execute

• **make all** to compile all the files  

• ***./HuffmanSeq.out*** "test.txt".

• ***./HuffmanThread.out*** "test.txt"" numberofthreads".

• ***./HuffmanFast.out*** "test.txt" "numberofthreads"
