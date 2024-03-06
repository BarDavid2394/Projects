class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
      unordered_map<int,int> map;
      vector<int> res;
      for(int num : nums){
          map[num] = 1;
      }  
      for(int i=1;i<=nums.size();i++){
          if(!map.count(i)){
              res.push_back(i);
          }
      }
    return res;
    }
};