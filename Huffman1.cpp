#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <bitset>
#include "utimer.hpp"
using namespace std;
typedef pair<char,double> pai;
struct nodeTree
{
    char a;
    double freq;
    nodeTree *left, *right;
};

class Compare {
public:
    bool operator()(nodeTree * below, nodeTree * above)
    {
        
        return below->freq>above->freq;
        
        
    }    
};
//Aggiunto commento
struct nodeTree* Newn(char data,double freq)
{
    struct nodeTree* temp = (struct nodeTree*)malloc(sizeof(struct nodeTree));
    temp->left = temp->right=nullptr;
    temp->a = data;
    temp->freq = freq;
 
    return temp;    
}

struct nodeTree* BuildHuffman(map<char,double> mpp)
{
    nodeTree * left;
    nodeTree * right;
    nodeTree * center;
    priority_queue<nodeTree *,vector<nodeTree *>,Compare> pq;
    map<char, double>::iterator it = mpp.begin();
    while (it != mpp.end())
    {
        pq.push(Newn(it->first,it->second));
        ++it;
    }
    while (pq.size()!=1)
    {
        left=pq.top();pq.pop();
        right=pq.top();pq.pop();

        //creo il nuovo nodo interno
        center=Newn('#',left->freq+right->freq);
        center->left=left;
        center->right=right;
        pq.push(center);

    }
    return pq.top();
    



};

void PrintTree(nodeTree* tree)
{
    if(tree==nullptr){
        return; 
    }
    cout << tree->a <<endl;
    PrintTree(tree->left);
    PrintTree(tree->right);
}
void saveEncode(nodeTree* node,string str, map <char,string>&Huffcode)
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
int main()
{
    map<char,double> mpp;
    ifstream myfile;
    string myString;
    string temp;
    float len;
    vector<pai> vec;
    string result;
    ifstream t("text3.txt");
    ofstream out("textOut.bin",ios::out | ios::binary);
    ofstream out2("textOut2.txt");
    stringstream buf;
    buf << t.rdbuf();
    
    myString=buf.str();
  //  cout << myString << endl;
    //take the number of occurences for each char
      long usecs; 
    {
        utimer t0("parallel computation",&usecs); 
        for(char&c : myString)
        {         
            mpp[c]=mpp[c]+1.0;
        }
        map<char, double>::iterator it = mpp.begin();
        len=myString.size();
    // Iterate through the map and print the elements
        /*while (it != mpp.end())
        {
            it->second= it->second/len;
            ++it;
        }
        it = mpp.begin();
        while (it != mpp.end())
        {
            std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
            ++it;
        }*/
        
        map <char,string>Huffcode;
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
        
         result=" ";
        for(char&c : myString)
        { 
            result=result+Huffcode[c];
        }
    }
    cout << "End (spent " << usecs << " usecs" << endl;
    out2 << result;
   
    unsigned bufs=0, bits=0;
   
   
    
    
    
    // write in the file
    for(char a: result)
    {
        
       if(bits==8)
        {
           
            out.put(bufs);      
            bufs=0;
            bits=0;
        }
        else
        {
            bufs=(bufs<<1) | (atoi(&a) & 1);
            bits++;
        }
    }
    if(bits>=8)
    {
        bits-=8;
        out.put(bufs >>bits);
    }
    

    
       
}