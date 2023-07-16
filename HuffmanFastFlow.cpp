#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <sstream>
#include <ff/ff.hpp>

#include <ff/parallel_for.hpp>
#include "utimer.hpp"
using namespace std;
using namespace ff;
//pipeline farm
typedef pair<char,double> pai;


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
    string myString;
    ifstream myfile;
    string temp; //size of the string
    string result;
    string Filename;
     unsigned bufs=0, bits=0;
    
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
    long usec;
    {
       utimer t0("parallel computation",&usec);
        ParallelFor pfr(w);
        map<char,int> mpp;
        
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
       
        map <char,string>Huffcode;
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);

        ParallelFor pfr2(w);
        map<int,string> maps;
        pfr2.parallel_for_thid(0,myString.size(),1,0,[&maps,&Huffcode,&myString](const long idx,const int thid){
            maps[thid]=maps[thid]+ Huffcode[myString[idx]];
           
            
        });

        map<int,string>::iterator it2 = maps.begin();
        while (it2 != maps.end())
        {
            //std::cout << "Inizio: " << it->first.first  <<"Fine: " << it->first.second << std::endl;
            result= result+it2->second;
            ++it2;
        }
    };

    cout << "End (spent " << usec << " usecs using " << w << " threads)"  << endl; 
    cout << result;
    //cout  << usec << "," << w << endl;    

    /* for(char a: result)
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
    //cout << result << endl;*/
   
     
       
}
