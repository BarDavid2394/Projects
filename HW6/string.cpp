#include <iostream>
#include <cstring>
#include "string.h"

enum {SUCCESS= 0, FAIL};

bool check_if_digit(char* data)
{
    return std::string (data).find_first_not_of("0123456789") == std::string::npos;
}



String::String() : data(nullptr), length(0) {
    data = new char[1];
    data[0] ='\0';
}

String ::String (const String &str) : data(nullptr),length(str.length)
{
    data = new char [length+1];
    strcpy(data,str.data);
}

String :: String(const char *str) : data(nullptr),length(strlen(str))
{
    data = new char [length+1];
    strcpy(data,str);
}

String::  ~String()
{
    delete[] data;
}

String& String::  operator=(const String &rhs)
{
    if(&rhs != this)
    {
        length = rhs.length;
        data = new char[length+1];
        strcpy(data,rhs.data);
    }
    return *this;
}

String& String :: operator=(const char *str)
{
    if(str != data)
    {
        delete[] data;
        length = strlen(str);
        data = new char[length+1];
        strcpy(data,str);
    }
    return *this;
}

bool String::equals(const String& rhs) const {
    if (length != rhs.length) {
        return false;
    }

    return std::strcmp(data, rhs.data) == 0;
}

bool String::equals(const char* rhs) const {
    size_t rhs_length = std::strlen(rhs);
    if (rhs_length != length) {
        return false;
    }

    return std::strcmp(data, rhs) == 0;
}

String String::trim() const
{
    size_t start_counter=0;
    size_t end_counter=length-1;
    while((start_counter<length) && ((data[start_counter]==' ')||
           (data[start_counter]=='\n')||(data[start_counter]=='\t')))
    {
        start_counter++;
    }
    while ((end_counter>start_counter) && ((data[end_counter]==' ')||(data[end_counter]=='\n')
            ||(data[end_counter]=='\t')))
    {
        end_counter--;
    }
    size_t real_length=((end_counter-start_counter)+1);
    char* real_data=new char[real_length+1];
    for(size_t i=0;i<real_length;i++)
    {
        real_data[i]=data[i+start_counter];
    }
    real_data[real_length]='\0';
    String trim_string(real_data);
    delete[] real_data;
    return trim_string;
}


int String :: to_integer() const
{
    if(!check_if_digit(data))
    {
        return FAIL;
    }
    int res = atoi(data);
    return res;
}


void String :: split(const char *delimiters, String **output, size_t *size) const {
    if (length == 0) {
        *size = 0;
        return;
    }
    char *copy = new char[length + 1];
    strcpy(copy, data);
    size_t sub_counter = 0;
    char *token = strtok(copy, delimiters);
    while (token != nullptr) {
        sub_counter++;
        token = strtok(nullptr, delimiters);
    }
    *output = new String[sub_counter];
    token = strtok(copy, delimiters);
    sub_counter = 0;
    while (token != nullptr) {
        (*output)[sub_counter++] =String(token);
        token = strtok(nullptr, delimiters);
    }
    if (sub_counter == 0) {
        (*output)[sub_counter] = *this;
    }
    delete[] copy;
    size = &sub_counter;
    return;
}


