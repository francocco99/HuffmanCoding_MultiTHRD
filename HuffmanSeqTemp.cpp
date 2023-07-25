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
int len;
string myString;

struct nodeTree
{
    char a;
    double freq;
    nodeTree *left, *right;
};

class Compare {
public:
    bool operator()(nodeTree * below, nodeTree * above)
    {
        
        return below->freq>above->freq;
        
        
    }    
};

struct nodeTree* Newn(char data,double freq)
{
    struct nodeTree* temp = (struct nodeTree*)malloc(sizeof(struct nodeTree));
    temp->left = temp->right=nullptr;
    temp->a = data;
    temp->freq = freq;
 
    return temp;    
}

struct nodeTree* BuildHuffman(vector<int> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
   
    for(int i=0;i<256;i++)
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


void saveEncode(nodeTree* node,string str,vector <string>&Huffcode)
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

string Encode(vector <string>Huffcode)
{
    string result;
    for(int i=0;i<len;i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

void ComputeFrequency(vector<int> &mp)
{
    for(int i=0;i<len;i++)
    {         
        mp[myString[i]]++;
    }
}
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

int main(int argc, char* argv[])
{
    //map<char,int> mpp;
    vector<int> mpp(256,0);
    vector <string>Huffcode(256,"");
    ifstream myfile;
    
    string Filename;
    string temp;
    string result;
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
   long usecRead;
    {
        utimer t0("parallel computation",&usecRead);
        buf << t.rdbuf();
        myString=buf.str();
    }
        cout << "End spent for Read the  file " << usecRead << " usecs" << endl;
    len=myString.size();
 
      long  freq; 
      long buildtemp;
      long encode;
    {
        utimer t0("parallel computation",&freq); 
        ComputeFrequency(ref(mpp));
    }
    
    {
        utimer t0("parallel computation",&buildtemp);  
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }   
    {
        utimer t0("parallel computation",&encode);
        result=Encode(Huffcode);
        
    }
    cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;
   
   
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        WriteFile(result);
    }
    cout << "End spent for Write the encoded file " << usecWrite << " usecs" << endl;
       
}