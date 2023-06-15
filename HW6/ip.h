#ifndef IP_H
#define IP_H


#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>

//comments are in the cpp file.
namespace IP {
    enum Rule { SRC = 0, DST };
}


class ip : public GenericField   {
private:

    unsigned int ip_num;
    int mask;
    IP::Rule rule;
public:
    ip();
    ip(const ip &ip_ad);
    bool set_value(String value);
    bool match (String packet);
    void set_rule(String str);
};











#endif