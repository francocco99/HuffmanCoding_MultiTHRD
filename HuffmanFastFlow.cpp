#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
using namespace std;
using namespace ff;
struct Frequency : ff_node_t <string,map<char,double>>
{
    map<char, double> *svc(string &inputString)
    {
        map<char, double> *freqmap = new map<char, double>();
        for (char c : inputString)
        {
            (*freqmap)[c]=(*freqmap)[c]+1/inputString.size();
        }
        return freqmap;
    }
};
int main()
{

}