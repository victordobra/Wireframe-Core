#include "string.hpp"
#include <string.h>
#include <assert.h>

namespace mge {
    //Constructors
    string::string() : str(nullptr), _capacity(0) { }
    string::string(const string& other) : _capacity(other._capacity), str(new char_t[other._capacity]) {
        //Copy the string over
        memcpy(str, other.str, _capacity * sizeof(char_t));
    }
    string::string(string&& other) noexcept : _capacity(other._capacity), str(other.str) {
        //Unassign the other vector's string pointer
        other.str = nullptr;
    }
    string::string(const char_t* other) : _capacity(1) {
        //Get the size of the string
        size_t sSize = strlen(other) + 1;
        assert((sSize <= MAX_SIZE) && "The string's size bust be lower than or equal to MAX_SIZE!");

        //Find the lowest possible capacity
        while(_capacity < sSize) {
            _capacity = _capacity << 1;
        }
        
        str = new char_t[_capacity];
        memcpy(str, other, sSize);
    }

    //Operators
    string& string::operator=(const string& other) {
        //If the other string is the same as this string, exit the function
        if (&other == this)
            return *this;

        //Delete the old string
        delete[] str;

        //Create the new string
        _capacity = other._capacity;
        str = new char_t[_capacity];

        //Copy everything over from the other string
        memcpy(str, other.str, _capacity);

        return *this;
    }
    string& string::operator=(string&& other) noexcept {
        //Delete the old string
        delete[] str;

        //Assign this string to the other string's memory
        _capacity = other._capacity;
        str = other.str;

        //Unassign the other string's pointer
        other.str = nullptr;

        return *this;
    }
    string& string::operator+=(const string& other) {
        return append(other);
    }
    string& string::operator+=(string&& other) noexcept {
        return append(other);
    }
    string string::operator+(const string& other) {
        string result(*this);
        result.append(other);
        return result;
    }
    string string::operator+(string&& other) {
        string result(*this);
        result.append(other);
        return result;
    }

    //Modifiers
    string& string::resize(size_t newSize) {
        //Increment the new size
        newSize++;

        //Assert that the new size must be lower than or equal to MAX_SIZE 
        assert((newSize <= MAX_SIZE) && "The string's size must be lower than or equal to MAX_SIZE!");

        //If the new size is lower than or equal to the capacity of the string, exit the function
        if (newSize <= _capacity)
            return *this;

        //Find the lowest possible capacity
        size_t newCapacity = 1;
        while(newCapacity < newSize) {
            newCapacity = newCapacity << 1;
        }

        //Reserve for the new capacity
        reserve(newCapacity);

        return *this;
    }
    string& string::reserve(size_t newCapacity) {
        //Assert that the new capacity must be lower than or equal to MAX_SIZE 
        assert((newCapacity <= MAX_SIZE) && "The string's capacity must be lower than or equal to MAX_SIZE!");

        size_t minCapacity = (_capacity < newCapacity) ? _capacity : newCapacity;
        size_t maxCapacity = (_capacity > newCapacity) ? _capacity : newCapacity;

        //Create the new string and copy everything to it
        char_t* newStr = new char_t[newCapacity];

        memcpy(newStr, str, minCapacity);

        //Add a null termination character at the end
        if(newCapacity < _capacity)
            newStr[newCapacity - 1] = 0;
        else
            newStr[minCapacity] = 0;

        //Set the new capacity
        _capacity = newCapacity;

        //Delete the old string and replace it with the new one
        delete[] str;
        str = newStr;

        return *this;
    }
    string& string::append(const string& other) {
        //Resize the string
        size_t newSize = length() + other.length();
        resize(newSize);

        //Concatenate the strings
        strcat(str, other.str);

        return *this;
    }
    string& string::append(size_t n, char_t c) {
        //Resize the string
        size_t oldSize = length();
        size_t newSize = length() + n;
        resize(newSize);

        //Add all of the new characters
        memset(str + oldSize, c, n * sizeof(char_t));
        str[newSize] = 0;

        return *this;
    }
    string& string::push_back(char_t c) {
        //Resize the string
        size_t oldSize = length();
        resize(oldSize + 1);

        //Add the new character
        str[oldSize] = c;
        str[oldSize + 1] = 0;

        return *this;
    }
    string& string::pop_back() {
        //If the string is empty, exit the function
        size_t len = length();
        if(!len)
            return *this;

        //Set the last character to the string termination character
        str[len] = 0;

        return *this;
    }
    string& string::assign(const string& other) {
        //If the other string is the same as this string, exit the function
        if (&other == this)
            return *this;

        //Delete the old string
        delete[] str;

        //Create the new string
        _capacity = other._capacity;
        str = new char_t[_capacity];

        //Copy everything over from the other string
        memcpy(str, other.str, _capacity);

        return *this;
    }
    string& string::insert(size_t pos, const string& other) {
        //Resize the string
        size_t oldSize = length();
        size_t otherSize = other.length();
        size_t newSize = oldSize + otherSize;
        resize(newSize);

        //Move everything over to make space for the string
        memmove(str + pos + otherSize, str + pos, oldSize - pos + 1);

        //Place the other string into the newly created spot
        memcpy(str + pos, other.str, otherSize);

        return *this;
    }
    string& string::insert(size_t pos, size_t n, char_t c) {
        //Resize the string
        size_t oldSize = length();
        size_t newSize = oldSize + n;
        resize(newSize);

        //Move everything over to make space for the string
        memmove(str + pos + n, str + pos, oldSize - pos + 1);

        //Fill the new space with characters
        memset(str + pos, c, n);

        return *this;
    }
    string& string::erase(size_t pos, size_t len) {
        //If the length is -1, set the length to reach to the end of the string
        if(len == -1)
            len = length() - pos;

        //Set every following character to the null termination character
        memset(str + pos, 0, len * sizeof(char_t));
        
        return *this;
    }

