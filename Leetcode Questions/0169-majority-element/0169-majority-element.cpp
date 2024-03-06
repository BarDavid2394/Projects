class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> numbers;
    for( int num : nums){
        if(++numbers[num] > nums.size()/2)
        return num;
    }
    return 0;
    }
    
};