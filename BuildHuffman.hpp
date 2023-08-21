#include <iostream>
#include <string.h>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_map>
#include "utimer.hpp"

using namespace std;
//Strucutre of each node of the Huffman tree
struct nodeTree
{
    char a;
    int freq;
    nodeTree *left, *right;
};
//The function used for managing the priority queue
class Compare{
    public:
        bool operator()(nodeTree* below, nodeTree * above);
};

//function used to create new node of the tree
struct nodeTree* Newn(char data,int freq);

//function used to create the Huffman Tree
struct nodeTree* BuildHuffman(unordered_map<char,int> mpp);

//Function used to construct the mapping of each character's encoding
void saveEncode(nodeTree* node,string str, unordered_map<char,string>&Huffcode);

//function used to perform ASCII encoding, only used by the sequential implementation
void WriteFile(string result);

