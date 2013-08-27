#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list {
    void * item;
    struct linked_list * next;
} linked_list_t;

#define MALLOC_TYPE(TYPE) ((TYPE *)malloc(sizeof(TYPE)))
#define COPY_ITEM(TYPE, POINTER) (&(*MALLOC_TYPE(TYPE) = *POINTER))

#define GET_ITEM(LIST, TYPE) ((TYPE *)((LIST)->item))
#define GET_MEMBER(LIST, TYPE, ITEM) (GET_ITEM(LIST, TYPE)->ITEM)
#define SET_MEMBER(LIST, TYPE, ITEM, VALUE) (GET_MEMBER(LIST, TYPE, ITEM) = VALUE)

#define NEXT(LIST) (LIST = (LIST)->next)

#define PUSH_FRONT(LIST, ITEM) ({ \
    linked_list_t * old_head = LIST; \
    LIST = MALLOC_TYPE(linked_list_t); \
    (LIST)->next = old_head; \
    (LIST)->item = ITEM; \
})

#define PUSH_BEHIND(BEFORE, ITEM) ({ \
    linked_list_t * self = MALLOC_TYPE(linked_list_t); \
    self->next = BEFORE->next; \
    self->item = ITEM; \
    BEFORE->next = self; \
})

#define POP_HEAD(LIST) ({ \
    linked_list_t * old_head = LIST; \
    NEXT(LIST); \
    free(old_head->item); \
    free(old_head); \
})

#endif
