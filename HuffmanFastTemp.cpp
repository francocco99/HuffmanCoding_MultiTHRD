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




int delta,len;
int w;

//traverse the tree for create the encoding of each char
void saveEncode(nodeTree* node,string str, unordered_map<char,string>&Huffcode)
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


//encode the tesxt using the right code for each letter
string Encode(unordered_map<char,string>Huffcode,string myString)
{
  ParallelFor pf(w);
    vector<string> Codes(w,""); // vector of encoded chunks of the input file
    string result;
     pf.parallel_for_idx(0,myString.size(),1,0,[&Huffcode,&myString,&Codes](const long first,const long last,const int thid){
        string buffer;

        for (long i =first; i < last; i++)
            buffer += Huffcode[myString[i]];

        Codes[thid] = buffer;
    });
    for (auto s:Codes)
    {
        result+= s;
    };
    return result;
    
}

//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mpp,string myString)
{
    
    ParallelFor pfr(w,true);
    vector<unordered_map<char,int>> listmps(w);
    
    pfr.parallel_for_thid(0,myString.size(),1,0,[&listmps,&myString](const int idx,const int thid){

        listmps[thid][myString[idx]]++;
    });
    long usecOver;
    {
        utimer t0("parallel computation",&usecOver);
        for (int i = 0; i < w; i++)
        {
            for (auto j: listmps[i])
            {
                mpp[j.first] += j.second;
            }
        }
    }
    cout  << usecOver << " -----> OverheadTemp " << w << endl;
}




int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    string result;
    string myString;
    string Filename;

    unordered_map<char,int> mpp;
    unordered_map<char,string>Huffcode;
    
    
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

    /*cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;*/
    
    cout << "F," << freq  << endl;
    cout << "b," << buildtemp <<  endl;
    cout << "e," << encode  << endl;


    WriteFile(result);
  
   
     
       
}
