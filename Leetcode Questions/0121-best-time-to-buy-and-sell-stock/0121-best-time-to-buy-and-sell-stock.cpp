class Solution {
public:
    int maxProfit(vector<int>& prices) {
       int n = prices.size();
       int max_pro=0;
       int min_price = prices[0];
       for(int i=0;i<n;i++){
           min_price = min(prices[i], min_price);
           max_pro = max(max_pro, prices[i] - min_price);
           }
       return max_pro;
       } 
};

//running on the vector looking for the biggest delta between 2 elements. o(n^2).
//