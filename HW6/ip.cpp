#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>
#include "ip.h"
void split_argument(String packet, String* numbers,IP::Rule rule);
void split_argument(String packet, String* numbers,IP :: Rule rule)
{
    String** output=nullptr;
    size_t *arg_size=nullptr;
    const char* arg_list = ",=";
    packet.split(arg_list,output,arg_size);
    if(rule == (IP ::SRC))
    {
        *numbers = output[1]->trim();
    }
    if(rule == (IP ::DST))
    {
        *numbers = output[3]->trim();
    }

}

bool ip ::set_value (String value)
{
    String copy =value.trim();
    String** output=nullptr;
    size_t *size=nullptr;
    const char* list = "./";
    value.split(list,output,size);
    if(*size != 5)
    {
        return false;
    }
    ip=((*output)[0].to_integer() <<  24) + ((*output)[1].to_integer() <<  16) +
            ((*output)[2].to_integer() <<  8) +((*output)[3].to_integer());
    mask = (*output)[4].to_integer();
    ip =( ip >> mask);
    return true;
}
bool ip::match(String packet) {
    String* numbers=nullptr;
    split_argument(packet,numbers, this->rule);
    String copy =numbers->trim();
    String** output=nullptr;
    size_t *size=nullptr;
    const char* list = ".";
   copy.split(list,output,size);
    if(*size != 4)
    {
        return false;
    }
    class ip user;
    user.ip=((*output)[0].to_integer() <<  24) + ((*output)[1].to_integer() <<  16) +
       ((*output)[2].to_integer() <<  8) +((*output)[3].to_integer());
    user.ip = user.ip >> this->mask;
    return (user.ip == ip);
}



void ip::set_rule(String str) {
    if(str.equals("src"))
    {
        rule = (IP ::SRC);
    }
    else
    {
        rule = (IP ::DST);
    }
}