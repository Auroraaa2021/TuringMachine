#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

#include<bits/stdc++.h>
using namespace std;

struct trans
{
    string state;
    string now_ch;
    string direction;
    string next_state;
    string new_ch;
};

class TuringMachine
{
private:
    string _filename;
    bool _verbose;

    set<string> _states;
    set<char> _input_symbols;
    set<char> _tape_symbols;
    string _start_state;
    char _blank;
    set<string> _final_states;
    
    int _tape_num;
    vector<trans> _transitions;

    string _input;
    string _state;

public:
    TuringMachine(string filename, bool verbose, string input);
    void split_str(string l, set<string>& values);
    bool split_char(string l, set<char>& values, string fault);
    void check_states_symbols();
    void definition_without_trans(string l);
    void add_one_transition_func(string l);
    void read_file();
    void check_input();
    void tape_process();
    bool same_ch(string s, string l);
    void check_tm();
    void error(int i);
    bool check_if_in_set(string str, set<string> set);
    bool check_if_in_set(char str, set<char> set);
};

#endif