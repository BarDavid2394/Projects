// class Solution {
// public:
//     int removeDuplicates(vector<int>& nums) {
//         int k = 0;
//         int n=nums.size();
//        for(int i=0;i<n;i++){
//            int runner = i+1;
//            while(nums[i] == nums[runner]){
//                nums.erase(std::next(nums.begin(),runner));
//                k++;
//                runner++;
//            }
//        } 
//        nums.resize(nums.size()+k);
//        return nums.size()-k;
//     }
// };
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        
        if (n <= 1) {
            return n;
        }
        
        int j = 0; // Pointer to the last unique element
        
        for (int i = 1; i < n; i++) {
            if (nums[i] != nums[j]) {
                j++;
                nums[j] = nums[i]; // Move the unique element to position j
            }
        }
        
        return j + 1; // Return the number of unique elements (k)
    }
};
