class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length()) return false;
        int hist_s[26] = {0};
        int hist_t[26] = {0};
        for(int i = 0;i<s.length();i++){
            hist_s[s[i]-'a']++;
        }
        for(int j = 0;j<t.length();j++){
            hist_t[t[j]-'a']++;
        }
        for(int k=0;k<26;k++){
            if(hist_t[k] != hist_s[k]){
                return false;
            }
        }
        return true;
    }
};

//naive solution - histogram for each string, o(1) space, then adding each letter via ascii value to the arraym, then comparing each cell. o(max{s.length,t.length})

//condition for different len.
//hashtable 