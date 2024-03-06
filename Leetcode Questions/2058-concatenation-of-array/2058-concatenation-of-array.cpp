class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        //original was for(inti=0;... i<nums.size()). problem is its always increasing and its time limit exceed (Size is bigger each etarion)
        for(int i=0;i<n;i++){
        nums.push_back(nums[i]);
        }
        return nums;
    }
};
//naive solution - run on the vector twice and push back.