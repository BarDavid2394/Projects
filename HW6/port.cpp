#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>
#include "port.h"

void split_argument(String packet, String* numbers,Port::Rule rule);
port :: port(){
     start_port=0;
     end_port=0;
     rule;
    String  numbers = String();
}
port :: port(const port &port_ad)
{
    start_port = port_ad.start_port;
    end_port = port_ad.end_port;
    rule = port_ad.rule;
    numbers = port_ad.numbers;
}


bool port ::set_value (String value)
{
   String copy =value.trim();
    String* output;
    size_t size=0;
//    const char* list = "-";
   value.split("-",&output,&size);
   if(size != 2)
   {
       return false;
   }
   start_port = output[0].to_integer();
   end_port = output[1].to_integer();
    return true;
}
bool port ::match(String packet) {
    String numbers=String();
    split_argument(packet,&numbers, this->rule);
    int num = numbers.to_integer();
    return((num <= end_port) && (num >= start_port));
}

void split_argument(String packet, String* numbers,Port :: Rule rule) {
    String* output;
    size_t arg_size=0;
//    const char *arg_list = ",=";
    packet.split(",=", &output, &arg_size);
    if(arg_size == 0){
        return;
    }
    if (rule == (Port :: SRC)) {
        *numbers = output[5].trim();
    }
    if (rule == (Port :: DST)) {
        *numbers = output[7].trim();
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
