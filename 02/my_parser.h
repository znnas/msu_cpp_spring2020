#pragma once
#include <cstdio>
#include <string>
#include <vector>
using namespace std;

void nothing(vector<string>& token);

vector<int> to_int(vector<string>& digits);


void parser(string& test, vector<int>& res_digits, vector<string>& res_letters,
		    void (*do_letter)(vector<string>&), //������ auto, ��� ��������, �� ��������. ����� ����� ���.
			void (*do_digits)(vector<int>&),             //����� ����
			void (*preproc)(vector<string>&)=nothing);
