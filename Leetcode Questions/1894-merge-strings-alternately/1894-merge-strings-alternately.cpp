class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        string res;
        int index = 0;
        while(index<word1.length() && index<word2.length()){
          res +=word1[index];
          res +=word2[index];
          index++;
        }
        while(index<word1.length()){
            res +=word1[index];
            index++;
        }
        while(index<word2.length()){
            res +=word2[index];
            index++;
        }
        return res;
    }    
};