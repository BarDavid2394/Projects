#include "string.h"
#include <bitset>
#include "ip.h"
#include <cstring>

void split_argument(String packet, String *numbers, IP::Rule rule);

ip::ip() {
    this->mask = 0;
    rule = IP::SRC;
    ip_num = 0;
}

ip::ip(const ip &ip_ad) {
    mask = ip_ad.mask;
    rule = ip_ad.rule;
    ip_num = ip_ad.ip_num;
}
//aux function that helps us to split the input and determine if its src \ dst.
void split_argument(String packet, String *numbers, IP::Rule rule) {
    String *output;
    size_t arg_size = 0;
    packet.split(",=", &output, &arg_size);
    if (arg_size == 0) {
        return;
    }
    if (rule == (IP::SRC)) {
        for(int i=0;i<8;i+=2)
        {
            if((output[i].equals("src-ip")))
            {
                *numbers = output[i+1].trim();
            }
        }
    }
    if (rule == (IP::DST)) {
        for(int i=0;i<8;i+=2)
        {
            if(output[i].equals("dst-ip"))
            {
                *numbers = output[i+1].trim();
            }
        }
    }
    delete[] output;
}

bool ip::set_value(String value) {
    String copy = value.trim();
    String *output;
    size_t size = 0;
    copy.split("./", &output, &size);
    if (size != 5) {
        if (size > 0) {
            delete[] output;
        }
        return false;
    }
    ip_num = ((output)[0].to_integer() << 24)
            | ((output)[1].to_integer() << 16)
            |((output)[2].to_integer() << 8)
            | ((output)[3].to_integer());
    mask = (output)[4].to_integer();
    delete[] output;
    return true;
}

bool ip::match(String packet) {
    String numbers = String();
    split_argument(packet, &numbers, this->rule);

    String copy = numbers.trim();
    String *output;
    size_t size = 0;
    copy.split(".", &output, &size);
    if (size != 4) {
        if (size > 0) {
            delete[] output;
        }
        return false;
    }
    ip user = ip();
    user.ip_num = ((output)[0].to_integer() << 24)
            | ((output)[1].to_integer() << 16)
            |((output)[2].to_integer() << 8)
            | ((output)[3].to_integer());
//using bitset to compare the rule and the input bit by bit
    std::bitset<32> rule_bit = std::bitset<32>(this->ip_num);
    std::bitset<32> packet_bit = std::bitset<32>(user.ip_num);
    //taking the mask into consideration. MSB is the last cell in the array
    for (int i = 31; i >= 32 - mask; i--) {
        if (rule_bit[i] != packet_bit[i]) {
            delete[] output;
            return false;
        }
    }
    delete[] output;
    return true;
}
//decides if the rule is for the src\dst field.
void ip::set_rule(String str) {
    if (str.equals("src")) {
        rule = (IP::SRC);
    } else {
        rule = (IP::DST);
    }
}