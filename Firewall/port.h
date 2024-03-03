#ifndef PORT_H
#define PORT_H


#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>


namespace Port {
    enum Rule { SRC = 0, DST };
}

//comments are in the cpp file.
class port : public GenericField   {
private:
    Port::Rule rule;
    String numbers;
    int start_port, end_port;
public:
    port();
    port(const port &port_ad);
    void set_rule(String str);
    bool set_value(String value);
    bool match (String packet);
};




#endif