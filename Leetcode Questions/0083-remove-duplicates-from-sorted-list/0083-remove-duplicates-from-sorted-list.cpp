/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* tail = head;
        while(tail && tail->next){
            if(tail->val == tail->next->val){
                tail->next = tail->next->next;
                continue;
            }
        tail = tail->next;  
        }
        return head;
    }
};