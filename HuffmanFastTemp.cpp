#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <ff/ff.hpp>
#include <string.h>
#include <ff/parallel_for.hpp>
#include "utimer.hpp"
using namespace std;
using namespace ff;
//pipeline farm
typedef pair<char,int> pai;


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

struct nodeTree* BuildHuffman(map<char,int> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
    map<char, int>::iterator it = mpp.begin();
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

//function for compute the frequency in parallel


// Functions for parallel transform the string in binary values 



int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    string result;
    string myString;
    string Filename;
    map <char,string>Huffcode;
    map<char,int> mpp;
    
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
    Filename=argv[1]; //number of workers
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
    
    long freq;
    long buildtemp;
    long encode;
    
     
    {
        utimer t0("parallel computation",&freq);
        ParallelFor pfr(w);
        
        int p=0;
        vector<map<char,int>> listmps(w);
        pfr.parallel_for_thid(0,myString.size(),1,0,[&listmps,&myString](const long idx,const int thid){
          
            
            listmps[thid][myString[idx]]++;
                
            
        });
        map<char,int>::iterator it;
        for(auto a: listmps)
        {
            it=a.begin();
            while (it != a.end())
            {
                mpp[it->first]=mpp[it->first]+it->second;
                ++it;
            }
        }
    }
    {
        utimer t0("parallel computation",&buildtemp);
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }
    {
        utimer t0("parallel computation",&encode);
        ParallelFor pfr2(w);
        map<pair<int,int>,string> maps;
        pfr2.parallel_for_idx(0,myString.size(),1,0,[&](const long first, const long last,const int thid){
        for(int i=first;i<last;i++)
        {
            maps[{first,last}]=maps[{first,last}]+ Huffcode[myString[i]];
        }
            
        });

        map<pair<int,int>,string>::iterator it2 = maps.begin();
        while (it2 != maps.end())
        {
            //std::cout << "Inizio: " << it->first.first  <<"Fine: " << it->first.second << std::endl;
            result= result+it2->second;
            ++it2;
        }
    }
    cout << "End (spent for Frequency " << freq << " usecs" << endl;
    cout << "End (spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End (spent  encode " << encode << " usecs" << endl;

    //cout << result << endl;
   
     
       
}
