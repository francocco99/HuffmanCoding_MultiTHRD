#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <thread>
#include "utimer.hpp"
#include "BuildHuffman.hpp"

using namespace std;

string myString; // input string
int delta,len; // chunk size for parallel computation, len of the string
int w; // number of workers

//vlues for compute the time
long OverC;
long OverE;

//function  execute by each thread for compute the frequency
void BodyParallel(int p,  vector<unordered_map<char,int>> &listmps)
{   
    int first,last;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
    for(int i=first;i<last;i++)
    {
       
       listmps[p][myString[i]]++;
    }
}

//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mpp)
{
    vector<thread*> Threads; //vector of thread
    vector<unordered_map<char,int>> listmps(w); 
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
   
    // overhead, Reconstructing a single ordered map
    {
        utimer t0("parallel computation",&OverC);
        for (int i = 0; i < w; i++)
        {
            for (auto j: listmps[i])
            {
                mpp[j.first] += j.second;
            }
        }
    }
}

// Functions  execute by each thread for parallel transform the string in binary values 
void paralEncode(int p,vector<string>  &cds,unordered_map<char,string>Huffcode)
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

// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode)
{
    vector<thread*> Threads;
    vector<string> Codes (w);
    string result;
    
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(paralEncode,i, ref(Codes),Huffcode));
       
    }
    for(auto t: Threads)
    {
        t->join();
    }
    // Overhead for create a single result
    {
        utimer t0("parallel computation",&OverE);
        for( string s: Codes)
        {
            result+= s;
        }
    }
    
    
    return result;
}  

// create a byte from a string of eight chars
char CreateByte(string result)
{
    
    unsigned bufs=0;
    for(char a: result)
    {
        bufs=(bufs<<1) | (atoi(&a)) ; 
    }
    return static_cast<char>(bufs);
     
}
//function execute by each thread
void EncodeinAscii(string newstring,int p,vector<string> &ResutlAscii)
{
    int first,last;
    string output;
    first=delta*p;
    if(p==w-1)
        last=len;
    else
        last=(p+1)*delta;
    
    
    for(int i=first;i<last;i+=8)
    {

        output+=CreateByte(newstring.substr(i,8));
    }
    ResutlAscii[p]=output;
}
//function to transform the encoded string to write it to a file
string AsciiTransform(string newstring)
{
    vector<thread*> Threads;
    vector<string> ResultAscii(w);
    string result;
    int bits;
    int size = newstring.size();
    bits = size % 8;
    if(bits!=0)
    {
        bits = 8 - bits;
    }
    newstring.append(bits, '0');
    len=newstring.size();
    delta=len/w;
    bits=delta%8;
    if(bits!=0)
    {
        bits=8-bits;
        delta+=bits; 
    }
    cout << delta%8<<endl;
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(EncodeinAscii,newstring,i, ref(ResultAscii)));
       
    }
    for(auto t: Threads)
    {
        t->join();
    }
    for( string s: ResultAscii)
    {
        result+= s;
    }
    return result;
}

int main(int argc, char * argv[])
{
    
    string Filename;
    string result;
    string line;
    
    ofstream outFile("textOut.bin",ios::out | ios::binary);
    
    unordered_map<char,int> mpp; // A map for each character with its frequency
    unordered_map<char,string>Huffcode; // map for each character the bit string 
    
    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return(0);
    }
    if(argc<3)
    {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return 0;
    }

    w=atoi(argv[2]); // take the number of workers
    Filename=argv[1];  // take the name of the file
    ifstream inputFile(Filename); // open the inputfilestream

    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    // take the input string
    while (getline(inputFile, line))
    {
        myString += line;
    }
   
    long usecRead;
    {
        utimer t0("parallel computation",&usecRead);
        while (getline(inputFile, line))
        {
            myString += line;
        }
    }
    cout <<"r, " << usecRead << endl;
    //cout << "End spent for Read the  file " << usecRead << " usecs" << endl;
    
   
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
    /*cout << "End spent for  Overhead in Frequency " << usecoverh1 << " usecs" << endl;
    cout << "End spent for  Overhead in encoding " << usecoverh2 << " usecs" << endl;
    cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;    */
    
    cout << "F," << freq  << endl;
    cout << "b," << buildtemp <<  endl;
    cout << "e," << encode  << endl;
    cout << "oc, " << OverC <<endl;
    cout << "oe, " << OverE <<endl;

    // write in the file
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        string Asciiresult=AsciiTransform(result);
        if (outFile.is_open()) 
        {
            outFile.write(Asciiresult.c_str(), Asciiresult.size());
            outFile.close();  // Close the file
        }
    }
    cout <<"w, " << usecWrite << endl;
    //cout << "End spent for Write the encoded file " << usecWrite << " usecs" << endl;
     
       
}