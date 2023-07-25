#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <execution>
#include <thread>
#include <mutex>
#include "utimer.hpp"

using namespace std;
int len; // len of the string
string myString; // string tht contain the string to process


struct nodeTree
{
    char a;
    int freq;
    nodeTree *left, *right;
};

class Compare {
public:
    bool operator()(nodeTree * below, nodeTree * above)
    {
        
        return below->freq>above->freq;
        
        
    }    
};


struct nodeTree* Newn(char data,int freq)
{
    struct nodeTree* temp = (struct nodeTree*)malloc(sizeof(struct nodeTree));
    temp->left = temp->right=nullptr;
    temp->a = data;
    temp->freq = freq;
 
    return temp;    
}

//construct the huffman tree
struct nodeTree* BuildHuffman(vector<int> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
    int len=mpp.size();
    for(int i=0;i< len;i++)
    {
        if(mpp[i]!=0)
            pq.push(Newn(char(i),mpp[i]));
       
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



//traverse the tree for create the encoding of a char
void saveEncode(nodeTree* node,string str, vector <string>&Huffcode)
{
    if(node==nullptr)
        return;
    // if there is a leaf i save the encoding of the char
    if (!node->left && !node->right) {
		Huffcode[node->a] = str;
	}
    saveEncode(node->left,str+"0",Huffcode);
    saveEncode(node->right,str+"1",Huffcode);
}

//encode the tesxt using the right code for each letter
string Encode(vector <string>Huffcode)
{
    string result;
    for(int i=0;i<len;i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

//compute frequency of the letter in the text
void ComputeFrequency(vector<int> &mp)
{
    for(int i=0;i<len;i++)
    {         
        mp[myString[i]]++;
    }
}

// function use to write the encoded string to file

void WriteFile(string result)
{
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


int main(int argc,char* argv[])
{
    
    ifstream myfile;
    string Filename;
    string temp;
    string result;
    vector<int> mpp(256,0);
    vector <string>Huffcode(256,"");
    
    
    
    
    ofstream out("textOut.bin",ios::out | ios::binary);
    stringstream buf;
    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return(0);
    }
    if(argc==1)
    {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return 0;
    }
    Filename=argv[1]; //number of workers
    ifstream t(Filename);
    if(t.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    buf << t.rdbuf();
    myString=buf.str();
    len=myString.size();
    
    //take the number of occurences for each char
      long usecs; 
    {
        utimer t0("parallel computation",&usecs); 
        ComputeFrequency(ref(mpp));  
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        result=Encode(Huffcode);        
       
    }
    //cout << "End (spent " << usecs << " usecs" << endl;
    cout << usecs << ",1" << endl;
    
   WriteFile(result);   
    
       
}