class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> map;
        stack<int> ans;
        for( int num : nums2){
            int element;
            while(!ans.empty() && num > ans.top()){
                element = ans.top();
                ans.pop();
                map[element] = num;
            }
            ans.push(num);
        } 
        for(int& num1 : nums1){
            if(map.count(num1))
                num1 = map[num1];
            else
            num1 = -1;
        }
        return nums1;
    }
};

