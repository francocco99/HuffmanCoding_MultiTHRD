#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <bitset>
#include "utimer.hpp"
using namespace std;
typedef pair<char,double> pai;
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
//Aggiunto commento
struct nodeTree* Newn(char data,double freq)
{
    struct nodeTree* temp = (struct nodeTree*)malloc(sizeof(struct nodeTree));
    temp->left = temp->right=nullptr;
    temp->a = data;
    temp->freq = freq;
 
    return temp;    
}

struct nodeTree* BuildHuffman(map<char,double> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
    map<char, double>::iterator it = mpp.begin();
    while (it != mpp.end())
    {
        pq.push(Newn(it->first,it->second));
        ++it;
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

void PrintTree(nodeTree* tree)
{
    if(tree==nullptr){
        return; 
    }
    cout << tree->a <<endl;
    PrintTree(tree->left);
    PrintTree(tree->right);
}
void saveEncode(nodeTree* node,string str, map <char,string>&Huffcode)
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
int main(int argc,char* argv[])
{
    map<char,double> mpp;
    ifstream myfile;
    string myString;
    string Filename;
    string temp;
    float len;
    vector<pai> vec;
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
    buf << t.rdbuf();
    
    myString=buf.str();
  //  cout << myString << endl;
    //take the number of occurences for each char
      long usecs; 
    {
        utimer t0("parallel computation",&usecs); 
        for(char&c : myString)
        {         
            mpp[c]=mpp[c]+1.0;
        }
        map<char, double>::iterator it = mpp.begin();
        len=myString.size();
        
        map <char,string>Huffcode;
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        
        result=" ";
        for(char&c : myString)
        { 
            result=result+Huffcode[c];
        }
    }
    cout << "End (spent " << usecs << " usecs" << endl;
   
   
    unsigned bufs=0, bits=0;
    // write  the char in the files
    for(char a: result)
    {
        
       if(bits==8)
        {
           
            out.put(bufs);      
            bufs=0;
            bits=0;
        }
        else
        {
            bufs=(bufs<<1) | (atoi(&a) & 1);
            bits++;
        }
    }
    if(bits>=8)
    {
        bits-=8;
        out.put(bufs >>bits);
    }
    

    
       
}