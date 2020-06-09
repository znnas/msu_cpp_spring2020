#include <chrono>
#include <iostream>
#include <random>
#include <fstream>
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include <queue>
#include <vector>

using namespace std;

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


class thread_sort
{
private:
    size_t cnt_block;            // размер блока в количестве чисел типа uint64_t
    size_t portion;
    string file_for_sort;
    string file_sorted;

    int nom_part = 1;                    // номер следующего частично отсортированного файла
    int cur_part = 1;
    size_t sz = sizeof(uint64_t);

    mutex guard_nom_part;            // для блокировки nom_part
    mutex guard_cur_part;			 // для блокировки cur_part
    mutex guard_input;               // для блокировки входного файла
    mutex* guard_file;               // для блокировки файла, в которы сейчас пишем

private:
    void sort_th(ifstream& for_sort);
    void merge_parts(string& p1fn, string& p2fn, string& outfn);

public:
    thread_sort(size_t batch, size_t portion)
		: cnt_block(batch)
		, portion(portion) {
    	guard_file = new mutex[2*portion];
    }
    ~thread_sort() {}

    //sort
    void operator()(string file_for_sort = "for_sort.dat", string file_sorted = "sorted.dat", size_t num_of_thread = 2)
    {
    	ifstream for_sort(file_for_sort, ios::binary);
        if (!for_sort) {
            cout << "no file_for_sort!\n";
            return ;
        }

        cout << "Number of threads: " << num_of_thread << endl;

        Timer sort_timer; // время для сортировки
        sort_timer.start();
        cout << "Start sorting '" << file_for_sort << "'\n";

		auto fun = [this](ifstream& for_sort) {
			this->sort_th(for_sort);
		};
		vector<thread> t;
		for(int i=0; i<num_of_thread; i++){
			t.emplace_back( bind(fun, ref(for_sort)) );
		}
		for(thread& cur_t : t){
			cur_t.join();
		}

        sort_timer.stop();

        for_sort.close();
        remove(file_sorted.c_str());
        string res_string = "part_"+to_string(nom_part-1);
        int rc = rename(res_string.c_str(), file_sorted.c_str());
        cout << "File sorted = '" << file_sorted << "'\n";
    	cout << "Elapsed " << sort_timer.elapsedMilliseconds() << " milliseconds." << endl;
    	cout << endl;

    	nom_part = 1; //to use several times
    	cur_part = 1;
        return ;
    }
};
