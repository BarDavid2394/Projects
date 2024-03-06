/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
       ListNode *A = headA;
       ListNode *B = headB;
       unordered_map <ListNode*, int > mapA;
       while(A){
           mapA[A] = 1;
           A = A->next;
       }
       while(B){
           if(mapA.count(B)){       
               return B;
           }
           B = B->next;  
       }
        return 0;
    }
};