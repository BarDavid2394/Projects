class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int>sum;
        int n = nums.size();
        for(int i=0;i<n;i++){
            sum[nums[i]] = i;
        }
        for(int i=0;i<n;i++){
            int complete = target - nums[i];
            if(sum.count(complete) && sum[complete] !=i){
                return{i, sum[complete]};
            }
        }  
     return {}; 
    }
};      

//two while loops on the vector elements. when target is met, pushing the 2 indexes/