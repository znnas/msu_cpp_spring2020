#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

void get_args(vector<string>& vs)
{
	//когда нет аргументов -- ничего не делаем
}

template <typename T>
void get_args(vector<string>& vs, T&& arg)
{
    stringstream conv_stream;
    conv_stream << forward<T>(arg);
    if (!conv_stream) {
        throw runtime_error("argument error");
    }
    string s;
    conv_stream >> s;
    vs.push_back(s);
}

template <typename T, typename... Args>
void get_args(vector<string>& vs, T&& arg, Args&&... args)
{
    get_args(vs, forward<T>(arg));
    get_args(vs, forward<Args>(args)...);
}

string form_string(const string& s, vector<string>& vs)
{
    if (vs.size() == 0) return s;
    string r;
    int cur;

    char br_open = '{';
    char br_close = '}';
    bool opened = false;
    int pos_open = 0;
    int pos_close = 0;

    for (int i=0; i<s.size(); ++i) {
        char c = s[i];
        if (opened == false) {
            if (c == br_open) {
                pos_open = i;
                r += s.substr(pos_close, pos_open - pos_close);
                opened = true;
            }
            else if (c == br_close) {
                throw runtime_error("error: order of }");
            }
        }
        else { //opened == true
            if (c == br_close) {
                pos_close = i + 1;
                string tmp = s.substr(pos_open + 1, pos_close - pos_open);
                try {
                	cur = stoi(tmp);
                }
                catch (invalid_argument &e) {
                	//cout << e.what();
                	throw runtime_error("error: not integer in brackets");
                }
                if (cur < 0 || vs.size() <= cur) {
                    throw runtime_error("error: number out of arg-range");
                }
                r += vs[cur]; // insert operation
                opened = false;
            }
            else if (c == br_open) {
                throw runtime_error("error: order of {");
            }
        }
    }
    if (opened == true) {
        throw runtime_error("error: lost }");
    }
    return r;
}

template <typename... Args>
string format(const string& str_in, Args&&... args)
{
    vector<string> vs;
    //if (sizeof...(args)>0){ //why it doesn't work?
    get_args(vs, forward<Args>(args)...);
    return form_string(str_in, vs);
}
