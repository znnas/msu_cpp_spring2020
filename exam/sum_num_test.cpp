#include "sum_num.h"

int main(){
	string input_file="input.txt";
	
	vector<string> str_of_num;
	size_t count_string = 0;
	vector<int> sums;
	size_t cur_num = 0;
	bool end;
	
	//sum_object test1();
	
	mutex guard_cur_num;
	
	thread t1(reader, ref(input_file), ref(str_of_num), ref(count_string), ref(sums), ref(end));
	thread t2(counter, ref(str_of_num), ref(count_string), ref(sums), ref(cur_num), ref(guard_cur_num), ref(end));
	thread t3(counter, ref(str_of_num), ref(count_string), ref(sums), ref(cur_num), ref(guard_cur_num), ref(end));
	t1.join();
	t2.join();
	t3.join();
	
	//get_num.close(); 
	
	for(int i=0; i<sums.size();i++){
		cout << "sum in string " << i << ": " << sums[i] << endl;
	}

	cout << "result: " << my_sum(sums) << endl;
	
	return 0;
}