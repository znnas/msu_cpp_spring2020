#pragma once
#include <iostream>
//#include <typeinfo>
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }

private:
    // process использует variadic templates
    template <class T>
    Error process(T&& val)
    {
        return save_(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        return (process(val) == Error::CorruptedArchive) ? Error::CorruptedArchive : process(forward<Args>(args)...);
    }

    Error save_(bool val)
    {
    	//https://en.cppreference.com/w/cpp/io/manip/boolalpha
        out_ << boolalpha << val << Separator;
        return (out_) ? Error::NoError : Error::CorruptedArchive;
    }

    Error save_(uint64_t val)
    {
        out_ << val << Separator;
        return (out_) ? Error::NoError : Error::CorruptedArchive;
    }
};

class Deserializer
{
    istream& in_;
public:
    explicit Deserializer(istream& in) : in_(in) {}

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    // process использует variadic templates
    template <class T>
    Error process(T&& val)
    {
        return load_(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        return (process(val) == Error::CorruptedArchive) ? Error::CorruptedArchive : process(forward<Args>(args)...);
    }

    Error load_(bool& value)
    {
    	in_ >> boolalpha >> value;
        return (in_) ? Error::NoError : Error::CorruptedArchive;
    }

    Error load_(uint64_t& value)
    {
    	in_ >> value;
        return (in_) ? Error::NoError : Error::CorruptedArchive;
    }

};
