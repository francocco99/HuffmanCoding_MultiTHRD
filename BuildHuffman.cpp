#include "BuildHuffman.hpp"

/*function used to create the Huffman Tree
mpp--> map of char,frequency
*/
struct nodeTree* BuildHuffman(unordered_map<char,int> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
   
    for(auto a:mpp)
    {
        // Create the leaves of the Huffman tree.
        pq.push(Newn(a.first,a.second));  
    }
  
    while (pq.size()!=1)
    {
        left=pq.top();pq.pop();
        right=pq.top();pq.pop();

        //create new internal node
        center=Newn('#',left->freq+right->freq);
        center->left=left;
        center->right=right;
        pq.push(center);

    }
    //return the root
    return pq.top();
};

//function used to create new node of the tree
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
    //padding of the string
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
        //if is a leaf, save the encoding
		Huffcode[node->a] = str;
	}
    saveEncode(node->left,str+"0",Huffcode);
    saveEncode(node->right,str+"1",Huffcode);
}