#ifndef __QUEUE_H__
#define __QUEUE_H__

#define QUEUE_SIZE 10

/**
 * 成员函数指针实现类
*/
typedef struct Queue Queue;
struct Queue
{
    int buffer[QUEUE_SIZE];
    int head;
    int size;
    int tail;
    /* 成员函数指针 */
    int (*isFull)(Queue *const me);
    int (*isEmpty)(Queue *const me);
    int (*getSize)(Queue *const me);
    void (*insert)(Queue *const me, int k);
    int (*remove)(Queue *const me);
};

/* constructors and destructors*/
void Queue_Init(Queue *const me, int (*isFullfunction)(Queue *const me),
                int (*isEmptyfunction)(Queue *const me),
                int (*getSizefunction)(Queue *const me),
                void (*insertfunction)(Queue *const me, int k),
                int (*removefunction)(Queue *const me));

void Queue_Cleanup(Queue* const me);

/* operations */
int Queue_isFull(Queue *const me);
int Queue_isEmpty(Queue *const me);
int Queue_getSize(Queue *const me);
void Queue_insert(Queue *const me, int k);
int Queue_remove(Queue *const me);

Queue* Queue_Create(void);
void Queue_Destroy(Queue* const me);

#endif // QUEUE_H__