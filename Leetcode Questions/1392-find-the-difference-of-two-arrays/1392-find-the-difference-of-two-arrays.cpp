class Solution {
public:
    vector<vector<int>> findDifference(vector<int>& nums1, vector<int>& nums2) {
        vector<vector<int>> answer(2);
        unordered_map<int,int> num1_map;
        unordered_map<int,int> num2_map;
        for(int num : nums1){
            num1_map[num]++;
        }  
        for(int num : nums2){
            if(num2_map[num] == 1){
                continue;
            }
            num2_map[num]++;
            if(!num1_map.count(num)){
                answer[1].push_back(num);
            }  
        }
        for(int num : nums1){
            if(num1_map[num] > 1){
                num1_map[num]--;
                continue;
            }
            if(!num2_map.count(num)){
                answer[0].push_back(num);
            }
        }
        return answer;
    }
};
           
                
            
        
