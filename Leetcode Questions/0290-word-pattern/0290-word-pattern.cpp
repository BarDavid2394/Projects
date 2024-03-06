// class Solution {
// public:
//     bool wordPattern(string pattern, string s) {
//         unordered_map<char,string> map;
//         unordered_map<string, char> usedWords;
//         int index = 0;
//         int word_counter = 0;
//         for(char c : pattern){
//             string word = "";
//             while(index<s.size() && s[index] != ' ' ){
//                 word +=s[index];
//                 index++;
//                 }
//             word_counter++;
//             index++;
//             if(map.count(c)){
//                 if(word != map[c])
//                     return false;
//                 else continue;
//             }
//             if (usedWords.count(word) && c != usedWords[word]) 
//                 return false;
//             map[c] = word;
//             usedWords[word] = c;
//         }
//         if(index<s.size()-1)
//             return false;
//         return true;
//     }
// }; //hashmap of chars and strings. insert each string to a char key. if we insert and there's a different string already inside, return false.
//this is code is not working for "he" pattern and s=unit. another code that works:
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        vector<string> v;
        string word;
        stringstream iss(s);
        while (iss >> word)
            v.push_back(word);

        if (pattern.size() != v.size())
            return false;

        unordered_map<char, string> mp;
        unordered_map<string, char> m;

        for(int i=0; i<v.size(); i++){
            if(mp.find(pattern[i]) != mp.end() && mp[pattern[i]] != v[i]) 
                return false;

            else if(m.find(v[i]) != m.end() && m[v[i]] != pattern[i]) 
                return false;

            mp[pattern[i]] = v[i];
            m[v[i]] = pattern[i];
        }

        return true;
    }
};