#include <iostream>
#include <string>
#include <string.h>
#include <functional>

#include <queue>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <execution>
#include <thread>
#include <mutex>
#include "utimer.hpp"
#include "BuildHuffman.hpp"
using namespace std;
int len;
string myString;



//traverse the tree for create the encoding of each char
void saveEncode(nodeTree* node,string str,vector <string>&Huffcode)
{
    if(node==nullptr)
        return;
    if (!node->left && !node->right) {
		Huffcode[node->a] = str;
	}
    saveEncode(node->left,str+"0",Huffcode);
    saveEncode(node->right,str+"1",Huffcode);
}

//encode the tesxt using the right code for each letter
string Encode(vector <string>Huffcode)
{
    string result;
    for(int i=0;i<len;i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

//compute frequency of the letter in the text
void ComputeFrequency(vector<int> &mp)
{
    for(int i=0;i<len;i++)
    {         
        mp[myString[i]]++;
    }
}

int main(int argc, char* argv[])
{
    vector<int> mpp(256,0);
    vector <string>Huffcode(256,"");
    ifstream myfile;
    
    string Filename;
    string temp;
    string result;
    ofstream out("textOut.bin",ios::out | ios::binary);
    stringstream buf;
    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return(0);
    }
    if(argc==1)
    {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return 0;
    }
    Filename=argv[1]; //number of workers
    ifstream t(Filename);
    if(t.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
   long usecRead;
    {
        utimer t0("parallel computation",&usecRead);
        buf << t.rdbuf();
        myString=buf.str();
    }
        //cout << "End spent for Read the  file " << usecRead << " usecs" << endl;
    cout << "r," << usecRead  << endl;
    len=myString.size();
 
      long  freq; 
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
    /*cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;*/


    cout << "F," << freq  << endl;
    cout << "b," << buildtemp <<  endl;
    cout << "e," << encode  << endl;

   



   
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        WriteFile(result);
    }
    cout << "w," << usecWrite  << endl;
   // cout << "End spent for Write the encoded file " << usecWrite << " usecs" << endl;
       
}