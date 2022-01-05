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

//Leetcode 19 删除链表的倒数第N个结点
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

//Leetcode 876 链表的中间节点
ListNode* middleNode(ListNode* head) {
    auto *fast = head, *low = head;
    while (fast != nullptr && fast->next != nullptr)
    {
        fast = fast->next->next;
        low = low->next;
    }
    return low;
}

//Leetcode 141 环形链表
bool hasCycle(ListNode *head) {
    ListNode* fast = head, *slow = head;
    while(fast != nullptr && fast->next != nullptr)
    {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow) return true;
    }
    return false;
}

//Leetcode 142 环形链表2
ListNode *detectCycle(ListNode *head) {
    ListNode* fast = head, *slow = head;
    while(fast != nullptr && fast->next != nullptr)
    {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow) break;
    }
    if (!fast || !fast->next) return nullptr;
    slow = head;
    while(slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

//Leetcode 160 相交链表
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode *p1 = headA, *p2 = headB;
    while (p1 != p2) {
        if (p1 == nullptr) p1 = headB;
        else p1 = p1->next;
        if (p2 == nullptr) p2 = headA;
        else p2 = p2->next;
    }
    return p1;
}

//Leetcode 206 翻转链表
ListNode* reverseList(ListNode* head) {
    if(head == nullptr || head->next == nullptr) return head;
    ListNode *last = reverseList(head->next);
    head->next->next = head;
    head->next = nullptr;
    return last;
}

//翻转链表前N个节点
ListNode *successor = nullptr;
ListNode* reverseN(ListNode* head, int n)
{
    if (n == 1) 
    {
        successor = head->next;
        return head;
    }

    ListNode *last = reverseN(head->next, n-1);
    head->next->next = head;
    head->next = successor;
    return last;
}

//Leetcode 92 翻转链表2
ListNode *successor = nullptr;
ListNode* reverseN(ListNode* head, int n)
{
    if (n == 1) 
    {
        successor = head->next;
        return head;
    }

    ListNode *last = reverseN(head->next, n-1);
    head->next->next = head;
    head->next = successor;
    return last;
}
ListNode* reverseBetween(ListNode* head, int left, int right) {
    if (left == 1) return reverseN(head, right);
    head->next = reverseBetween(head->next, left-1, right-1);
    return head;
}