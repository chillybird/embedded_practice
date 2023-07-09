#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* constructors and destructors*/
void Queue_Init(Queue *const me, int (*isFullfunction)(Queue *const me),
                int (*isEmptyfunction)(Queue *const me),
                int (*getSizefunction)(Queue *const me),
                void (*insertfunction)(Queue *const me, int k),
                int (*removefunction)(Queue *const me))
{
    me->head = 0;
    me->size = 0;
    me->tail = 0;
    me->isFull = isFullfunction;
    me->isEmpty = isEmptyfunction;
    me->getSize = getSizefunction;
    me->insert = insertfunction;
    me->remove = removefunction;
}

void Queue_Cleanup(Queue *const me) {}

/* operations */
int Queue_isFull(Queue *const me)
{
    return me->size == QUEUE_SIZE;
    // return (me->head + 1) % QUEUE_SIZE == me->tail;
}

int Queue_isEmpty(Queue *const me)
{
    return me->size == 0;
    // return (me->head == me->tail);
}

int Queue_getSize(Queue *const me)
{
    return me->size;
}

void Queue_insert(Queue *const me, int k)
{
    if (!me->isFull(me))
    {
        me->buffer[me->head] = k;
        me->head = (me->head + 1) % QUEUE_SIZE;
        ++me->size;
    }
}

int Queue_remove(Queue *const me)
{
    int value = -999; // 哨兵值
    if (!me->isEmpty(me))
    {
        value = me->buffer[me->tail];
        me->tail = (me->tail + 1) % QUEUE_SIZE;
        --me->size;
    }
    return value;
}

Queue *Queue_Create(void)
{
    Queue *me = (Queue *)malloc(sizeof(Queue));
    if (me != NULL)
    {
        Queue_Init(me, Queue_isFull, Queue_isEmpty, Queue_getSize, Queue_insert, Queue_remove);
    }
    return me;
}

void Queue_Destroy(Queue *const me)
{
    if (me != NULL)
    {
        Queue_Cleanup(me);
    }
    free(me);
}