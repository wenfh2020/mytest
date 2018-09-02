/* wfh - 2018-09-02
 * test for list.
 */
#include <stdio.h>

typedef struct TListNode {
    int iData;
    TListNode* pNext;
} TListNode;

class TList {
public:
    TList();
    ~TList();

    TListNode* GetHead();
    TListNode* GetNext(TListNode*);
    TListNode* AddTail(TListNode* pNode);

    void Reverse();
    void PrintList();

private:
    TListNode* m_pNodeHead;
    TListNode* m_pNodeTail;
};
