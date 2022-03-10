/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __LISTS_H__
#define __LISTS_H__

/* Bi-directional list structure */
struct bi_list_node_t {
    struct bi_list_node_t *prev;
    struct bi_list_node_t *next;
};

/* Init an empty node. */
#define BI_LIST_INIT_NODE(node) do {              \
    (node)->next = node;                          \
    (node)->prev = node;                          \
} while(0)

/* Insert a new node after (next) current. */
#define BI_LIST_INSERT_AFTER(curr, node) do {     \
    (node)->next = (curr)->next;                  \
    (node)->prev = curr;                          \
    (curr)->next->prev = node;                    \
    (curr)->next = node;                          \
} while(0)

/* Add one node into list as the tail (prev) of head. */
#define BI_LIST_INSERT_BEFORE(curr, node) do {    \
    (curr)->prev->next = node;                    \
    (node)->prev = (curr)->prev;                  \
    (curr)->prev = node;                          \
    (node)->next = curr;                          \
} while(0)

/* Remove one node from the list. */
#define BI_LIST_REMOVE_NODE(node) do       {      \
    (node)->prev->next = (node)->next;            \
    (node)->next->prev = (node)->prev;            \
} while(0)

/* Is the head empty? */
#define BI_LIST_IS_EMPTY(head)      ((head)->next == (head))

/* The node's next node */
#define BI_LIST_NEXT_NODE(node)     ((node)->next)

/* Go through each node of a list */
#define BI_LIST_FOR_EACH(node, head)              \
    for (node = (head)->next; node != head; node = (node)->next)

#endif /* __LISTS_H__ */
