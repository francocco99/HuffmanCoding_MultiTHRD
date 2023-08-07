#include "BuildHuffman.hpp"
#include <queue>
#include <fstream>
#include <sstream>

// function that built the Huffman tree
struct nodeTree* BuildHuffman(unordered_map<char,int> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
   
    for(auto a:mpp)
    {
      pq.push(Newn(a.first,a.second));  
    }
  
    while (pq.size()!=1)
    {
        left=pq.top();pq.pop();
        right=pq.top();pq.pop();

        //creo il nuovo nodo interno
        center=Newn('#',left->freq+right->freq);
        center->left=left;
        center->right=right;
        pq.push(center);

    }
    return pq.top();
};
//funtion that create new node for the huffman tree
struct nodeTree* Newn(char data,int freq)
{
    struct nodeTree* temp = (struct nodeTree*)malloc(sizeof(struct nodeTree));
    temp->left = temp->right=nullptr;
    temp->a = data;
    temp->freq = freq;
 
    return temp;    
}

bool Compare::operator()(nodeTree* below, nodeTree * above)
{
    return below->freq>above->freq;
};
//Write the encoded string in the file.
void WriteFile(string result)
{
    int size = result.size();
    int adds = size % 8;
    if(adds!=0)
    {
        adds = 8 - adds;
    }
    result.append(adds, '0');
    ofstream out("textOut.bin",ios::out | ios::binary);
    unsigned bufs=0, bits=0;
    for(char a: result)
    {
        
       if(bits==8)
        {
            out.put(bufs);      
            bufs=atoi(&a);
            bits=1;
        }
        else
        {   
            bufs=(bufs<<1) | (atoi(&a)) ; 
            bits++;
        }
    }
    if(bits==8 || bits <8)
    {
        out.put(bufs); 
    } 
}
//funtion that create the map that associate at each char the encoding, traversing the tree
void saveEncode(nodeTree* node,string str, unordered_map<char,string>&Huffcode)
{
    if(node==nullptr)
        return;
    if (!node->left && !node->right) {
		Huffcode[node->a] = str;
	}
    saveEncode(node->left,str+"0",Huffcode);
    saveEncode(node->right,str+"1",Huffcode);
}