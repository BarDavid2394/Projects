#include <iostream>
#include <cstring>
#include "string.h"

enum {
    SUCCESS = 0, FAIL
};
//aux function to check if the data is made of digits. used in "to integer".
//rest of the comments are built in inside the header file.
bool check_if_digit(char *data) {
    return std::string(data).find_first_not_of
    ("0123456789") == std::string::npos;
}

String::String()  {
    length = (0);
    data = new char[1];
    data[0] = '\0';
}

String::String(const String &str){

    length = (strlen(str.data));
    data = new char[length + 1];
    strcpy(data, str.data);
}

String::String(const char *str) {

    length = (strlen(str));
    data = new char[length + 1];
    strcpy(data, str);
}

String::~String() {
    delete[] data;
}

String &String::operator=(const String &rhs) {
    if (&rhs != this) {
        delete[] data;
        length = rhs.length;
        data = new char[length + 1];
        strcpy(data, rhs.data);
    }
    return *this;
}

String &String::operator=(const char *str) {
    if (str != data) {
        delete[] data;
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }
    return *this;
}

bool String::equals(const String &rhs) const {
    if (length != rhs.length) {
        return false;
    }
    return std::strcmp(data, rhs.data) == 0;
}

bool String::equals(const char *rhs) const {
    size_t rhs_length = std::strlen(rhs);
    if (rhs_length != length) {
        return false;
    }
    return std::strcmp(data, rhs) == 0;
}

String String::trim() const {
    size_t start_counter = 0;
    size_t end_counter = length;
    while ((start_counter < length) && ((data[start_counter] == ' ') ||
                                        (data[start_counter] == '\n') ||
                                        (data[start_counter] == '\t'))) {
        start_counter++;
    }
    while ((end_counter > start_counter) && ((data[end_counter-1] == ' ')
                                             || (data[end_counter-1] == '\n')
                                             ||(data[end_counter-1] == '\t'))){
        end_counter--;
    }
    size_t real_length = (end_counter - start_counter);
    char *real_data = new char[real_length + 1];
    for (size_t i = 0; i < real_length; i++) {
        real_data[i] = data[i + start_counter];
    }
    real_data[real_length] = '\0';
    String trim_string(real_data);
    delete[] real_data;
    return trim_string;
}


int String::to_integer() const {
    if (!check_if_digit(data)) {
        return FAIL;
    }
    return atoi(data);
}

void String::split(const char *delimiters,
                   String **output, size_t *size) const {

    if (length == 0) {
        *size = 0;
        return;
    }
    String copy = String(this->data);
    size_t sub_counter = 0;
    char *token = strtok(copy.data, delimiters);
    while (token != NULL) {
        ++sub_counter;
        token = strtok(NULL, delimiters);
    }
    *size = sub_counter;
    if (output == NULL) {
        return;
    }
    if (sub_counter == 0) {
        *output = new String[1];
        (*output)[sub_counter] = *this;
    }
    (*output) = new String[sub_counter];
    String copy2 = String(this->data);
    token = strtok(copy2.data, delimiters);
    sub_counter = 0;
    while (token != nullptr) {
        (*output)[sub_counter] = String(token).trim();

        sub_counter++;
        token = strtok(nullptr, delimiters);
    }
}

