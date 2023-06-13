#include "string.h"
#include "generic-field.h"
#include <stddef.h>
#include <iostream>
#include <bitset>
#include "ip.h"
void split_argument(String packet, String* numbers,IP::Rule rule);

ip ::ip() {
    this->mask = 0;
    rule;
    ip_num = 0;
}
ip:: ip(const ip &ip_ad){
    mask = ip_ad.mask;
    rule = ip_ad.rule;
    ip_num = ip_ad.ip_num;

}
void split_argument(String packet, String* numbers,IP :: Rule rule)
{
    String* output;
    size_t arg_size=0;
//    const char* arg_list = ",=";
    packet.split(",=",&output,&arg_size);
    if(arg_size == 0){
        return;
    }
    if(rule == (IP ::SRC))
    {
        *numbers = output[1].trim();
    }
    if(rule == (IP ::DST))
    {
        *numbers = output[3].trim();
    }

}

bool ip ::set_value (String value)
{
    String copy =value.trim();
    String* output;
    size_t size=0;
//    const char* list = "./";
    value.split("./",&output,&size);
    if(size != 5)
    {
        return false;
    }
    ip_num=((output)[0].to_integer() <<  24) | ((output)[1].to_integer() <<  16) |
            ((output)[2].to_integer() <<  8) |((output)[3].to_integer());
    mask = (output)[4].to_integer();

    return true;
}
bool ip::match(String packet) {
    String numbers = String();
    split_argument(packet,&numbers, this->rule);

    String copy =numbers.trim();
    String* output;
    size_t size=0;
//    const char* list = ".";
   copy.split(".",&output,&size);
    if(size != 4)
    {
        return false;
    }
    ip user = ip();
    user.ip_num=((output)[0].to_integer() <<  24) | ((output)[1].to_integer() <<  16) |
       ((output)[2].to_integer() <<  8) | ((output)[3].to_integer());

    std:: bitset<32> rule_bit = std:: bitset<32> (this->ip_num);
    std :: bitset<32> packet_bit = std:: bitset<32>(user.ip_num);
    for(int i=31;i>31-mask;i--)
    {
        if(rule_bit[i] != packet_bit[i])
        {
            return false;
        }
    }
    //user.ip_num = user.ip_num >> this->mask;
//    std::cout << ip_num << std::endl;
//    std::cout << user.ip_num << std::endl;

    return true;
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