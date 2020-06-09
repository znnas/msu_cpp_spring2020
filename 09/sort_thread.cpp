#include "uint64.h"

void check_sort(string sorted_file){
    uint64_t prev;
    uint64_t next;
    bool is_correct = true;
    size_t count_of_numbers = 1;
    size_t sz = sizeof(uint64_t);

    ifstream check_sort(sorted_file, ios::binary);
    bool read_ok = !( !check_sort.read((char*) &prev, sz) );

    while (read_ok) {
        read_ok = !( !check_sort.read((char*) &next, sz) );
        if (read_ok) {
            ++count_of_numbers;
            if (prev > next) {
                cout << "ERROR SORTING Pos= " << count_of_numbers << "\n";
                is_correct = false;
            }
            next = prev;
        }
    }
    cout << count_of_numbers << " numbers were sorted " << "\n";
    cout << "Sorting is '" << (is_correct ? "correct" : "uncorrect") << "'.\n";
    cout << endl;

}

int main()
{
    // 1. Preparing
	// unsorted and sorted files with numbers
    string file_for_sort = "for_sort.dat";
    string sorted_file = "sorted.dat";

    // make unsorted
    size_t sz = sizeof(uint64_t);
    size_t batch = 100000; // по условию памяти не более 8mb -- нам нужно, чтобы batch * bytes_of_uint < 8mb.
    size_t portion = 10;
    size_t sz_sorting_data = batch * portion;
    size_t count = 0;

	Timer generate_timer; //время создания файла для сортировки
	generate_timer.start();

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<uint64_t> dis(0, numeric_limits<uint64_t>::max()-1);
	//not rand anymore: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution

	ofstream for_sort(file_for_sort, ios::binary);

	for (size_t i=0; i<sz_sorting_data; ++i) {
		uint64_t rnd = dis(gen);
		for_sort.write((char*) &rnd, sz);
		++count;
	}
	for_sort.close();
	if(!for_sort.good()) {
	    cout << "Error occurred at writing time!" << endl;
	    return 1;
	}
	generate_timer.stop();
	cout << "file for_sort.dat created, count = " << count << "\n";
	cout << "Elapsed " << generate_timer.elapsedMilliseconds() << " milliseconds." << endl;
	cout << endl;

	// 2. Sort and check
	size_t num_of_threads;
	thread_sort order(batch, portion);

	num_of_threads = 1;
	order(file_for_sort, "sorted1.dat", num_of_threads); //write in sorted_file
	check_sort("sorted1.dat");

	num_of_threads = 2;
	order(file_for_sort, "sorted2.dat", num_of_threads); //write in sorted_file
	check_sort("sorted2.dat");

	num_of_threads = 4;
	order(file_for_sort, "sorted4.dat", num_of_threads); //write in sorted_file
	check_sort("sorted4.dat");
}

