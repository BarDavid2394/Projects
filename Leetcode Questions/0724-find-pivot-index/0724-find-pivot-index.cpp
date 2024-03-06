class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        for(int i=0;i<nums.size();i++){
          int end = nums.size()-1;
          int begin = 0;
          int sum_left = 0;
          int sum_right = 0;
          while(end > i){
              sum_right +=nums[end];
              end--;
          }
         while(begin < i){
             sum_left +=nums[begin];
             begin++;
         }
         if(sum_left == sum_right)
            return i;
          }
          return -1;  
        }
        
    
};
//