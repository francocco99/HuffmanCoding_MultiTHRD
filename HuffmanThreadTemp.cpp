#include <thread>
#include "BuildHuffman.hpp"
using namespace std;

string myString; // input string
int delta,len; // chunk size for parallel computation, len of the string
int w; // number of workers

//values for compute the time
long OverC;
long OverE;
long OverA;
long AsciiComp;

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
            delete(t);
        }
    {
        // overhead, Reconstructing a single ordered map
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
    // Overhead for create a single result
   
        for(auto t: Threads)
        {
            t->join();
            delete(t);
        } 
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
    //padding of the string
    bits = size % 8;
    if(bits!=0)
    {
        bits = 8 - bits;
    }
    newstring.append(bits, '0'); //append 0 at the end of the string
    len=newstring.size();
    delta=len/w;
    //delta must be a multiple of eight
    bits=delta%8;
    if(bits!=0)
    {
        bits=8-bits;
        delta+=bits; 
    }
    for(int i=0;i<w;i++)
    { 
        Threads.push_back(new thread(EncodeinAscii,newstring,i, ref(ResultAscii)));
    }
    
        for(auto t: Threads)
        {
            t->join();
            delete(t);
        }
    {
        //overhead reconstruct the result
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
    
    string Filename;
    string result;
    string Asciiresult;
    string line;
    
    nodeTree* Root;
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
    long usecRead;
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
    //compute the frequency
    {
        utimer t0("parallel computation",&freq); 
        ComputeFrequency(ref(mpp));
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
        result=Encode(Huffcode);
    }
    //compute the  ASCII encoding
    {
        utimer t0("parallel computation",&AsciiComp);
        Asciiresult=AsciiTransform(result);
    }
    //Write the encoded file
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        outFile.write(Asciiresult.c_str(), Asciiresult.size());
        outFile.close();  // Close the file
        
    }
    
    cout << endl;
    cout << "Time spent reading the file: " << usecRead << " usecs" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Time spent to compute the Frequency: " << freq << " usecs" << endl;
    cout << "Overhead in Frequency " << OverC << " usecs" << endl;
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
    cout << "TOTAL: " << usecRead+freq+buildtemp+encode+usecWrite+AsciiComp << " usecs" << endl;
    cout << endl;



    DisposeTree(Root);

    //print use for script
    /*
     cout <<"r," << usecRead << endl;
    cout << "F," << freq  << endl;
    cout << "b," << buildtemp <<  endl;
    cout << "enc," << encode  << endl;
    cout << "oc," << OverC <<endl;
    cout << "oe," << OverE <<endl;
    cout << "As," << AsciiComp << endl;
    cout << "oa," << OverA << endl;
    cout <<"w," << usecWrite << endl;*/
   
     
       
}