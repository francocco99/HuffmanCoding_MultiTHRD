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
    int len= mpp.size();
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

string Encode(vector<string>Huffcode,string myString)
{
    /*ParallelFor pf(w);
    vector<string> maps(w); 
    string result;
     pf.parallel_for_idx(0,myString.size(),1,0,[&Huffcode,&myString,&maps](const long first,const long last,const int thid){
        if (first == last)
                return;
            string temp_buffer;
            for (long i =first; i < last; i++)
                temp_buffer += Huffcode[myString[i]];

            maps[thid] = temp_buffer;
    });  
    for (auto s : maps)
    {
        result += s;
    };
    return result;*/
    string result="";
    ParallelForReduce<string> pfr2(w);
    pfr2.parallel_reduce(result,"",0,myString.size(),1,0,[&](const long i, string &myres){
    //cout <<"A[i]: "<<A[i] <<endl;
        myres+=Huffcode[myString[i]];
    },
    [](string &s,const string e){s+=e;}
    );
    return result;
    
}
void ComputeFrequency(vector<int> &mpp,string myString)
{
    ParallelFor pfr(w);
    vector<int> listmps[w];
    
    
    pfr.parallel_for_idx(0,myString.size(),1,0,[&listmps,&myString](const long first,const long last,const int thid){
        if (first == last)
                return;

            listmps[thid] = vector<int>(256, 0);

            for (long i = first; i < last; ++i)
            {
                listmps[thid][myString[i]]++;
            }
    });
    for (int i=0; i<w;i++)
    {
        for (int j = 0;j <256;j++)
        {
            mpp[j] += listmps[i][j];
        }
    }
}

//function for compute the frequency in parallel


// Functions for parallel transform the string in binary values 
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
    string temp; 
    string result="";
    string Filename;
    string myString;
    
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
        ComputeFrequency(ref(mpp),myString);
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        result=Encode(Huffcode,myString);
    };

    cout  << usec << "," << w << endl;    
    WriteFile(result);
       
     
       
}
