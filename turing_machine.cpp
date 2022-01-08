#include<bits/stdc++.h>
#include "turing_machine.h"

using namespace std;

string err_banner = "==================== ERR ====================\n";
string end_banner = "==================== END ====================\n";
string step_banner = "---------------------------------------------\n";
string run_banner = "==================== RUN ====================\n";

#define MAX_LINE_LENGTH 100

void TuringMachine::error(int i)
{
    cout << "syntax error\n";
    //fprintf(stderr, "syntax error\n");
    exit(i);
}

void TuringMachine::split_str(string l, set<string>& values)
{
    string s = "";
    for(int i = 0; i < l.length(); ++i)
    {
        if(l[i] == ',')
        {
            values.insert(s);
            s = "";
        }
        else s += l[i];
    }
    values.insert(s);
}

bool TuringMachine::split_char(string l, set<char>& values, string fault)
{
    l += ",";
    for(int i = 0; i < l.length(); i += 2)
    {
        if(l[i + 1] == ',') values.insert(l[i]);
        else return false;
    }
    return true;
}

void TuringMachine::check_states_symbols()
{
    //check input symbols in complete set of tape symbols
    for (const char &is : _input_symbols)
    {
        string str(1, is);
        if(_tape_symbols.find(is) == _tape_symbols.end())
        {
            if(_verbose) cout << "1: input symbol \'" + str + "\' is not in the set of tape symbols\n";
            error(1);
        }
    }

    //check start state belong to states
    if(_states.find(_start_state) == _states.end())
    {
        if(_verbose) cout << "start state " + _start_state + " is not in all states\n";
        error(0);
    }

    //check blank symbol in all symbols
    if(_tape_symbols.find(_blank) == _tape_symbols.end())
    {
        string b(1, _blank);
        if(_verbose) cout << "blank state \'" + b + "\' is not in all states\n";
        error(2);
    }

    //check final states all in all states
    for (const string &fs : _final_states)
    {
        if(_states.find(fs) == _states.end())
        {
            if(_verbose) cout << "final state \'" + fs + "\' is not in the set of states\n";
            error(3);
        }
    }
}

void TuringMachine::definition_without_trans(string l)
{
    l.erase(remove(l.begin(), l.end(), ' '), l.end());
    
    string tmp3 = l.substr(0, 3);
    string tmp4 = l.substr(0, 4);
    if(tmp3 == "#B=") _blank = l[3];
    else if(tmp3 == "#N=") _tape_num = stoi(l.substr(3, l.length() - 3));
    else if(tmp4 == "#Q={")
    {
        string vq = l.substr(4, l.length() - 4);
        vq = vq.substr(0, vq.length() - 2);
        split_str(vq, _states);
    }
    else if(tmp4 == "#S={")
    {
        string vs = l.substr(4, l.length() - 4);
        vs = vs.substr(0, vs.length() - 2);
        bool flags = split_char(vs, _input_symbols, "the finite set of input symbols in ");
        if(!flags)
        {
            if(_verbose) cout << "invalid final states input: " + l << endl;
            error(4);
        }
    }
    else if(tmp4 == "#G={")
    {
        string vg = l.substr(4, l.length() - 4);
        vg = vg.substr(0, vg.length() - 2);
        bool flagg = split_char(vg, _tape_symbols, "the complete set of tape symbols in ");
        if(!flagg)
        {
            if(_verbose) cout << "invalid tape symbols input: " + l << endl;
            error(5);
        }
    }
    else if(tmp4 == "#F={")
    {
        string vf = l.substr(4, l.length() - 4);
        vf = vf.substr(0, vf.length() - 2);
        split_str(vf, _final_states);
    }
    else if(tmp4 == "#q0=")
    {
        string vq0 = l.substr(4, l.length() - 4);
        vq0 = vq0.substr(0, vq0.length() - 1);
        _start_state = vq0;
    }
    else
    {
        if(_verbose) cout << "invalid input: " + l << endl;
        error(6);
    }
}

bool TuringMachine::check_if_in_set(string str, set<string> set)
{
    for(auto i: set)
    {
        if(str == i) return true;
    }
    return false;
}

