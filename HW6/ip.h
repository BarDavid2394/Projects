#ifndef IP_H
#define IP_H


#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>


namespace IP {
    enum Rule { SRC = 0, DST };
}


class ip : public GenericField   {
private:
    unsigned int ip;
    int mask;
    IP::Rule rule;


public:
    bool set_value(String value);
    bool match (String packet);
    void set_rule(String str);
//    void split_argument(String packet, String* numbers,IP::Rule rule);

};











#endif