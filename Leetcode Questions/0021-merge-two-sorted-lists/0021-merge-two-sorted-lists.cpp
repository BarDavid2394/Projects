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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if(list2 == NULL)
            return list1;
        if(list1 == NULL)
            return list2;
        ListNode* newhead = NULL;
        ListNode* newtail = NULL;
        if(list1->val > list2->val){ //defining the first element in the list
            newhead = list2;
            newtail = list2;
            list2 = list2->next;
        }
        else {
            newhead = list1;
            newtail = list1;
            list1 = list1->next;
        }
        while (list1 && list2){
            if(list1->val > list2->val ){
                newtail->next = list2;
                newtail = newtail->next;
                list2 = list2->next;
            }
            else{
                newtail->next = list1;
                newtail = newtail->next;
                list1 = list1->next;
            }
        }
        if(!list1){
            newtail->next = list2;
        }
        if(!list2){
            newtail->next = list1;
        }
        return newhead;
    }
 
};