bool TuringMachine::check_if_in_set(char str, set<char> set)
{
    if(str == '_' || str == '*') return true;
    for(auto i: set)
    {
        if(str == i) return true;
    }
    return false;
}

void TuringMachine::add_one_transition_func(string l)
{
    stringstream ss(l);
    string str[5];

    int i = 0;
    string tmp;
    while(ss >> tmp)
    {
        if(i >= 5)
        {
            if(_verbose) cout << "too much input for a transtion function: " + l << endl;
            error(7);
        }
        
        str[i] = tmp;
        ++i;
    }

    if(i != 5)
    {
        if(_verbose) cout << "too much input for a transtion function: " + l << endl;
        error(7);
    }

    if(!check_if_in_set(str[0], _states))
    {
        if(_verbose) cout << "in transition function \'" + l + 
        "\', there is no start state \'" + str[0] + "\' in states\n";
        error(8);
    }
        
    if(!check_if_in_set(str[4], _states))
    {
        if(_verbose) cout << "in transition function \'" + l + 
        "\', there is no final state \'" + str[4] + "\' in states\n";
        error(9);
    }

    for(int i = 0; i < str[1].length(); ++i)
    {
        if(!check_if_in_set(str[1][i], _input_symbols))
        {
            if(_verbose) cout << "in transition function \'" + l + 
            "\', there is no symbol \'" + str[1][i] + "\' in input symbols\n";
            error(10);
        }
    }
    
    for(int i = 0; i < str[2].length(); ++i)
    {
        if(!check_if_in_set(str[2][i], _tape_symbols))
        {
            if(_verbose) cout << "in transition function \'" + l + 
            "\', there is no symbol \'" + str[2][i] + "\' in tape symbols\n";
            error(11);
        }
    }

    for(int i = 0; i < str[3].length(); ++i)
    {
        if(str[3][i] != 'l' && str[3][i] != 'r' && str[3][i] != '*')
        {
            if(_verbose) cout << "in transition function \'" + l +
            "there is no direction \'" + str[3][i] + "\' in all directions\n";
            error(12);
        }
    }

    trans tr = {str[0], str[1], str[3], str[4], str[2]};
    _transitions.push_back(tr);
}

void TuringMachine::read_file()
{
    ifstream f(_filename);
    if(!f.is_open())
    {
        cout << "file not exists.\n";
        exit(0);
    }
    else
    {
        string l;
        int i = 1;
        while(getline(f, l))
        {
            //cout << i << ' ' << l << endl;
            ++i;
            if(l.length() == 1 || l[0] == ';') continue;

            //there is definition and annotation in the same line
            size_t pos = l.find(";");
            if(pos != l.npos) l = l.substr(0, pos);

            if(l[0] == '#') definition_without_trans(l);
            else add_one_transition_func(l);
        }
        f.close();
    }
}

void TuringMachine::check_input()
{
    for(int i = 0; i < _input.length(); ++i)
    {
        if(_input_symbols.find(_input[i]) == _input_symbols.end())
        {
            if(!_verbose)
            {
                cout << "illegal input\n";
                exit(13);
            }

            cout << "input: " << _input << endl;
            cout << err_banner;

            cout << "error: \'" << _input[i] << "\' was not declared in the set of input symbols\n";
            cout << "input: " << _input << endl;
            for(int j = 0; j < 7 + i; ++j) cout << " ";
            cout << "^\n";

            cout << end_banner;
            exit(0);
        }
    }
}

bool TuringMachine::same_ch(string s, string l)
{
    int len = s.length();
    for(int i = 0; i < len; ++i)
    {
        if(s[i] == l[i]) ;
        else if(s[i] == '*') ;
        else return false;
    }
    return true;
}

