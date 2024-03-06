class Solution {
public:
    int lengthOfLastWord(string s) {
        int n = s.length();
        while(n>0 && s[n-1] == ' '){
            n--;
        }
      
       int length = 0;
       for(int i=n-1;i>=0;i--){
           if(s[i] == ' '){
               break;
           }
        length++;
       } 
       return length;
    }
};