//JZ6 从头到尾打印链表
vector<int> printListFromTailToHead(ListNode* head) {
    ListNode* p = head;
    vector<int> res;
    while (p) {
        res.push_back(p->val);
        p = p->next;
    }
    reverse(res.begin(), res.end());
    return res;
}

//JZ24 反转链表
ListNode* ReverseList(ListNode* pHead) {
    if (!pHead) return NULL;
    auto p1 = pHead, p2 = pHead->next;
    while (p2) {
        auto t = p2->next;
        p2->next = p1;
        p1 = p2;
        p2 = t;
    }
    pHead->next = NULL;
    return p1;
}

//JZ25 合并两个排序链表
ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
{
    ListNode* dummy = new ListNode(-1);
    auto p = dummy;
    while (pHead1 && pHead2)
    {
        if (pHead1->val < pHead2->val) p->next = pHead1, p = p->next, pHead1 = pHead1->next;
        else p->next = pHead2, p = p->next, pHead2 = pHead2->next;
    }
    while (pHead1) p->next = pHead1, p = p->next, pHead1 = pHead1->next;
    while (pHead2) p->next = pHead2, p = p->next, pHead2 = pHead2->next;
    return dummy->next;
}

//JZ52 两个链表的第一个公共结点
ListNode* FindFirstCommonNode( ListNode* pHead1, ListNode* pHead2) {
    if (!pHead1 || !pHead2) return NULL;
    auto p1 = pHead1, p2 = pHead2;
    while (p1 != p2) {
        if (p1) p1 = p1->next;
        else p1 = pHead2;
        if (p2) p2 = p2->next;
        else p2 = pHead1;
    }
    return p1;
}

//JZ23 链表中环的入口结点
ListNode* EntryNodeOfLoop(ListNode* pHead)
{
    if (!pHead || !pHead->next) return nullptr;
    auto p1 = pHead, p2 = pHead;
    while (p1 && p2) {
        p1 = p1->next;
        p2 = p2->next;
        if (p2) p2 = p2->next;
        if (p1 == p2) {
            p1 = pHead;
            while (p1 != p2) {
                p1 = p1->next;
                p2 = p2->next;
            }
            return p2;
        }
    }
}

//JZ35 复杂链表的复制
RandomListNode* Clone(RandomListNode* pHead)
{
    auto p = pHead;
    while (p) {
        auto next = p->next;
        auto temp = new RandomListNode(p->label);
        p->next = temp;
        temp->next = next;
        p = next;
    }
    p = pHead;
    while (p) {
        p->next->random = p->random == nullptr ? nullptr : p->random->next;
        p = p->next->next;
    }
    auto dummy = new RandomListNode(-1);
    auto cur = dummy;
    p = pHead;
    while (p) {
        auto pre = p;
        cur->next = p->next;
        cur = cur->next;
        p = p->next->next;
        pre->next = p;
    }
    return dummy->next;
}

//JZ76 删除链表中重复的结点
ListNode* deleteDuplication(ListNode* pHead)
{
    auto dummy = new ListNode(-1);
    dummy->next = pHead;
    auto p = dummy;
    while (p->next) {
        auto q = p->next;
        while (q && q->val == p->next->val) q = q->next;
        if (p->next->next == q) p = p->next;
        else p->next = q;
    }
    return dummy->next;
}