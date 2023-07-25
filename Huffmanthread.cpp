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
string myString;
int delta,len;
int w;

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
//Aggiunto commento
struct nodeTree* Newn(char data,int freq)
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

//function for compute the frequency in parallel
void BodyParallel(int p,  vector<vector<int>> &listmps)
{   
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
   // cout << "first: " << first << " last: " << last << endl;
   listmps[p]=vector<int>(256,0);
    for(int i=first;i<last;i++)
    {
       
       listmps[p][myString[i]]++;
    }
}
void ComputeFrequency(vector<int> &mpp)
{
    vector<thread*> Threads; //vector of thread
    vector<vector<int>> listmps(w);
    len=myString.size();
    delta=len/w;
    for(int i=0;i<w;i++)
    {
        Threads.push_back(new thread(BodyParallel,i,ref(listmps)));
    }
    for(auto t: Threads)
    {
        t->join();
    }
   
    
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            mpp[j] += listmps[i][j];
        }
    }
     

}

// Functions for parallel transform the string in binary values 
void paralEncode(int p,vector<string>  &cds,vector <string>Huffcode)
{   
     
    int first,last;
    string output;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
    
    
    for(int i=first;i<last;i++)
    {
        output+= Huffcode[myString[i]];
    }
     cds[p] = output;
}

string Encode(vector <string>Huffcode)
{
    vector<thread*> Threads;
    vector<string> Codes (w);
    string result;
   
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(paralEncode,i, ref(Codes),Huffcode));
       
    }
    for(auto t: Threads)
    {
        t->join();
    }
    for( string s: Codes)
    {
        result+= s;
    }
    
    
    return result;
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


int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    string Filename;
    string result;

    vector<int> mpp(256,0);
    vector <string>Huffcode(256,"");
    
    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return(0);
    }
    if(argc<3)
    {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return 0;
    }
    w=atoi(argv[2]);
    Filename=argv[1]; 
    ifstream t(Filename);
    if(t.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    
    ofstream out("textOut.bin",ios::out | ios::binary);
    stringstream buf;
    buf << t.rdbuf();
    myString=buf.str();
   
    long usecs1;
    
    {
        utimer t0("parallel computation",&usecs1); 
        ComputeFrequency(ref(mpp));
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        result=Encode(Huffcode);
    }
   
    cout  << usecs1 << "," << w << endl;    
    
    WriteFile(result); 
     
       
}