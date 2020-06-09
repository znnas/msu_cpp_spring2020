#pragma once
#include <cstdio>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

using namespace std;

//void nothing(vector<string>& token);
void nothing(string& token);

int to_int(string& digits);
//vector<int> to_int(vector<string>& digits);

class Timer
{
public:
    void start()
    {
        m_StartTime = chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop()
    {
        m_EndTime = chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds()
    {
        chrono::time_point<chrono::system_clock> endTime;

        if(m_bRunning)
        {
            endTime = chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return chrono::duration_cast<chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    chrono::time_point<chrono::system_clock> m_StartTime;
    chrono::time_point<chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};
//copied from https://gist.github.com/mcleary/b0bf4fa88830ff7c882d

void parser(string& test,
			vector<int>& res_digits,
			vector<string>& res_letters,
			void (*starter)(Timer& ),
			void (*ender)(Timer& 	),
			void (*do_letter)(string& ),
			int (*do_digits)(int ),
			void (*preproc)(string& )=nothing);
