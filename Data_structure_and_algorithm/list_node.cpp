//返回链表的倒数第k个节点
ListNode findFromEnd(ListNode head, int k) {
    ListNode p1 = head, p2 = head;
    for (int i = 0; i < k; i++) p1 = p1.next;
    while (p1 != null)
    {
        p1 = p1.next;
        p2 = p2.next;
    }
    return p2;
}

//Leetcode19 删除链表的倒数第N个结点
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode *dummy = new ListNode(-1);
    dummy->next = head;
    ListNode *tmp = findNthNode(dummy, n+1);
    tmp->next = tmp->next->next;
    return dummy->next;

}

ListNode* findNthNode(ListNode* head, int k) {
    ListNode *p1 = head, *p2 = p1;
    for (int i=0; i<k; i++) p1 = p1->next;
    while (p1)
    {
        p1 = p1->next;
        p2 = p2->next;
    }
    return p2;
}