/**
ListNode library, for linked lists
*/

#ifndef LIST_H
#define LIST_H

typedef struct SerialListNode_t ListNode;
typedef ListNode LinkedList;

/*
 * struct ListNode_t
 *
 * Data structure for storing the data about the points.
 *
 * Implemented as a circular doubly-linked list.
 *
 * removed - the logical remove flag
 * point - point being represented
 * prev - the previous node
 * next - the next node
 */
struct SerialListNode_t{
    bool removed;
    Point point;
    ListNode *prev, *next;
};

/*
 * LinkedList_create
 *
 * Creates and allocates a LinkedList, containing a Point object, with the removed flag
 * initialized to false.
 *
 * point - the Point to contain
 *
 * Returns a LinkedList reference.
 */
LinkedList* LinkedList_create(Point point) {
    LinkedList* node = (LinkedList*)malloc(sizeof(LinkedList));
    node->removed = false;
    node->point = point;
    node->prev = node;
    node->next = node;
    return node;
}

/*
 * LinkedList_add
 *
 * Adds a new Point to the list. Each allocated Point should be added at most once.
 *
 * list - the LinkedList to add to
 * point - the Point to add
 *
 * Returns the LinkedList that contains the point.
 */
LinkedList* LinkedList_add(LinkedList* list, Point point) {
    LinkedList* node = (LinkedList*)malloc(sizeof(LinkedList));
    node->removed = false;
    node->point = point;
    node->next = list;
    node->prev = list->prev;
    while (node->prev->removed)
        node->prev = node->prev->prev;
    node->prev->next = node;
    list->prev = node;
    return node;
}

/*
 * Linkedlist_remove
 *
 * Removes a LinkedList node.
 *
 * list - the LinkedList node to remove
 */
void LinkedList_remove(LinkedList* list) {
    list->removed = true;
    list->prev->next = list->next;
    list->next->prev = list->prev;
    free(list);
}

/*
 * LinkedList_purge
 *
 * Removes all nodes in a LinkedList.
 *
 * list - the list to purge
 */
void LinkedList_purge(LinkedList* list) {
    list->prev->next = NULL;
    while (list != null) {
        list->removed = true;
        LinkedList* curr = list;
        list = list->next;
        free(curr);
    }
}
#endif
