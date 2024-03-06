class Solution {
public:
    bool isSubsequence(string s, string t) {
        
        int i = 0;
        int j = 0;
        int counter = 0;
        while(i<t.length() && j<s.length()){
            if(t[i] == s[j]){
                counter++;
                j++;
            }
                i++;
        }
            if(counter == s.length())
                return true;
            else return false;
        }
};