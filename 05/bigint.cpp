#include "bigint.h"

//base operators, copy and move
BigInt::BigInt(int64_t number)
{
    data_=new uint8_t[10];
    isNegative= (number < 0);
    data_size=10;
    int64_t num=abs(number);
    for (int i=0; i < 10; ++i) {
        data_[i]=num % 100;
        num/=100;
    }
}

BigInt::~BigInt()
{
    delete[] data_;
}

BigInt::BigInt(const BigInt& copied)
	: data_(new uint8_t[copied.data_size])
	, data_size(copied.data_size)
	, isNegative(copied.isNegative)
{
    copy(copied.data_, copied.data_ + data_size, data_);
}

BigInt& BigInt::operator=(const BigInt& copied)
{
    if (this == &copied){
    	return *this;
    }
    uint8_t* ptr=new uint8_t[copied.data_size];
    delete[] data_;
	data_=ptr;
	data_size=copied.data_size;
	copy(copied.data_, copied.data_ + data_size, data_);
	isNegative=copied.isNegative;
    return *this;
}

BigInt::BigInt(BigInt&& moved)
	: data_(moved.data_)
	, data_size(moved.data_size)
	, isNegative(moved.isNegative)
{
   moved.data_=nullptr;
   moved.data_size=0;
}

BigInt& BigInt::operator=(BigInt&& moved)
{
    if (this == &moved){
    	return *this;
    }
    delete[]  data_;
    data_=moved.data_;
    data_size=moved.data_size;
    moved.data_=nullptr;
    moved.data_size=0;
    isNegative=moved.isNegative;
    return *this;
}

//printer
ostream& operator<<(ostream& os, const BigInt& val)
{
    int first_digit=val.data_size;
    if (first_digit == 0){
    	os << '0';
    }
    else {
		if (val.isNegative) {
			os << '-';
		}
		while (val.data_[first_digit - 1] == 0 && first_digit > 1 ){
			--first_digit;
		}
		if (first_digit == 1 && val.data_[0] == 0){
			os << '0';
		}
		else {
//			if (val.isNegative) {
//				os << '-';
//			}
			os << int(val.data_[first_digit-1]);
			--first_digit;
			while (first_digit > 0) {
				if (val.data_[first_digit-1] < 10){
					os << '0' << int(val.data_[first_digit-1]);
				}
				else {
					os << int(val.data_[first_digit-1]);
				}
				--first_digit;
			}
		}
    }
    return os;
}

//plus
BigInt& BigInt::operator+=(const BigInt& other)
{
    if (isNegative == other.isNegative) {
        abs_add(other);
    }
    else {
        abs_dif(other);
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    BigInt tmp(*this);
    tmp += other;
    return tmp;
}

// minus
BigInt& BigInt::operator-=(const BigInt& other)
{
    if (isNegative != other.isNegative) {
        abs_add(other);
    }
    else {
        abs_dif(other);
    }
    return *this;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt tmp(*this);
    tmp -= other;
    return tmp;
}

BigInt BigInt::operator-() const
{
    BigInt tmp(*this);
    if (is_zero()) return tmp;  //нулю знак не меняем
    tmp.isNegative=!isNegative;
    return tmp;
}

//compare
bool BigInt::operator<(const BigInt& other) const
{
    if (*this == other) return false; //далее числа не равны

    if (isNegative && !other.isNegative) return true;
    if (!isNegative && other.isNegative) return false;

    bool less=abs_less(*this, other);

    return (!isNegative && less) || (isNegative && !less); // поэтому здесь просто !less
}

bool BigInt::operator==(const BigInt& other) const
{
    if (this == &other) return true;
    if (isNegative != other.isNegative) return false;

    int count=digits_count();
    int count_other=other.digits_count();
    if (count != count_other) return false;

    while (count > 0) {
        if (data_[count-1] != other.data_[count-1]) return false;
        --count;
    }
    return true;
}

//sub_functions
bool BigInt::is_zero() const
{
    size_t count = data_size;
    while (data_[count-1]==0 && count>0) {
    	--count;
    }
    if (count==0) return true;
    return false;
}

size_t BigInt::digits_count() const
{
    size_t count = data_size;
    while (data_[count-1]==0 && count>1) {
    	--count;
    }
    return count;
}

size_t BigInt::mem_max(const BigInt& other){
	// увеличивает память, если она может потребоваться при арифметической операции, и возвращает максимальное число разрядов

	size_t count=digits_count();
	size_t count_other=other.digits_count();
	size_t count_max=(count > count_other) ? count + 1 : count_other + 1;

	if (data_size < count_max) {
		uint8_t* ptr=new uint8_t[count_max];
		copy(data_, data_ + data_size, ptr);
		delete[] data_;
		data_=ptr;
		for (int i=data_size; i < count_max; ++i)
			ptr[i]=0;
		data_size=count_max;
	}
	return(count_max);
}

void BigInt::abs_add(const BigInt& other)
{
	size_t count_max = mem_max(other);
	size_t count_other = other.digits_count();

    uint8_t num_up = 0;
    int res;

    for (int i=0; i<count_max; ++i) {
        if (i < count_other) {
            res = data_[i] + other.data_[i] + num_up;
        }
        else {
            res = data_[i] + num_up;
        }
        data_[i] = res % 100;
        num_up = res / 100;
    }
}

void BigInt::abs_dif(const BigInt& other)
{
	size_t count_max = mem_max(other);
	size_t count_other = other.digits_count();

    bool less = abs_less(*this, other);
    uint8_t num_up=0;
    int res;

    if (!less) {
		for (int i=0; i<count_other; ++i) {
			res = data_[i] - other.data_[i];
			if (res < 0){
				res+=100;
				data_[i+1]--;
			}
			data_[i] = res;
		}
		if (is_zero()) {
			isNegative=false;
		}
    }
    else {
		for (int i=0; i<count_other; ++i) {
			res = other.data_[i] - data_[i];
			if (res < 0){
				res+=100;
				other.data_[i+1]--; // число меньше other, поэтому в максимальной цифре сюда не приходим
			}
			data_[i] = res;
		}
		isNegative=!isNegative;
    }
}

bool abs_less(const BigInt& left, const BigInt& right)
{
    int l_count=left.digits_count();
    int r_count=right.digits_count();

    if (l_count < r_count) return true;
    if (l_count > r_count) return false;

    while (l_count > 0) {
        if (left.data_[l_count-1] < right.data_[l_count-1] ) return true;
        if (left.data_[l_count-1] > right.data_[l_count-1] ) return false;
        --l_count;
    }
    return false;
}
