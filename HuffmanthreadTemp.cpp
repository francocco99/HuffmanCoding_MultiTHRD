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
#include <string.h>
#include "utimer.hpp"
#include "BuildHuffman.hpp"
using namespace std;
typedef pair<char,int> pai;
mutex Lockmp;
string myString;
int delta,len;
int w;
long usecoverh1;
long usecoverh2;





void saveEncode(nodeTree* node,string str,  vector <string>&Huffcode)
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
void BodyParallel(int p,  vector<vector<int>> &listmps)
{   
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
   // cout << "first: " << first << " last: " << last << endl;
   listmps[p]=vector<int>(256,0);
    for(int i=first;i<last;i++)
    {
       
       listmps[p][myString[i]]++;
    }
}
void ComputeFrequency(vector<int> &mpp)
{
    vector<thread*> Threads; //vector of thread
    vector<vector<int>> listmps(w);
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
   
    {
        utimer to("Overhead computation",&usecoverh1);
        for (int i=0; i<w;i++)
        {
            for (int j = 0; j < 256; j++)
            {
                mpp[j] += listmps[i][j];
            }
        }
    }
     

}

// Functions for parallel transform the string in binary values 
void paralEncode(int p, vector<string>  &cds,vector <string>Huffcode)
{   
     
    int first,last;
    string output;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;

    for(int i=first;i<last;i++)
    {
        output+= Huffcode[myString[i]];
    }
     cds[p] = output;
}

string Encode(vector <string>Huffcode)
{
    vector<thread*> Threads;
    vector<string> Codes (w);
    string result;
    //cout << "non ci entro "  << w << endl;
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(paralEncode,i, ref(Codes),Huffcode));
    }
    for(auto t: Threads)
    {
        t->join();
    }
   
    
    {
        utimer t0("parallel computation",&usecoverh2);
        for( string s: Codes)
        {
            result+= s;
        }
    
    }

    return result;
}  

int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string temp; //size of the string
    string Filename;
    vector<int> mpp(256,0);
    vector <string>Huffcode(256,"");
    string result;
    
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
    long usecRead;
    {
        utimer t0("parallel computation",&usecRead);
        buf << t.rdbuf();
        myString=buf.str();
    }
    cout << "End spent for Read the  file " << usecRead << " usecs" << endl;
    
   
    long freq;
    long buildtemp;
    long encode;
    {
        utimer t0("parallel computation",&freq); 
        ComputeFrequency(ref(mpp));
    }
    {

        utimer t0("parallel computation",&buildtemp);
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }
    {
        utimer t0("parallel computation",&encode); 
        result=Encode(Huffcode);
    }
    cout << "End spent for  Overhead in Frequency " << usecoverh1 << " usecs" << endl;
    cout << "End spent for  Overhead in encoding " << usecoverh2 << " usecs" << endl;
    cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;    
    //cout<< "Result of Encoding is: " << result << endl;
    // write in the file
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        WriteFile(result);
    }
    cout << "End spent for Write the encoded file " << usecWrite << " usecs" << endl;
     
       
}