#include "uint64.h"

void Uint64_order::sort_uint64(ifstream& for_sort)
{
    unique_ptr<uint64_t[]> dat(new uint64_t[cnt_block]);
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
            qsort(dat.get(), cnt, sz, [](const void* a, const void* b) {
                uint64_t arg1 = *static_cast<const uint64_t*>( a );
                uint64_t arg2 = *static_cast<const uint64_t*>( b );
                if (arg1 < arg2) return -1;
                if (arg1 > arg2) return 1;
                return 0;
            //https://en.cppreference.com/w/cpp/algorithm/qsort, don't work without comparator(((
            });

            //check queue
            bool is_empty;
            string file_name_from_queue;
            {
                lock_guard<mutex> lock(guard_queue_parts);
                is_empty = queue_parts.empty();
                if (!is_empty) {
                    file_name_from_queue = queue_parts.front();
                    queue_parts.pop();
                }
            }

            // get num of parts
            string file_name_merged;
            {
                lock_guard<mutex> lock(guard_nom_part);
                file_name_merged = "part_" + to_string(nom_part);
                nom_part++;
            }

            // write to file
            if  (is_empty) {
                ofstream sorted_part(file_name_merged, ios::binary);
                for (size_t i = 0; i < cnt; ++i) {
                    sorted_part.write((char*) &dat[i], sz);
                }
                sorted_part.close();
            }
            else { // merge data and queue file
                merge_array_with_part(dat, cnt, file_name_from_queue, file_name_merged);
            }

            // to queue
            {
                lock_guard<mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }
        }
    }
    while (true) {
        size_t sz_que;
        string file_part1_name;
        string file_part2_name;
        {
            lock_guard<mutex> lock(guard_queue_parts);
            sz_que = queue_parts.size();
            if (sz_que > 1) {
                file_part1_name = queue_parts.front();
                queue_parts.pop();
                file_part2_name = queue_parts.front();
                queue_parts.pop();
            }
        }
        if (sz_que > 1) {
            string file_name_merged;
            {
                lock_guard<mutex> lock(guard_nom_part);
                file_name_merged = "part_" + to_string(nom_part);
                nom_part++;
            }

            // merge two files
            merge_parts(file_part1_name, file_part2_name, file_name_merged);

            // result to queue
            {
                lock_guard<mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }
        }
        else {
            break;
        }
    }
}


void Uint64_order::merge_parts(string& p1fn, string& p2fn, string& outfn)
{
    ifstream part1file;
    ifstream part2file;
    ofstream outfile;
    part1file.open(p1fn, ios::binary);
    part2file.open(p2fn, ios::binary);
    outfile.open(outfn, ios::binary);

    uint64_t value1;
    uint64_t value2;
    size_t sz = sizeof(uint64_t);

    bool stop_part1 = !part1file.read((char*) &value1, sz);
    bool stop_part2 = !part2file.read((char*) &value2, sz);

    while ( !(stop_part1 && stop_part2) ) {
        if (!stop_part1 && !stop_part2) {
            if (value1 < value2) {
                outfile.write((char*) &value1, sz);
                stop_part1 = !part1file.read((char*) &value1, sz);
            }
            else {
                outfile.write((char*) &value2, sz);
                stop_part2 = !part2file.read((char*) &value2, sz);
            }
        }
        if (!stop_part1 && stop_part2) {
            outfile.write((char*) &value1, sz);
            stop_part1 = !part1file.read((char*) &value1, sz);
        }
        if (stop_part1 && !stop_part2) {
            outfile.write((char*) &value2, sz);
            stop_part2 = !part2file.read((char*) &value2, sz);
        }
    }
    part1file.close();
    part2file.close();
    outfile.close();
    remove(p1fn.c_str());
    remove(p2fn.c_str());
}

void Uint64_order::merge_array_with_part(unique_ptr<uint64_t[]>& dat, size_t cnt, string& pfn, string& outfn)
{
    ifstream partfile;
    ofstream outfile;
    partfile.open(pfn, ios::binary);
    outfile.open(outfn, ios::binary);

    uint64_t value;
    size_t sz = sizeof(uint64_t);

    bool stop_dat = (cnt == 0);
    size_t idx = 0;
    bool stop_part = !partfile.read((char*) &value, sz);

    while ( !(stop_dat && stop_part) ) {
        if (!stop_dat && !stop_part) {
            if (dat[idx] < value) {
                outfile.write((char*) &dat[idx++], sz);
                stop_dat = (idx >= cnt);
            }
            else {
                outfile.write((char*) &value, sz);
                stop_part = !partfile.read((char*) &value, sz);;
            }
        }
        if (!stop_dat && stop_part) {
            outfile.write((char*) &dat[idx++], sz);
            stop_dat = ( idx >= cnt );
        }
        if (stop_dat && !stop_part) {
            outfile.write((char*) &value, sz);
            stop_part = !partfile.read((char*) &value, sz);;
        }
    }
    partfile.close();
    outfile.close();
    remove(pfn.c_str());
}
