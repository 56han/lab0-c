#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    element_t *n, *s;
    list_for_each_entry_safe (n, s, head, list)
        q_release_element(n);

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;
    }
    // new_element->value = strdup(s);  // strdup 自動分配內存 複製字符串

    new_element->value = malloc(strlen(s) + 1);
    if (!new_element->value) {
        free(new_element);
        return false;
    }

    strncpy(new_element->value, s, strlen(s));
    new_element->value[strlen(s)] = '\0';  // 确保字符串终止

    list_add(&new_element->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;
    }
    new_element->value =
        strdup(s);  // strdup 複製字符串，生命週期會和new_element相同
    if (!new_element->value) {  //字串複製失敗
        free(new_element);
        return false;
    }

    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {  // head是否為NULL 或 list是否為空
        return NULL;
    }
    element_t *n = list_first_entry(head, element_t, list);
    list_del(head->next);

    if (sp != NULL) {
        strncpy(sp, n->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return n;  //回傳 head
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *n = list_last_entry(head, element_t, list);
    list_del(head->prev);

    if (sp != NULL) {
        strncpy(sp, n->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return n;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *slow = head->next;

    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next) {
        slow = slow->next;
    }
    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || head->next == head)
        return false;
    struct list_head *node;
    bool flag = false;

    for (node = head->next; node->next != head;) {
        element_t *current = list_entry(node, element_t, list);
        element_t *next = list_entry(node->next, element_t, list);

        if (strcmp(current->value, next->value) == 0) {
            list_del(&next->list);
            q_release_element(next);
            flag = true;
        } else {
            if (flag) {
                // 刪除當前節點
                struct list_head *temp = node->next;  // 保存下一個節點的地址
                list_del(node);
                q_release_element(current);
                node = temp;  // 更新node為下一個節點
                flag = false;
            } else {
                node = node->next;
            }
            // flag = false;
        }
    }
    // 處理最後有重複的節點
    if (flag) {
        element_t *c = list_entry(node, element_t, list);
        list_del(node);
        q_release_element(c);
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    struct list_head *node;

    for (node = head->next; (node->next != head) && (node != head);
         node = node->next) {
        struct list_head *next = node->next;
        list_del(node);
        list_add(node, next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *current, *s;
    list_for_each_safe (current, s, head)
        list_move(current, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;

    struct list_head *it, *safe, *cut;
    int count = k;
    cut = head;
    list_for_each_safe (it, safe, head) {
        if (--count)
            continue;
        LIST_HEAD(tmp);
        count = k;
        list_cut_position(&tmp, cut, it);
        q_reverse(&tmp);
        list_splice(&tmp, cut);
        cut = safe->prev;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    // if (!head)
    //     return;

    // if (descend == false) {  // ascend

    // } else {
    // }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->next;
    while (cur != head) {  // 外层循环遍历链表
        struct list_head *next_for_comparison = cur->next;
        bool should_delete = false;

        while (next_for_comparison !=
               head) {  // 内层循环遍历当前节点右侧的所有节点
            element_t *cur_element = list_entry(cur, element_t, list);
            element_t *next_element =
                list_entry(next_for_comparison, element_t, list);

            if (strcmp(cur_element->value, next_element->value) > 0) {
                should_delete = true;
                break;  // 找到一个严格更小的值，当前节点需要被删除
            }

            next_for_comparison = next_for_comparison->next;
        }

        struct list_head *next = cur->next;  // 保存当前节点的下一个节点
        if (should_delete) {
            list_del(cur);  // 从链表中删除当前节点
            element_t *to_delete = list_entry(cur, element_t, list);
            q_release_element(to_delete);  // 释放节点资源
        }

        cur = next;  // 移动到下一个节点
    }

    return q_size(head);
}



/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->next;
    while (cur != head) {  // 外层循环遍历链表
        struct list_head *next_for_comparison = cur->next;
        bool should_delete = false;

        while (next_for_comparison !=
               head) {  // 内层循环遍历当前节点右侧的所有节点
            element_t *cur_element = list_entry(cur, element_t, list);
            element_t *next_element =
                list_entry(next_for_comparison, element_t, list);

            if (strcmp(cur_element->value, next_element->value) < 0) {
                should_delete = true;
                break;  // 找到一个严格更小的值，当前节点需要被删除
            }

            next_for_comparison = next_for_comparison->next;
        }

        struct list_head *next = cur->next;  // 保存当前节点的下一个节点
        if (should_delete) {
            list_del(cur);  // 从链表中删除当前节点
            element_t *to_delete = list_entry(cur, element_t, list);
            q_release_element(to_delete);  // 释放节点资源
        }

        cur = next;  // 移动到下一个节点
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
