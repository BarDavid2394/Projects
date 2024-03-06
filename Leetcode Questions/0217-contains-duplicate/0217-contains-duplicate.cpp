class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
     //solution - inserting the elements one by one to a hashtable. before inserting i'll search. if i found it, return true. if i went through the whole vector and still didn't find any, i'll return false.
     unordered_map<int,int> numbers;
    while(!nums.empty()){
        int temp = nums.back();
        nums.pop_back();
        if(numbers.count(temp))
        // if(numbers.find(temp) != numbers.end()){ // if the elemtns exists, return true.
            return true;
        // }
        numbers[temp] = temp;
    }
    return false;

        
    }
};
//O(n) time, O(n) space.