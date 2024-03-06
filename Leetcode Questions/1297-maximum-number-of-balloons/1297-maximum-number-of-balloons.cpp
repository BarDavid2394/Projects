class Solution {
public:
    int maxNumberOfBalloons(string text) {
    unordered_map<char,int> letters;
      for(char c : text){
          letters[c]++;
      }
      int min_count = min({letters['b'],letters['a']
      ,letters['l']/2,letters['o']/2,letters['n']});
      return min_count;
    }
};