    //Destructor
    string::~string() {
        delete[] str;
    }

    //sto_ functions
    sint32_t  stoi  (const string& str, char_t** endPtr, size_t base) {
        return strtol(str.c_str(), endPtr, (sint32_t)base);
    }
    sint32_t  stol  (const string& str, char_t** endPtr, size_t base) {
        return strtol(str.c_str(), endPtr, (sint32_t)base);
    }
    sint64_t  stoll (const string& str, char_t** endPtr, size_t base) {
        return strtoll(str.c_str(), endPtr, (sint32_t)base);
    }
    uint32_t  stoui (const string& str, char_t** endPtr, size_t base) {
        return strtoul(str.c_str(), endPtr, (sint32_t)base);
    }
    uint32_t  stoul (const string& str, char_t** endPtr, size_t base) {
        return strtoul(str.c_str(), endPtr, (sint32_t)base);
    }
    uint64_t  stoull(const string& str, char_t** endPtr, size_t base) {
        return strtoull(str.c_str(), endPtr, (sint32_t)base);
    }
    float32_t stof  (const string& str, char_t** endPtr) {
        return strtof(str.c_str(), endPtr);
    }
    float64_t stod  (const string& str, char_t** endPtr) {
        return strtod(str.c_str(), endPtr);
    }
    
    //ToString overloads
    template<> 
    string ToString(sint8_t   number) {
        if(!number)
            return "0";

        char_t num[30]; 
        size_t b = 30;
        bool8_t negative = number < 0;
        if(negative)
            number *= -1;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        if(negative)
            num[--b] = '-';
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(sint16_t  number) {
        if(!number)
            return "0";

        char_t num[30]; 
        size_t b = 30;
        bool8_t negative = number < 0;
        if(negative)
            number *= -1;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        if(negative)
            num[--b] = '-';
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(sint32_t  number) {
        if(!number)
            return "0";

        char_t num[30]; 
        size_t b = 30;
        bool8_t negative = number < 0;
        if(negative)
            number *= -1;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        if(negative)
            num[--b] = '-';
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(sint64_t  number) {
        if(!number)
            return "0";

        char_t num[30]; 
        size_t b = 30;
        bool8_t negative = number < 0;
        if(negative)
            number *= -1;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        if(negative)
            num[--b] = '-';
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(uint8_t   number) {
        if(!number)
            return "0";

        char_t num[31]; num[30] = 0; 
        size_t b = 30;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(uint16_t  number) {
        if(!number)
            return "0";

        char_t num[31]; num[30] = 0; 
        size_t b = 30;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(uint32_t  number) {
        if(!number)
            return "0";

        char_t num[31]; num[30] = 0; 
        size_t b = 30;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(uint64_t  number) {
        if(!number)
            return "0";

        char_t num[31]; num[30] = 0; 
        size_t b = 30;
        
        while(number) {
            num[--b] = number % 10 + '0';
            number /= 10;
        }
        
        string str = num + b;
        return str;
    }
    template<> 
    string ToString(float32_t number) {
        char_t buf[50];
        gcvt(number, 15, buf);
        string str = buf;
        return str;
    }
    template<> 
    string ToString(float64_t number) {
        char_t buf[50];
        gcvt(number, 15, buf);
        string str = buf;
        return str;
    }
    template<> 
    string ToString(bool8_t val) {
        string str = val ? "true" : "false";
        return str;
    }
} 