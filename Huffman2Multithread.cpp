#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
#include <algorithm>
#include <execution>
#include <thread>
using namespace std;
typedef pair<char,double> pai;
int main(int argc, char * argv[])
{
    
    ifstream myfile;
    string myString;
    string temp;
    

    myfile.open("text2.txt");
    if(myfile.is_open()) {
        while (myfile)
        {
            myfile >>myString;
        }
    }
    //vector<pair<char,double>> Mapout(myString.size());ù
    map<char,double> mpp;
    
    transform( //execution::par,
        myString.begin(),
        myString.end(),
        mpp.begin(),
        [](char w){ mpp.first=w;};
    );
    for(auto&e : mpp)
    {
       /* auto key=e.first;
        auto value=e.second;
        if(res.contains(key))*/
        cout << e.first << " " << e.second << " " <<endl;
    }
    //take the number of occurences for each char
     
       
}