#include <stddef.h> // size_t
#include <iostream>
#include <cstring> // strlen, strcpy
#include "string.h"
#include "input.h"
#include "port.h"
#include "ip.h"

enum {SUCCESS = 0,FAIL};

void split_argument(const String& argument,String* rule_type, String* type, String* numbers)
{
    String **output = nullptr;
    size_t *arg_size= nullptr;
    const char* arg_list = "=";
    argument.split(arg_list,output,arg_size);
    String** second_split=nullptr;
    size_t* second_size=nullptr;
    const char* arg_list2 = "-";
    (*output)[0].split(arg_list2,second_split,second_size);
    *rule_type = second_split[0]->trim();
    *type = second_split[1]->trim();
    *numbers = output[1]->trim();
}

int main(int argc, char **argv) {
    if(check_args( argc,  argv) != 0)
    {
        return FAIL;
    }
    String* type=nullptr;
    String* rule_type=nullptr;
    String* numbers=nullptr;
    String argument=argv[1];
    split_argument(argument,rule_type,type,numbers);
    if (type->equals("port"))
    {
        port* rule_port = new port;
        rule_port->set_rule(*rule_type);
        rule_port->set_value(*numbers);
//        parse_input(*rule_port);
        delete rule_port;
    }
    if (type->equals("ip"))
    {
        ip* rule_ip=new ip;
        rule_ip->set_rule(*rule_type);
        rule_ip->set_value(*numbers);
//        parse_input(*rule_ip);
        delete rule_ip;
    }
    return 0;
}


