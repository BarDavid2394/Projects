#include "string.h"
#include <iostream>
#include "port.h"


//aux function that helps us to split the input and determine if its src \ dst.
void split_argument(String packet, String *numbers, Port::Rule rule);
//constructor
port::port() {
    start_port = 0;
    end_port = 0;
    rule = Port::SRC;
    String numbers = String();
}
//copy constructor
port::port(const port &port_ad) {
    start_port = port_ad.start_port;
    end_port = port_ad.end_port;
    rule = port_ad.rule;
    numbers = port_ad.numbers;
}
//extract the rule's port range. will be compared in "match"
bool port::set_value(String value) {
    String copy = value.trim();
    String *output;
    size_t size = 0;
    copy.split("-", &output, &size);
    if (size != 2) {
        return false;
    }
    start_port = output[0].to_integer();
    end_port = output[1].to_integer();
    delete[] output;
    return true;
}
//checks if the packet's port is inside the allowed range.
bool port::match(String packet) {
    String numbers = String();
    split_argument(packet, &numbers, this->rule);
    int num = numbers.to_integer();
    return ((num <= end_port) && (num >= start_port));
}

void split_argument(String packet, String *numbers, Port::Rule rule) {
    String *output;
    size_t arg_size = 0;
    packet.split(",=", &output, &arg_size);
    if (arg_size == 0) {
        return;
    }
    if (rule == (Port::SRC)) {
        for(int i=0;i<8;i+=2)
        {
            if(output[i].equals("src-port"))
            {
                *numbers = output[i+1].trim();
            }
        }
    }
    if (rule == (Port::DST)) {
        for(int i=0;i<8;i+=2)
        {
            if(output[i].equals("dst-port"))
            {
                *numbers = output[i+1].trim();
            }
        }
    }
    delete[] output;
}
//decides if the rule is for the src\dst field.
void port::set_rule(String str) {
    if (str.equals("src")) {
        rule = (Port::SRC);
    } else {
        rule = (Port::DST);
    }

}
