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
    int list_len(ListNode* head){
        int len=0;
        while(head){
            len++;
            head=head->next;
        }
        return len;
}
    bool isPalindrome(ListNode* head) {
        int len = list_len(head);
        stack<int> stack;
        for(int i=0;i<len/2;i++){
            stack.push(head->val);
            head = head->next;
        }
        if(len%2 == 1)
            head=head->next;
        while(head){
            if(head ->val != stack.top() )
                return false;
            else{
                head=head->next;
                stack.pop();
            }
        }
        return true;
    }
};


