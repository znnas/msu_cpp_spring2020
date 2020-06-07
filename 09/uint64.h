#include <chrono>
#include <iostream>
#include <random>
#include <limits>
#include <fstream>
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include <queue>
#include <iterator>
#include <cstdio>

using namespace std;

class Timer
{
public:
    Timer()
        : start_(chrono::high_resolution_clock::now())
    {}

    ~Timer()
    {
        const auto finish = chrono::high_resolution_clock::now();
        cout << chrono::duration_cast<chrono::microseconds>( finish - start_ ).count() << " us" << endl;
    }

private:
    const chrono::high_resolution_clock::time_point start_;
};


class Uint64_order
{
private:
    size_t cnt_block = 100000;            // size of block
    string file_for_sort;
    string file_sorted;

    queue<string> queue_parts; 			 // names of sorted files
    int nom_part = 1;                    // current number of file
    size_t sz = sizeof(uint64_t);

    mutex guard_nom_part;
    mutex guard_queue_parts;
    mutex guard_input;

private:
    // сортировка чисел из потока ввода
    void sort_uint64(ifstream& for_sort);

    // слияние двух файлов с упорядоченными числами
    void merge_parts(string& p1fn, string& p2fn, string& outfn);

    // слияние массива отсортированных чисел и файла с упорядоченными числами
    void merge_array_with_part(unique_ptr<uint64_t[]>& dat, size_t cnt, string& pfn, string& outfn);

public:
    Uint64_order(size_t batch = 100000, string file_in = "for_sort.dat", string file_out = "sorted.dat")
		: cnt_block(batch)
		, file_for_sort(file_in)
		, file_sorted(file_out) {}
    ~Uint64_order() {}

    //sort
    void operator()(string file_for_sort = "for_sort.dat", string file_sorted = "sorted.dat")
    {
        ifstream for_sort(file_for_sort, ios::binary);
        if (!for_sort) {
            cout << "no file_for_sort!\n";
            return ;
        }
        cout << "Start sorting '" << file_for_sort << "'\n";
        {
            Timer ttt; // время для сортировки
            auto fun = [this](ifstream& for_sort) { this->sort_uint64(for_sort); };
            thread t1(bind(fun, ref(for_sort)));
            thread t2(bind(fun, ref(for_sort)));
            t1.join();
            t2.join();
        }
        for_sort.close();
        remove(file_sorted.c_str());
        int rc = rename(queue_parts.front().c_str(), file_sorted.c_str());
        cout << "File sorted = '" << file_sorted << "'\n";
        return ;
    }

};

