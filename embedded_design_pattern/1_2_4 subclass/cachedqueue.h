#ifndef __CACHEDQUEUE_H__
#define __CACHEDQUEUE_H__

#include "queue.h"

typedef struct CachedQueue CachedQueue;
struct CachedQueue
{
    /* 基类 */
    Queue *queue;

    /* 新属性 */
    char filename[80];
    int numberElementOnDisk;

    /* 聚合虚函数 */
    Queue *outputQueue;

    /* 继承虚函数 */
    int (*isFull)(CachedQueue *const me);
    int (*isEmpty)(CachedQueue *const me);
    int (*getSize)(CachedQueue *const me);
    void (*insert)(CachedQueue *const me, int k);
    int (*remove)(CachedQueue *const me);

    /* 新虚函数 */
    void (*flush)(CachedQueue *const me);
    void (*load)(CachedQueue *const me);
};

/* Constructors and destructors */
void CachedQueue_Init(CachedQueue *const me, char *fName,
                      int (*isFullfunction)(CachedQueue *const me),
                      int (*isEmptyfunction)(CachedQueue *const me),
                      int (*getSizefunction)(CachedQueue *const me),
                      void (*insertfunction)(CachedQueue *const me, int k),
                      int (*removefunction)(CachedQueue *const me),
                      void (*flushfunction)(CachedQueue *const me),
                      void (*loadfunction)(CachedQueue *const me));

void CachedQueue_Cleanup(CachedQueue* const me);


/* Operations */
int CachedQueue_isFull(CachedQueue *const me);
int CachedQueue_isEmpty(CachedQueue *const me);
int CachedQueue_getSize(CachedQueue *const me);
void CachedQueue_insert(CachedQueue *const me, int k);
int CachedQueue_remove(CachedQueue *const me);
void CachedQueue_flush(CachedQueue *const me);
void CachedQueue_load(CachedQueue *const me);

CachedQueue* CachedQueue_Create(void);
void CachedQueue__Destory(CachedQueue* const me);

#endif // CACHEDQUEUE_H__