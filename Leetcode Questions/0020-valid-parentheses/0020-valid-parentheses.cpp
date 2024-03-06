class Solution {
public:
    bool isValid(string s) {
       stack<char> brackets;
       for(int i=0;i<s.size();i++){
           if(s[i] == '(' || s[i] == '{' || s[i] == '['){
               brackets.push(s[i]);
               continue;
           }
            if(s[i] == ')' || s[i] == '}' || s[i] == ']'){
                if(brackets.empty()){
                    return false;
                }
                else{
                    char top = brackets.top();
                    brackets.pop();
                        if ((s[i] == ')' && top != '(') ||
                          (s[i] == '}' && top != '{') ||
                          (s[i] == ']' && top != '[')) {
                              return false;
                        }
                        else continue;
                    // if(s[i] == brackets.top()){
                    //     brackets.pop();
                    //     continue;
                    }
            }               
        }        
       if(brackets.empty()){
            return true; 
       }
       else return false;
    }
};

//running on the string. 3 if's for each bracket using ascii sizes. worked only for the easy cases:
//for(int i=0;i<s.size();i++){
        //     if((s[i] == '(' && s[i+1] != ')')
        //         || (s[i] == '{' && s[i+1] != '}')
        //         || (s[i] == '[' && s[i+1] != ']' ))
        //         return false;
        // }
        // return true;