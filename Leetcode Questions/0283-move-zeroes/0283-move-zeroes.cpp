// class Solution {
// public:
//     void moveZeroes(vector<int>& nums) {
//         for(int i=0;i<nums.size();i++){               
//             int index = i;
//             if(nums[i] == 0 ){
//                 while(index+1 <nums.size()){
//                     swap(nums[index],nums[index+1]);
//                     index++;
//                 }
//             }
//         }

//     }
// };

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        for(int i = 0, index = 0; i < nums.size(); i++) {
            if(nums[i] !=0) {
                swap(nums[i], nums[index]);
                index++;
            }
        }
    }
};