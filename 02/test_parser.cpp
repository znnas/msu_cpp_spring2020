#include "my_parser.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm> //transform


//examples of user function for callback

//preprocessing: когда уже нашли токены, но ещЄ не знаем их тип 
void only_three(vector<string>& token){
	int n = token.size();
	for(int i=0;i<n;i++){
		if (token[i].length()>3) {
			token[i] = token[i].substr(0,3);
		}
	}
}

//манипул€ции с числами
void squared_vect(vector<int>& digits){
//	int n = digits.size();
	transform(digits.begin(), digits.end(), digits.begin(),
						[](int cur){ return cur*cur; });
}

//манипул€ции со словами
void to_upper(vector<string>& letters){
	int n = letters.size();
	for(int i=0;i<n;i++){
		transform(letters[i].begin(), letters[i].end(), letters[i].begin(),
				[](unsigned char c){ return std::toupper(c); });
	};
}


int main(){
	vector<int> res_digits;
	vector<string> res_letters;
	int n_dig;
	int n_let;


	//1st test
    string test1="aabbcc qqwwee \t 12 24 tutu \n blabla 01 kek   G";
    cout << "Test1. preproc: by default--nothing function, letters -- to_upper, int -- squared." << endl;
    cout << "Test1 text:"<< endl;
    cout << test1 << endl;
    cout << "(end of text)" << endl;
    cout << endl;
    
    parser(test1, res_digits, res_letters, to_upper, squared_vect);
    n_dig=res_digits.size();
    n_let=res_letters.size();

    for(int i=0;i<n_dig;i++){
	   cout << res_digits[i]<< " ";
	}
	cout << endl;
	for(int i=0;i<n_let;i++){
	   cout << res_letters[i]<< " ";
	}
	cout << endl;

	res_digits.erase(res_digits.begin(),res_digits.end());
	res_letters.erase(res_letters.begin(),res_letters.end());

	
	
	//2nd test (void letters)
    string test2=" \t 3 \n\n\n 22";
    cout << "Test2. preproc: by default--nothing function, letters -- to_upper, int -- squared. " << endl;
    cout << "Test2 text (have no letters):"<< endl;
    cout << test2 << endl;
    cout << "(end of text)" << endl;
    cout << endl;
    
    parser(test2, res_digits, res_letters, to_upper, squared_vect);
    n_dig=res_digits.size();
    n_let=res_letters.size();

    for(int i=0;i<n_dig;i++){
	   cout << res_digits[i]<< " ";
	}
	cout << endl;
	for(int i=0;i<n_let;i++){
	   cout << res_letters[i]<< " ";
	}
	cout << endl;

	res_digits.erase(res_digits.begin(),res_digits.end());
	res_letters.erase(res_letters.begin(),res_letters.end());

	
	//3rd test (void digits)
    string test3=" \t wEw \n\n\n";
    cout << "Test3. preproc: by default--nothing function, letters -- to_upper, int -- squared. " << endl;
    cout << "Test3 text (have no digits):"<< endl;
    cout << test3 << endl;
    cout << "(end of text)" << endl;
    cout << endl;
    
    parser(test3, res_digits, res_letters, to_upper, squared_vect);
    n_dig=res_digits.size();
    n_let=res_letters.size();

    for(int i=0;i<n_dig;i++){
	   cout << res_digits[i]<< " ";
	}
	cout << endl;
	for(int i=0;i<n_let;i++){
	   cout << res_letters[i]<< " ";
	}
	cout << endl;

	res_digits.erase(res_digits.begin(),res_digits.end());
	res_letters.erase(res_letters.begin(),res_letters.end());

	
	//4th test (void)
    string test4=" \t  \n\n\n ";
    cout << "Test4. preproc: by default--nothing function, letters -- to_upper, int -- squared. " << endl;
    cout << "Test4 text (only tabs and void strings):"<< endl;
    cout << test4 << endl;
    cout << "(end of text)" << endl;
    cout << endl;
    
    parser(test4, res_digits, res_letters, to_upper, squared_vect);
    n_dig=res_digits.size();
    n_let=res_letters.size();

    for(int i=0;i<n_dig;i++){
	   cout << res_digits[i]<< " ";
	}
	cout << endl;
	for(int i=0;i<n_let;i++){
	   cout << res_letters[i]<< " ";
	}
	cout << endl;

	res_digits.erase(res_digits.begin(),res_digits.end());
	res_letters.erase(res_letters.begin(),res_letters.end());

	//5th test (again not void) //preprocessing: оставл€ем не более трЄх первых символов от каждого tokena
    string test5=" \t wha t\n's hapqpiruc pensdfjlkj ingdkjk\n\n 1009999999999999999999 ";
    cout << "Test5. preproc: get argument--only_three function, letters -- to_upper, int -- squared. " << endl;
    cout << "Test5 text (use first three symbols of token):"<< endl;
    cout << test5 << endl;
    cout << "(end of text)" << endl;
    cout << endl;
    
    parser(test5, res_digits, res_letters, to_upper, squared_vect, only_three);
    n_dig=res_digits.size();
    n_let=res_letters.size();

    for(int i=0;i<n_dig;i++){
	   cout << res_digits[i]<< " ";
	}
	cout << endl;
	for(int i=0;i<n_let;i++){
	   cout << res_letters[i]<< " ";
	}
	cout << endl;

	res_digits.erase(res_digits.begin(),res_digits.end());
	res_letters.erase(res_letters.begin(),res_letters.end());

}