#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>
#include "port.h"

void split_argument(String packet, String* numbers,Port::Rule rule);
bool port ::set_value (String value)
{
   String copy =value.trim();
   String** output = nullptr;
   size_t *size =nullptr;
    const char* list = "-";
   value.split(list,output,size);
   if(*size != 2)
   {
       return false;
   }
   start_port = output[0]->to_integer();
   end_port = output[1]->to_integer();
    return true;
}
bool port ::match(String packet) {
    String* numbers= nullptr;
    split_argument(packet,numbers, this->rule);
    int num = numbers->to_integer();
    return((num <= end_port) && (num >= start_port));
}

void split_argument(String packet, String* numbers,Port :: Rule rule) {
    String **output=nullptr;
    size_t *arg_size=nullptr;
    const char *arg_list = ",=";
    packet.split(arg_list, output, arg_size);
    if (rule == (Port :: SRC)) {
        *numbers = output[5]->trim();
    }
    if (rule == (Port :: DST)) {
        *numbers = output[7]->trim();
    }
}

void port::set_rule(String str) {
    if(str.equals("src"))
    {
        rule =(Port :: SRC);
    }
    else
    {
        rule = (Port :: DST);
    }

}
