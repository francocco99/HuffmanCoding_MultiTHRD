#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "BuildHuffman.hpp"
using namespace std;
using namespace ff;


string myString; // input string
int delta,len; // chunk size for parallel computation, len of the string
int w; // number of workers


// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode,string myString)
{
    ParallelFor pf(w);
    vector<string> Codes(w,""); // vector of encoded chunks of the input file
    string result;
     pf.parallel_for_idx(0,myString.size(),1,0,[&Huffcode,&myString,&Codes](const long first,const long last,const int thid){
        string temp; // using local variable reduce the time
        for (long i =first; i < last; i++)
        {
            temp+=Huffcode[myString[i]];
        }
        Codes[thid]=temp;
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
    ParallelForReduce<string> pfr(w);

    pfr.parallel_for_idx(0,newstring.size(),1,delta,[&](const long first,const long last,const int thid){
    string output;
        for(int i=first;i<last;i+=8)
        {
            output+=CreateByte(newstring.substr(i,8));
        }
        ResultAscii[thid]=output;
    });
    //reconstruct the string
    for( string s: ResultAscii)
    {
        result+= s;
    }
    return result;

}
 


int main(int argc, char * argv[])
{
    string result="";
    string Filename;
    string line;
    int mode;
    
    ofstream outFile("textOut.bin",ios::out | ios::binary);

    unordered_map<char,int> mpp;// A map for each character with its frequency
    unordered_map<char,string>Huffcode;// map for each character the bit string 
    
    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return(0);
    }
    if(argc<3)
    {
        cout << "Usage is: " << argv[0] << " fileName number_workers" << endl; 
        return 0;
    }
    
    mode=(argc<4 ? 0: atoi(argv[3]));
    w=atoi(argv[2]); //take number of workers
    Filename=argv[1]; // take the name of the file
    ifstream inputFile(Filename);// open the inputfilestream
    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    
    long usec;
    if(!mode)
    {
        while (getline(inputFile, line))
        {
            myString += line;
        }

        {     
            utimer t0("parallel computation",&usec); 
            ComputeFrequency(ref(mpp),myString);
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            result=Encode(Huffcode,myString);
        };
       
        string Asciiresult=AsciiTransform(result);
        outFile.write(Asciiresult.c_str(), Asciiresult.size());
        outFile.close(); 
        cout << "Time spend for computing the result: "<< usec <<" Using: " << w << " threads" <<endl;
    }
    else
    {
        {
            while (getline(inputFile, line))
            {
                myString += line;
            }
            utimer t0("parallel computation",&usec); 
            ComputeFrequency(ref(mpp),myString);
            nodeTree* Root=BuildHuffman(mpp);
            saveEncode(Root,"",Huffcode);
            result=Encode(Huffcode,myString);
            string Asciiresult=AsciiTransform(result);
            outFile.write(Asciiresult.c_str(), Asciiresult.size());
            outFile.close();
        };
        cout << "Time spend for computing the result with I/O Operation: "<< usec <<" Using: " << w << " threads" << endl;
    }  
    
    //print use for script
    //cout  << usec << "," << w << endl; 

}
