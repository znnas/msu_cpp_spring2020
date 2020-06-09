#include "uint64.h"

void thread_sort::sort_th(ifstream& for_sort)
{
	size_t cur;
	uint64_t* dat= new uint64_t[cnt_block];
    size_t sz = sizeof(uint64_t);
    while (!for_sort.eof()) {
        size_t cnt = 0;
        {
            lock_guard<mutex> lock(guard_input);
            while (cnt < cnt_block)
                if (!for_sort.read((char*) &dat[cnt], sz) )
                    break;
                else
                    ++cnt;
        }
        //sorting dat
        if (cnt > 0) {
            qsort(dat, cnt, sz, [](const void* a, const void* b) {
                uint64_t arg1 = *static_cast<const uint64_t*>( a );
                uint64_t arg2 = *static_cast<const uint64_t*>( b );
                if (arg1 < arg2) return -1;
                if (arg1 > arg2) return 1;
                return 0;
            //copied from https://en.cppreference.com/w/cpp/algorithm/qsort; don't work without comparator(((
            });

            string new_file;
            {
                lock_guard<mutex> lock(guard_nom_part);
                new_file = "part_" + to_string(nom_part);
                nom_part++;
            }
            ofstream sorted_part(new_file, ios::binary);
          	for (size_t i=0; i<cnt; ++i) {
          		sorted_part.write((char*) &dat[i], sz);
            }
            sorted_part.close();
        }
    }

    while (true) {
    	string file1;
    	string file2;
    	string file_merged;
    	bool flag = false;
    	{
            lock_guard<mutex> lock(guard_cur_part);
             if (cur_part < nom_part-1) {
                 file1 = "part_" + to_string(cur_part);
                 cur_part++;
                 file2 = "part_" + to_string(cur_part);
                 cur_part++;
                 flag = true;
         		 //cout << cur_part << endl;
             }
    	}
    	if (flag) {
            {
                lock_guard<mutex> lock(guard_nom_part);
           		file_merged = "part_" + to_string(nom_part);
           		cur = nom_part;
                nom_part++;
            }
            {
            	lock_guard<mutex> lock(guard_file[cur]);
                merge_parts(file1, file2, file_merged);
            }
    	}
    	else {
    		break;
    	}
    }
}


void thread_sort::merge_parts(string& p1fn, string& p2fn, string& outfn)
{
    ifstream part1(p1fn, ios::binary);
    ifstream part2(p2fn, ios::binary);
    ofstream outfile(outfn, ios::binary);

    queue<uint64_t> buffer1;
    queue<uint64_t> buffer2;

	bool stop1;
	bool stop2;
	uint64_t from1;
	uint64_t from2;
	size_t who_tail = 0;

	do {
		if (who_tail!=1) {
			for(int i=0; i<1000; i++){
				stop1 = !part1.read((char*) &from1, sz);
				if (!stop1) buffer1.emplace(from1);
			}
		}
		if (who_tail!=2) {
			for(int i=0; i<1000; i++){
				stop2 = !part2.read((char*) &from2, sz);
				if (!stop2) buffer2.emplace(from2);
			}
		}
		if (buffer1.empty() || buffer2.empty()){
			break;
		}
		while( (!buffer1.empty()) && (!buffer2.empty()) ){
			if (buffer1.front() < buffer2.front()) {
				outfile.write((char*) &buffer1.front(), sz);
				buffer1.pop();
			}
			else {
				outfile.write((char*) &buffer2.front(), sz);
				buffer2.pop();
			}
		}
		if (buffer1.empty()) {
			who_tail = 2;
		}
		else {
			who_tail = 1;
		}
	} while (true);

	if (who_tail==1) {
        while (!buffer1.empty()){
        	outfile.write((char*) &buffer1.front(), sz);
        	buffer1.pop();
        }
	}
	else {
		while (!buffer2.empty()){
			outfile.write((char*) &buffer2.front(), sz);
			buffer2.pop();
		}
	}

    part1.close();
    part2.close();
    outfile.close();
    remove(p1fn.c_str());
    remove(p2fn.c_str());
}

