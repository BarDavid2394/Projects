class Solution {
public:
    vector<int> replaceElements(vector<int>& arr) {
      int n = arr.size();
      vector<int>::iterator it_begin = arr.begin();
      vector<int>::iterator it_end = arr.end();
      if(n == 1)
        return {-1};
      for(int i=0;i<n-1;i++){
          arr[i] = *max_element(it_begin + i + 1, it_end);
      }
      arr[n-1] = -1;
      return arr;
    }
};