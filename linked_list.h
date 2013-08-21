#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list {
    void * item;
    struct linked_list * next;
} linked_list_t;

#define MALLOC_TYPE(TYPE) ((TYPE *)malloc(sizeof(TYPE)))
#define COPY_ITEM(TYPE, POINTER) (&(*MALLOC_TYPE(TYPE) = *POINTER))

#define GET_ITEM(LIST, TYPE) ((TYPE *)(LIST->item))
#define GET_MEMBER(LIST, TYPE, ITEM) (GET_ITEM(LIST, TYPE)->ITEM)
#define SET_MEMBER(LIST, TYPE, ITEM, VALUE) (GET_MEMBER(LIST, TYPE, ITEM) = VALUE)

#define NEXT(LIST) (LIST = LIST->next)

static inline void push_front(linked_list_t * list, void * item) {
    linked_list_t * old_head = list;
    list = MALLOC_TYPE(linked_list_t);
    list->next = old_head;
    list->item = item;
}

static inline void push_behind(linked_list_t * pos_before, void * item) {
    linked_list_t * self = MALLOC_TYPE(linked_list_t);
    self->next = pos_before->next;
    self->item = item;
    pos_before->next = self;
}

static inline void pop_head(linked_list_t * list) {
    linked_list_t * old_head = list;
    NEXT(list);
    free(old_head->item);
    free(old_head);
}

#endif
