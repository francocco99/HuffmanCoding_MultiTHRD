#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include "utimer.hpp"
#include "BuildHuffman.hpp"

using namespace std;
int len; // len of the string
string myString; // string tht contain the string to process

// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode)
{
    string result;
    for(int i=0;i<len;i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mp)
{
    for(int i=0;i<len;i++)
    {         
        mp[myString[i]]++;
    }
    
}




int main(int argc,char* argv[])
{
    string Filename;
    string result;
    string line;
    int mode;

    ofstream out("textOut.bin",ios::out | ios::binary);

    unordered_map<char,int> mpp; // A map for each character with its frequency
    unordered_map<char,string>Huffcode;// map for each character the bit string 
   
    long usecRead;

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return(0);
    }
    if(argc==1)
    {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return 0;
    }


    mode=(argc<2 ? 0: atoi(argv[2]));
    Filename=argv[1]; // take the name of the file
    ifstream inputFile(Filename);// open the inputfilestream
    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    // take the input string
    {
        utimer t0("parallel computation",&usecRead); 
        while (getline(inputFile, line))
        {
            myString += line;
        }
    }
   
    len=myString.size();
     long usecs; 
    if(!mode)
    {
        
        {
            utimer t0("parallel computation",&usecs); 
            ComputeFrequency(ref(mpp));  
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            for(auto a: Huffcode)
            {
                cout <<" letter " <<a.first << " coding " << a.second << endl;
            }
            result=Encode(Huffcode);        
        
        };
    
        WriteFile(result);   // write the encoded string in a file
    }
    else
    {
       
        {
            utimer t0("parallel computation",&usecs); 
            ComputeFrequency(ref(mpp));  
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            for(auto a: Huffcode)
            {
                cout <<" letter " <<a.first << " coding " << a.second << endl;
            }
            result=Encode(Huffcode);        
            WriteFile(result);   // write the encoded string in a file
        }
    }
    cout << usecs << ",1" << endl;
     
       
}