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
    ListNode* middleNode(ListNode* head) {
     ListNode* p = head;
     int size = 0;
     while(p){
         size++;
         p = p->next;
     }  
    for(int i=0;i<size/2;i++){
        head=head->next;
    } 
    return head;
    }
};

//other option - run on the linked list with fast and slow when fast does 2 steps each time. 
// ListNode* middleNode(ListNode* head) 
//     {
//         if(head == NULL)
//             return head;
//         ListNode* slow = head;
//         ListNode* fast = head;
//         while(fast != NULL && fast -> next != NULL)
//         {
//             slow = slow -> next;
//             fast = fast -> next -> next;
//         }
//         return slow;
// 	}