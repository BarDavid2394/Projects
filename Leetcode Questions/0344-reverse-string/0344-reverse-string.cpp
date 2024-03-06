// class Solution {
// public:
//     void reverseString(vector<char>& s) {
//         for(int i=s.size()-1;i>=0;i--){
//             s.push_back(s[i]);
//         }
//         for(int j=0;j<s.size()-1;j++){
//             s.erase(s.begin());
//         }
//     }
// }; nice solution but the runtime is slow.

//genius solution:
class Solution {
public:
    void reverseString(vector<char>& s) {
        int i = 0, j = s.size() - 1;
        while(i < j){
            swap(s[i++], s[j--]); 
        }
    }
};