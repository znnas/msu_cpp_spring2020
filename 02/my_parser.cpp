#include "my_parser.h"

void nothing(string& token){

}

int to_int(string& digits){
	return stoi(digits);
}

void parser(string& test,
			vector<int>& res_digits,
			vector<string>& res_letters,
			void (*starter)(Timer&),
			void (*ender)(Timer&),
			void (*do_letter)(string& ),
			int (*do_digits)(int ),
			void (*preproc)(string& )){
	Timer t;
	starter(t);
	test=" "+test+" ";
	vector<int> token_start;
	vector<int> token_end;
	vector<string> token;
	bool cond_i;
	bool cond_plus;
	size_t len = test.length();
	int num_i;
	string str_i;

	for(int i=0;i<len-1;i++){
	   cond_i = !isalnum(test[i]);
	   cond_plus = isalnum(test[i+1]);
	   if (cond_i && cond_plus) {
		   token_start.push_back(i+1);
	   }
	}
	for(int i=0;i<len-1;i++){
	   cond_plus = !isalnum(test[i+1]);
	   cond_i = isalnum(test[i]);
	   if (cond_i && cond_plus) {
		   token_end.push_back(i+1);
	   }
	}
	int n_token = token_start.size();
	cout << "Count of tokens:" << n_token << endl;
	for(int i=0;i<n_token;i++){
	   token.push_back(test.substr(token_start[i],token_end[i] - token_start[i]));
	   preproc(token[i]);
	}

	bool is_num;
	int len_token;
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
		   num_i = to_int(token[i]);
		   res_digits.push_back(do_digits(num_i));
	   }
	   else {
		   str_i = token[i];
		   do_letter(str_i);
		   res_letters.push_back(str_i);
	   }
	}
	ender(t);
}



