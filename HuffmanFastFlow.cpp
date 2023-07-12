#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <ff/parallel_for.hpp>
using namespace std;
using namespace ff;
//pipeline farm
typedef pair<char,double> pai;

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
void ComputeFrequency(map<char,double> &mpp)
{
    ParallelFor pf(w);
    pf.parallel_for(0,len,1,0,[&](const long i){
        mpp[myString[i]]++;
    });
    
}
string Encode( map <char,string>Huffcode)
{
    ParallelFor pf(w);
    string result;
    pf.parallel_for(0,len,1,0,[&](const long i){
        result=result+Huffcode[myString[i]];
    });
    return result;
}

int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    
    long usec;
    
    w=(argc > 1 ? atoi(argv[1]) : 3); //number of workers
    ifstream t("text3.txt");
    stringstream buf;
    buf << t.rdbuf();
    myString=buf.str();

    ParallelFor pfr(w);
    int p=0;
    vector<map<char,double>> mps(w);
    //map<char,double> mpp;
    //vector<map<char,double>> mps(w);
    pfr.parallel_for(0,myString.size(),1,0,[&](int i){
       cout << i<< endl;;
    });
   /* map<char,double>::iterator it;
    for(auto a: mps)
    {
        it=a.begin();
        while (it != a.end())
        {
            std::cout << "key:  " << it->first << " Value: " <<it->second << endl;
            ++it;
        }
    }*/
     
       
}
