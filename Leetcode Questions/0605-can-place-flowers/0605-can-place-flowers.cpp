class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
     for (int i = 0; i < flowerbed.size() && n > 0; i++) {
            if (flowerbed[i] == 0 && (i == 0 || flowerbed[i-1] == 0) && (i == flowerbed.size()-1 || flowerbed[i+1] == 0)) {
                i++;
                n--;
            }
        }
        return n == 0;
    }
};
//if there are odd elements in the flowerbed, we need to check if first element is 1. then trying to put every other index.
//