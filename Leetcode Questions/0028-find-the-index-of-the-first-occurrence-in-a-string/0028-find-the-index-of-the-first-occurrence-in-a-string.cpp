class Solution {
public:
    int strStr(string haystack, string needle) {
        int res = -1;
        for(int i=0;i<haystack.size();i++){
            if(haystack.substr(i,needle.size()) == needle){
                return i;
               
            }
        }
        return res;
    }
};