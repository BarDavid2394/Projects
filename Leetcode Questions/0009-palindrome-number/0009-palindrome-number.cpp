#include <string>
#include <iostream>
class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0)
            return false;
        std::string str = std::to_string(x);
        int len = str.length();
        for(int i=0;i<len/2;i++){
                if(str[i] != str[len - i - 1]){
                    return false;
                }
            }
    return true; 
    }
};