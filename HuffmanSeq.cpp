#include <iostream>
#include <string.h>
#include <functional>
#include <fstream>
#include <algorithm>
#include "utimer.hpp"
#include "BuildHuffman.hpp"

using namespace std;
int len; // len of the string
string myString; // string that contain the string to process

// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode)
{
    string result;
    for(size_t i=0;i<myString.length();i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mp)
{
    for(size_t i=0;i<myString.length();i++)
    {         
        mp[myString[i]]++;
    }
    
}

int main(int argc,char* argv[])
{
    string Filename; // name of the file in input
    string result;  //result of the encoding
    string line; //used for read the file
    
    int mode; // 0 / 1 used to print

    ofstream out("textOut.bin",ios::out | ios::binary);

    unordered_map<char,int> mpp; // A map for each character with its frequency
    unordered_map<char,string>Huffcode;// map for each character the bit string 
   
  

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName" << " mode " <<endl; 
        return(0);
    }
    if(argc==1)
    {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return 0;
    }


    mode=(argc<3 ? 0: atoi(argv[2]));
    Filename=argv[1]; // take the name of the file
    ifstream inputFile(Filename);
    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    
    long usecs; 
    if(!mode)
    {
        // take the input string
        while (getline(inputFile, line))
        {
            myString+=line;
        }
        { 
            utimer t0("parallel computation",&usecs); 
            ComputeFrequency(ref(mpp));  
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            result=Encode(Huffcode);        
        
        };
        cout << "Time spend for computing the result: "<< usecs << endl;
        WriteFile(result);   // write the encoded string in a file
    }
    else
    {
        
        {
            while (getline(inputFile, line))
            {
                myString+=line;
            }
            
            utimer t0("parallel computation",&usecs); 
            ComputeFrequency(ref(mpp));  
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            result=Encode(Huffcode);        
            WriteFile(result);   // write the encoded string in a file
        }
        cout << "Time spend for computing the result with I/O Operation: "<< usecs << endl;
    }
    
    
    
    //cout << usecs << ",1" << endl;  print used for the script
     
       
}