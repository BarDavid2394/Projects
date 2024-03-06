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
    ListNode* reverseList(ListNode* head) {
      ListNode *nextNode,*prevNode = NULL;
      while(head){
          nextNode = head->next;
          head->next = prevNode;
          prevNode = head;
          head = nextNode;
      }  
      return prevNode;
    }
};

//naive sol - creat a new linked list and insert each value from the top. O(n) time and space.

