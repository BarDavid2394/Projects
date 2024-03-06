class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_map<string, int> valid_mail;
        for(string &email : emails){
          string clean_mail;
          for(char c : email){
            if(c == '@' || c == '+')
                break;
            if (c == '.')
                continue;
            clean_mail +=c;
          }  
        clean_mail += email.substr(email.find('@'));
        valid_mail[clean_mail] = 1;

        }
        // int n = emails.size();
        // for(int i=0;i<n;i++){
        //     string clean_mail;
        //     string email = emails[i];
        //     for(int j=0;j<email.length();j++){
        //         if(email[j] == '@')
        //         {
        //             clean_mail +=email.substr(j);
        //             if(!(valid_mail.find(clean_mail) != valid_mail.end())){
        //                valid_mail[clean_mail] = 1;
        //             }
        //             break; 
        //         }
        //         if(email[j] == '+'){
        //             while(j<email.length() && email[j] !='@'){
        //                 j++;
        //             }
        //         }
        //         else if(email[j] != '.'){
        //             clean_mail +=email[j];
        //         }
        //         if(email[j] == '@')
        //             j--;
        //         }
                
        //     }
        return valid_mail.size();   
    }
};
//going thorugh each string. remove unnecessary dots and delete everything between + and @. then add each string to hashtable. if the address which im trying to insert already exists, we dont promote the counter. if its not there, insert + counter++