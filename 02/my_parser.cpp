#include "my_parser.h"
#include <cstdio>
#include <string>
#include <vector>
using namespace std;

void nothing(vector<string>& token){

}

vector<int> to_int(vector<string>& digits){
	vector<int> res;
	int n = digits.size();
	for(int i=0;i<n;i++){
		res.push_back(stoi(digits[i]));
	}
	return res;
}

void parser(string& test, vector<int>& res_digits, vector<string>& res_letters,
		    void (*do_letter)(vector<string>&), //ставил auto, все работает, но ругается. Пусть знаем тип.
			void (*do_digits)(vector<int>&),             //здесь тоже
			void (*preproc)(vector<string>&)){
	   test=" "+test+" ";
	   vector<int> token_start;
	   vector<int> token_end;
	   vector<string> token;
	   bool cond_i;
	   bool cond_plus;
	   size_t len = test.length();

	   for(int i=0;i<len-1;i++){
		   //isalnum
		   cond_i = !isalnum(test[i]);//((test[i]==' ')||(test[i]=='\t')||(test[i]=='\n'));
		   cond_plus = isalnum(test[i]);//((test[i+1]!=' ')&&(test[i+1]!='\t')&&(test[i+1]!='\n'));
		   if (cond_i && cond_plus) {
			   token_start.push_back(i+1);
		   }
	   }
	   for(int i=0;i<len-1;i++){
		   cond_plus = !isalnum(test[i+1]);//((test[i+1]==' ')||(test[i+1]=='\t')||(test[i+1]=='\n'));
		   cond_i = isalnum(test[i]);//((test[i]!=' ')&&(test[i]!='\t')&&(test[i]!='\n'));
		   if (cond_i && cond_plus) {
			   token_end.push_back(i+1);
		   }
	   }
	   int n_token = token_start.size();
	   for(int i=0;i<n_token;i++){
		   token.push_back(test.substr(token_start[i],token_end[i] - token_start[i]));
		   //cout << token[i]<< endl;
	   }

	   preproc(token);
	   n_token = token.size();

	   bool is_num = true;
	   int len_token;
	   vector<string> digits;
	   vector<string> letters;
	   for(int i=0;i<n_token;i++){
		   len_token=token[i].size();
		   is_num = true;
		   for(int j=0;j<len_token;j++){
			   if ((token[i][j]<'0')||(token[i][j]>'9')){
				   is_num = false;
				   break;
			   }
		   }
		   if (is_num == true){
			   digits.push_back(token[i]);
		   } else letters.push_back(token[i]);
	   }
//	   int n_dig=digits.size();
//	   int n_let=letters.size();

	   //into?
	   //vector<int> new_digits = to_int(digits);
	   res_digits = to_int(digits);
	   do_letter(letters);
	   
	   do_digits(res_digits);
	   res_letters = letters;
}



