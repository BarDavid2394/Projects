class Solution {
public:
    bool palindrome(string s, int begin, int end){
    while(begin<end){
        if(s[begin++] != s[end--])
            return false; 
    }
    return true;
}
    bool validPalindrome(string s) {
        int begin = 0;
        int end = s.size()-1;
        if(palindrome(s,begin,end))
            return true;
        while(begin < end){
            if(s[begin] == s[end]){
                begin++;
                end--;
            }
            else return (palindrome(s,begin +1,end) || palindrome(s,begin,end-1));
        }
        return true;     
    }
};    
        // for(int i=0;i<s.size();i++){   IT GETS MEMEORY LIMIT EXCEEDED FOR LONG INPUTS
        //     string temp = s; 
        //     temp.erase(i,1);
        //     int end_temp = temp.size()-1;
        //     if(palindrome(temp,begin,end_temp))
        //         return true;
        // }

   // aux_function that checks if string is palindrom
// 1. we check if the original can be a palindrome
// 2. we'll itearte through the string and we will remove a char each time. then another check. of course it will occur on the string copy so we will not change the original string

