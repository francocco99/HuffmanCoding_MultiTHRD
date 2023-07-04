#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
#include <algorithm>
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

    myfile.open("text2.txt");
    if(myfile.is_open()) {
        while (myfile)
        {
            myfile >>myString;
        }
    }
    //take the number of occurences for each char
    for(char&c : myString)
    {         
        mpp[c]=mpp[c]+1.0;
    }
    map<char, double>::iterator it = mpp.begin();
    len=myString.size();
  // Iterate through the map and print the elements
    while (it != mpp.end())
    {
        it->second= it->second/len;
        ++it;
    }
    it = mpp.begin();
    while (it != mpp.end())
    {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        ++it;
    }
    
    map <char,string>Huffcode;
    nodeTree* Root=BuildHuffman(mpp);
    saveEncode(Root,"",Huffcode);
    
    string result=" ";
    for(char&c : myString)
    { 
        result=result+Huffcode[c];
    }
    cout<< "Result of Encoding is: " << result << endl; 
       
}