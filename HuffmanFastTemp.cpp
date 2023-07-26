#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <string.h>
#include <ff/parallel_for.hpp>
#include <ff/ff.hpp>
#include "utimer.hpp"
#include "BuildHuffman.hpp"
using namespace std;
using namespace ff;
//pipeline farm



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
   ParallelFor pf(w);
    vector<string> maps(w); // vector of encoded chunks of the input file
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
        for (int j= 0;j<256;j++)
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
    string temp; //size of the string
    string result;
    string myString;
    string Filename;

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
    
   
    
    stringstream buf;
    buf << t.rdbuf();
    myString=buf.str();
    
    long freq;
    long buildtemp;
    long encode;
    
     
    {
        utimer t0("parallel computation",&freq);
        ComputeFrequency(ref(mpp),myString);
    }
    {
        utimer t0("parallel computation",&buildtemp);
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }
    {
        utimer t0("parallel computation",&encode);
        result=Encode(Huffcode,myString);
    }
    cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;

    WriteFile(result);
  
   
     
       
}
