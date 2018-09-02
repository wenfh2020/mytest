#include "tlist.h"

TList::TList() : m_pNodeHead(NULL), m_pNodeTail(NULL) {

}

TList::~TList() {
}

TListNode* TList::GetHead() {
    return m_pNodeHead;
}

TListNode* TList::AddTail(TListNode* pNode) {
    if (NULL == pNode) {
        return NULL;
    }

    pNode->pNext = NULL;

    if (NULL == m_pNodeHead) {
        m_pNodeHead = m_pNodeTail = pNode;
    } else {
        m_pNodeTail->pNext = pNode;
        m_pNodeTail = pNode;
    }

    return NULL;
}

TListNode* TList::GetNext(TListNode* pNode) {
    return (NULL == pNode) ? NULL : pNode->pNext;
}

void TList::Reverse() {
    if (NULL == m_pNodeHead || NULL == m_pNodeHead->pNext) {
        return;
    }

    TListNode* pNodeTemp = m_pNodeHead;
    TListNode* pNodeTemp2 = m_pNodeHead->pNext;
    TListNode* pNodeTemp3 = NULL;

    m_pNodeTail = m_pNodeHead;
    m_pNodeTail->pNext = NULL;
    while (pNodeTemp2 != NULL) {
        pNodeTemp3 = pNodeTemp2->pNext;
        pNodeTemp2->pNext = pNodeTemp;
        pNodeTemp = pNodeTemp2;
        pNodeTemp2 = pNodeTemp3;
    }

    m_pNodeHead = pNodeTemp;
}

void TList::PrintList() {
    TListNode* pNodeTemp = m_pNodeHead;
    while (pNodeTemp != NULL) {
        printf("%d,", pNodeTemp->iData);
        pNodeTemp = pNodeTemp->pNext;
    }
    printf("\n");
}
