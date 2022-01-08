#include<bits/stdc++.h>
#include "turing_machine.h"

using namespace std;

string help_output = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
string illegal = "illegal input\n";

int main(int argc, char** argv)
{
    bool verbose = false;
    string tm_file = "";
    string input_str = "";

    for(int i = 1; i < argc; ++i)
    {
        string s = argv[i];
        if(s == "-h" || s == "--help")
        {
            cout << help_output;
            return 0;
        }
        else if(s == "-v" || s == "--verbose") verbose = true;
        else if(tm_file == "") tm_file = s;
        else if(input_str == "") input_str = s;
        else
        {
            cerr << illegal;
            exit(0);
        }
    }
    
    TuringMachine tm(tm_file, verbose, input_str);

    return 0;
}