#include <vector>
#include <string>

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
struct nodeTree* BuildHuffman(vector<int> mpp);
void WriteFile(string result);

