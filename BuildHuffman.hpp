
#include <string>
#include <unordered_map>

using namespace std;

struct nodeTree
{
    char a;
    int freq;
    nodeTree *left, *right;
};
class Compare{
    public:
        bool operator()(nodeTree* below, nodeTree * above);
};
struct nodeTree* Newn(char data,int freq);
struct nodeTree* BuildHuffman(unordered_map<char,int> mpp);
void saveEncode(nodeTree* node,string str, unordered_map<char,string>&Huffcode);
void WriteFile(string result);

