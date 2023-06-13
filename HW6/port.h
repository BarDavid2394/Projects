#ifndef PORT_H
#define PORT_H


#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>


namespace Port {
    enum Rule { SRC = 0, DST };
}


class port : public GenericField   {
private:
    short start_port, end_port;
    Port::Rule rule;
    String numbers;
public:
    port();
    port(const port &port_ad);
    void set_rule(String str);
    bool set_value(String value);
    bool match (String packet);
//    void split_argument(String packet, String* numbers,Port::Rule rule);
};








#endif