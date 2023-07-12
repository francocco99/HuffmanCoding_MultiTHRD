#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <fstream>
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

void BodyParallel(map<char,double> &mpp,int p)
{   
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
    for(int i=first;i<last;i++)
    {
        //Inserire lock
        char c=myString[i];
        Lockmp.lock();
           mpp[c]++;
           
        Lockmp.unlock();
        //Inserire lock
    }
}
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
    vector<thread*> Threads; //vector of thread
    long usec;
    
    w=(argc > 1 ? atoi(argv[1]) : 3); //number of workers
    myfile.open("text3.txt");
    if(myfile.is_open()) {
        while (myfile)
        {
            myfile >>myString;
        }
    }
    len=myString.size();
    delta=len/w;
    map<char,double> mpp;
    for(int i=0;i<w;i++)
    {
       
        Threads.push_back(new thread(BodyParallel,ref(mpp),i));
    }
    for(auto t: Threads)
    {
        t->join();
    }
    map<char, double>::iterator it = mpp.begin();
    it = mpp.begin();
    cout << "Num of workers:" << w << endl;
    while (it != mpp.end())
    {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        ++it;
    }
    //Creation of the map char with frequencies
    
    map <char,string>Huffcode;
    nodeTree* Root=BuildHuffman(mpp);
    saveEncode(Root,"",Huffcode);
    string result=Encode(Huffcode);
    
    cout<< "Result of Encoding is: " << result << endl;
     
       
}