#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "BuildHuffman.hpp"
using namespace std;
using namespace ff;


string myString; // input string
int delta,len; // chunk size for parallel computation, len of the string
int w; // number of workers

long OverE;
long OverA;
long AsciiComp;
// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode,string myString)
{
    ParallelFor pf(w);
    vector<string> Codes(w,""); // vector of encoded chunks of the input file
    string result;  // using local variable reduce the time
     pf.parallel_for_idx(0,myString.size(),1,0,[&Huffcode,&myString,&Codes](const long first,const long last,const int thid){
        string temp;
        for (long i =first; i < last; i++)
        {
            temp+=Huffcode[myString[i]];
        }
        Codes[thid]=temp;
    });
    {
        utimer t0("parallel computation",&OverE);
        for (auto s:Codes)
        {
            result+= s;
        };
    }
    return result;
    
}
//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mpp,string myString)
{
    
    ParallelForReduce<unordered_map<char,int>> pfr2(w);
    unordered_map<char,int> Initial;
   
    pfr2.parallel_reduce(mpp,Initial,0,myString.size(),1,0,[&](const int idx,unordered_map<char,int> &res)
    {
        res[myString[idx]]++; 
    },
    [&](unordered_map<char,int>& total,unordered_map<char,int> partial ){
        for(auto elem: partial)
        {   
            total[elem.first]+=elem.second;
        }
        
    });  
}

// create a byte from a string of eight chars
char CreateByte(string result)
{
    unsigned bufs=0;
    for(char a: result)
    {
        bufs=(bufs<<1) | (atoi(&a));       
    }
    return static_cast<char>(bufs);
     
}

// "ASCII Encoding" of the string
string AsciiTransform(string newstring)
{
    string result="";
    int bits;
    int size = newstring.size();

    //padding of the string
    bits = size % 8;
    if(bits!=0)
    {
        bits = 8 - bits;
    }
    newstring.append(bits, '0');
    len=newstring.size();
    delta=len/w;
    bits=delta%8;
    //delta must be a multiple of eight
    if(bits!=0)
    {
        bits=8-bits;
        delta+=bits; 
    }
    vector<string> ResultAscii(w);
    ParallelFor pfr(w);

    pfr.parallel_for_idx(0,newstring.size(),1,delta,[&](const long first,const long last,const int thid){
    string output; // each thread work on local variable
        for(int i=first;i<last;i+=8)
        {
            output+=CreateByte(newstring.substr(i,8));
        }
        ResultAscii[thid]=output;
    });
    //reconstruct the string
    {
        utimer t0("parallel computation",&OverA);
        for( string s: ResultAscii)
        {
            result+= s;
        }
    }
    return result;

}
 

int main(int argc, char * argv[])
{
    string result="";
    string Asciiresult;
    string Filename;
    string line;
    long usecRead;

    nodeTree* Root;
    ofstream outFile("textOut.bin",ios::out | ios::binary);

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
    w=atoi(argv[2]); //take number of workers
    Filename=argv[1]; //number of workers
    ifstream inputFile(Filename); // open the inputfilestream
    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    {
        utimer t0("parallel computation",&usecRead);
        while (getline(inputFile, line))
        {
            myString += line;
        }
    }
    
    long freq;
    long buildtemp;
    long encode;
    long usecWrite;
     //compute the frequency
    {
        utimer t0("parallel computation",&freq);
        ComputeFrequency(ref(mpp),myString);
    }
    //build HuffmanTree
    {
        utimer t0("parallel computation",&buildtemp);
        Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }
    //compute the encoding
    {
        utimer t0("parallel computation",&encode);
        result=Encode(Huffcode,myString);
    }
    //compute the  ASCII encoding 
    {
        utimer t0("parallel computation",&AsciiComp);
        Asciiresult=AsciiTransform(result);
    }
    //Write the encoded file
    {
        utimer t0("parallel computation",&usecWrite);
        if (outFile.is_open()) 
        {
            outFile.write(Asciiresult.c_str(), Asciiresult.size());
            outFile.close();  
        }
    }

    cout << endl;
    cout << "Time spent reading the file: " << usecRead << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent to compute the Frequency: " << freq << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent to Build and Traverse the Huffman Tree: " << buildtemp << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent to Encode the file " << encode << " usecs" << endl;  
    cout << "Overhead in encoding: " << OverE << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent to compute ASCII encoding " << AsciiComp << " usecs" << endl;
    cout << "Overhead in ASCII encoding: " << OverA << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent writing the  file " << usecWrite << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << endl;
    cout << "TOTAL: " << usecRead+freq+buildtemp+encode+usecWrite+ AsciiComp <<" usecs" << endl;
    cout << endl;


    /*cout <<"r," << usecRead << endl;
        cout << "F," << freq  << endl;
        cout << "b," << buildtemp <<  endl;
        cout << "enc," << encode  << endl;
        cout << "oe," << OverE <<endl;
        cout << "As," << AsciiComp << endl;
        cout << "oa," << OverA << endl;
        cout <<"w," << usecWrite << endl;*/
    DisposeTree(Root);
 
}
