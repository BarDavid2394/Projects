
#include <iostream>
#include "string.h"
#include "input.h"
#include "port.h"
#include "ip.h"

enum {
    SUCCESS = 0, FAIL
};

void split_argument(const String argument, String *rule_type,
                    String *type, String *numbers);

int main(int argc, char **argv) {
    //checking if the arguments inserted are valid.
    if (check_args(argc, argv)) {
        exit(1);
    }
    String type = String();
    String rule_type = String();
    String numbers = String();
    String argument(argv[1]);
    argument = argument.trim();
    split_argument(argument, &rule_type, &type, &numbers);
    if (type.equals("port")) {
        port *rule_port = new port();
        rule_port->set_rule(rule_type);
        rule_port->set_value(numbers);
        parse_input(*rule_port);
        delete rule_port;
    }
    if (type.equals("ip")) {
        ip *rule_ip = new ip();
        rule_ip->set_rule(rule_type);
        rule_ip->set_value(numbers);
        parse_input(*rule_ip);
        delete rule_ip;
    }
    return 0;
}
//aux function that splits the input into src\dst,ip\port,and the numbers field
void split_argument(const String argument, String *rule_type,
                    String *type, String *numbers) {
    String *output;
    size_t arg_size = 0;
    argument.split("=", &output, &arg_size);
    String *second_split;
    size_t second_size = 0;
    output[0].split("-", &second_split, &second_size);
    *rule_type = second_split[0].trim();
    *type = second_split[1].trim();
    *numbers = output[1].trim();
    delete[] second_split;
    delete[] output;
}

