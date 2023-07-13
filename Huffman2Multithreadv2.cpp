#include <iostream>
#include <string>
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
typedef pair<char,double> pai;
mutex Lockmp;
string myString;
int delta,len;
int w;
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
void BodyParallel(int p,  vector<map<char,double>> &listmps)
{   
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
   // cout << "first: " << first << " last: " << last << endl;
    for(int i=first;i<last;i++)
    {
       
       listmps[p][myString[i]]++;
    }
}
void ComputeFrequency(map<char,double> &mpp)
{
    vector<thread*> Threads; //vector of thread
    vector<map<char,double>> listmps(w);
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
    map<char,double>::iterator it;
    
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

// Functions for parallel transform the string in binary values 
void paralEncode(int p, map<pair<int,int>,string> &mps,map <char,string>Huffcode)
{   
     
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
    //cout << "first: " << first << " last: " << last << endl;
    for(int i=first;i<last;i++)
    {
        mps[{first,last}]=mps[{first,last}]+ Huffcode[myString[i]];
    }
}

string Encode(map <char,string>Huffcode)
{
    vector<thread*> Threads;
    map<pair<int,int>,string> maps;
    string result;
    //cout << "non ci entro "  << w << endl;
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(paralEncode,i, ref(maps),Huffcode));
    }
    for(auto t: Threads)
    {
        t->join();
    }
    map<pair<int,int>,string>::iterator it = maps.begin();
    while (it != maps.end())
    {
        //std::cout << "Inizio: " << it->first.first  <<"Fine: " << it->first.second << std::endl;
        result= result+it->second;
        ++it;
    }
    return result;
}  

int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    
    string result;
    
    w=(argc > 1 ? atoi(argv[1]) : 3); //number of workers
    ifstream t("text3.txt");
    ofstream out("textOut.bin",ios::out | ios::binary);
    stringstream buf;
    buf << t.rdbuf();
    myString=buf.str();
    unsigned bufs=0, bits=0;
    long usecs1;
    long usecs2;
    long usecs3;
    {
        utimer t0("parallel computation",&usecs1); 
        map<char,double> mpp;
        ComputeFrequency(ref(mpp));
        map <char,string>Huffcode;
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        result=Encode(Huffcode);
    }
    cout << "End (spent " << usecs1 << " usecs using " << w << " threads)"  << endl;    
    //cout<< "Result of Encoding is: " << result << endl;
    // write in the file
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