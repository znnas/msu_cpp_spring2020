#pragma once

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <string>
#include <limits>

using namespace std;

class BigInt
{
	//работаем со сдвоенной цифрой (от 0 до 99), в начале массива меньшие разряды
    
	uint8_t* data_=nullptr;
    size_t data_size=0;
    bool isNegative;
    
public:

    BigInt(int64_t value=0);
    ~BigInt();

    BigInt(const BigInt& val);
    BigInt& operator=(const BigInt& copied);

    BigInt(BigInt&& moved);
    BigInt& operator=(BigInt&& moved);

    //printer
    friend ostream& operator<<(ostream& os, const BigInt& val);

    //plus
    BigInt& operator+=(const BigInt& rhs);
    BigInt operator+(const BigInt& other) const;
                                                              
    //minus
    BigInt& operator-=(const BigInt& rhs);
    BigInt operator-(const BigInt& other) const;
    BigInt operator-() const;

    //compare
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const {
    	return !(*this == other);
    }
    bool operator<(const BigInt& other) const;
    bool operator>=(const BigInt& other) const {
    	return !(*this < other);
    }
    bool operator>(const BigInt& other) const {
    	return !(*this < other || *this == other);
    }
    bool operator<=(const BigInt& other) const {
    	return !( *this > other );
    }

//sub_functions
public:
    
    size_t digits_count() const;
    friend bool abs_less(const BigInt& left, const BigInt& right);

private:

    void abs_add(const BigInt& other);
    void abs_dif(const BigInt& other);
    size_t mem_max(const BigInt& other);
    bool is_zero() const;

};

ostream& operator<<(ostream& os, const BigInt& val);
bool abs_less(const BigInt& left, const BigInt& right);