void TuringMachine::tape_process()
{
    check_input();

    int n = _tape_num;
    string* tapes = new string[n];
    int* zero_index = new int[n];
    int* heads = new int[n];

    tapes[0] = _input;
    for(int i = 1; i < n; ++i) tapes[i] = _blank;
    for(int i = 0; i < n; ++i) zero_index[i] = heads[i] = 0;

    /*cout << "tape num = " << _tape_num << endl;
    for(int i = 0; i < n; ++i)
    {
        cout << "left = " << left[i] << ", right = " << right[i] << endl;
        for(int j = left[i]; j <= right[i]; ++j)
            cout << tapes[i][j] << ' ';
        cout << endl;
    }*/

    int step = 0;
    bool flag = true;

    if(_verbose)
    {    
        cout << "Input: " << _input << endl;
        cout << run_banner;
    }

    while(true)
    {
        flag = true;
        string s = "";
        for(int i = 0; i < n; ++i) s += tapes[i][heads[i]];
        
        if(_verbose)
        {
            if(step > 0) cout << step_banner;

            cout << "Step    : " << step << endl;

            for(int i = 0; i < _tape_num; ++i)
            {
                cout << "Index" << i << "  : ";
                for(int j = -zero_index[i]; j < -zero_index[i] + (int)tapes[i].length(); ++j)
                    cout << abs(j) << ' ';
                cout << endl;
                
                cout << "Tape" << i << "   : ";
                for(int j = 0; j < tapes[i].length(); ++j)
                    cout << tapes[i][j] << ' ';
                cout << endl;

                cout << "Head" << i << "   : ";
                for(int j = 0; j < heads[i]; ++j) cout << "  ";
                cout << '^' << endl;
            }

            cout << "State   : " << _state << endl;
        }

        for(auto t: _transitions)
        {
            if(t.state == _state && same_ch(t.now_ch, s))
            {
                flag = false;
                _state = t.next_state;
                for(int i = 0; i < t.direction.length(); ++i)
                {
                    char c = t.new_ch[i];
                    if(c != '*') tapes[i][heads[i]] = c;
                    
                    char d = t.direction[i];
                    if(d == 'l')
                    {
                        if(heads[i] > 0) heads[i]--;
                        else
                        {
                            tapes[i] = "_" + tapes[i];
                            zero_index[i]++;
                        }
                    }
                    else if(d == 'r')
                    {
                        heads[i]++;
                        if(heads[i] >= tapes[i].length() - 1)
                            tapes[i] += "_";
                    }
                }
                break;
            }
        }

        //delete redundant blank
        for(int i = 0; i < _tape_num; ++i)
        {
            int r = heads[i];
            for(int j = tapes[i].length() - 1; j > r; --j)
            {
                if(tapes[i][j] == '_')
                    tapes[i] = tapes[i].substr(0, tapes[i].length() - 1);
                else break;
            }

            int l = min((int)tapes[i].length(), heads[i]);
            for(int j = 0; j < l; ++j)
            {
                if(tapes[i][j] == '_')
                {
                    tapes[i] = tapes[i].substr(1, tapes[i].length() - 1);
                    zero_index[i]--;
                    heads[i]--;
                }
                else break;
            }
        }

        if(flag) break;
        step++;
    }
}

void TuringMachine::check_tm()
{
    cout << "states: ";
    for(auto i: _states) cout << i << " ";
    cout << endl;

    cout << "input sym: ";
    for(auto i: _input_symbols) cout << i << " ";
    cout << endl;

    cout << "tape sym: ";
    for(auto i: _tape_symbols) cout << i << " ";
    cout << endl;

    cout << "start state: " << _start_state << endl;

    cout << "blank: " << _blank << endl;

    cout << "final states: ";
    for(auto i: _final_states) cout << i << " ";
    cout << endl;

    cout << "tape num: " << _tape_num << endl;
    
    cout << "now state: " << _state << endl;

    cout << "transitions:\n";
    for(auto i: _transitions)
        cout << i.state << ' ' << i.now_ch << ' '<< i.direction
        << ' ' << i.next_state << ' ' << i.new_ch << endl;
}

TuringMachine::TuringMachine(string filename, bool verbose, string input)
{
    _filename = filename;
    _verbose = verbose;

    read_file();

    //check_tm();

    check_states_symbols();

    _input = input;
    if(_input == "") _input = _blank;

    _state = _start_state;

    bool f = false;
    
    tape_process();

    for(auto s: _final_states)
    {
        if(s == _state)
        {
            f = true;
            break;
        }
    }

    if(!_verbose)
    {
        if(f) cout << "true\n";
        else cout << "false\n";
    }
    else
    {
        cout << step_banner << "Result: ";
        if(f) cout << "true\n";
        else cout << "false\n";
        cout << end_banner;
    }
}