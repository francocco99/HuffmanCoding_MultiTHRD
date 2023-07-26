#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <sstream>
#include <ff/ff.hpp>

#include <ff/parallel_for.hpp>

#include "utimer.hpp"
#include "BuildHuffman.hpp"
using namespace std;
using namespace ff;



int delta,len;
int w;




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
