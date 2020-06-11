#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>


using namespace std;

int my_sum(vector<int>& ints){
	int sum = 0;
	for(int i=0; i<ints.size(); i++){
		sum+=ints[i];
	}
	return sum;
}

void reader(string& input_file, 
			vector<string>& str_of_num, 
			size_t& count_string,
			vector<int>& sums,
			bool& end){
	end=false;
	ifstream get_num;
	get_num.open(input_file);
	
	string cur_line;
	while (!get_num.eof()){
		getline(get_num, cur_line);
		str_of_num.push_back(cur_line);
		count_string++;
		sums.push_back(0);
	}
	end=true;
	get_num.close();
}
	
void counter(vector<string>& str_of_num, 
			 size_t& count_string, 
			 vector<int>& sums,
			 size_t& cur_num,
			 mutex& guard_cur_num,
			 bool& end){
	int x;
	int num_in_th;
	vector<int> cur_ints;
	{
		lock_guard<mutex> lock(guard_cur_num);
		num_in_th = cur_num;
		cur_num++; //lock			
	}
	while ((num_in_th < count_string)||(!end)){
		if (num_in_th < count_string){
			stringstream cur_stream(str_of_num[num_in_th]);
			do {
				cur_stream >> x;
				if (cur_stream) {
					cur_ints.push_back(x);
				}
			} while (cur_stream);
			sums[num_in_th] = my_sum(cur_ints);
			cur_ints.erase(cur_ints.begin(),cur_ints.end());	
			{
				lock_guard<mutex> lock(guard_cur_num);
				num_in_th = cur_num;
				cur_num++;
			}
		} 
	}
}

/*class sum_object
{
	vector<string> str_of_num;
	size_t count_string = 0;
	vector<int> sums;
	size_t cur_num = 0;
	bool end;
	
	mutex guard_cur_num;
};
*/
