#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachedqueue.h"

void CachedQueue_Init(CachedQueue *const me, char *fName,
                      int (*isFullfunction)(CachedQueue *const me),
                      int (*isEmptyfunction)(CachedQueue *const me),
                      int (*getSizefunction)(CachedQueue *const me),
                      void (*insertfunction)(CachedQueue *const me, int k),
                      int (*removefunction)(CachedQueue *const me),
                      void (*flushfunction)(CachedQueue *const me),
                      void (*loadfunction)(CachedQueue *const me))
{
    /* 初始化基类 */
    me->queue = Queue_Create();

    /* 初始化子类属性 */
    me->numberElementOnDisk = 0;
    strcpy(me->filename, fName);

    /* 初始化聚合虚函数 */
    me->outputQueue = Queue_Create();

    /* 初始化子类函数指针 */
    me->isFull = isFullfunction;
    me->isEmpty = isEmptyfunction;
    me->getSize = getSizefunction;
    me->insert = insertfunction;
    me->remove = removefunction;

    me->flush = flushfunction;
    me->load = loadfunction;
}

void CachedQueue_Cleanup(CachedQueue *const me)
{
    Queue_Cleanup(me->queue);
}

/* Operations */
int CachedQueue_isFull(CachedQueue *const me)
{
    return me->queue->isFull(me->queue) &&
           me->outputQueue->isFull(me->outputQueue);
}

int CachedQueue_isEmpty(CachedQueue *const me)
{
    return me->queue->isEmpty(me->queue) &&
           me->outputQueue->isEmpty(me->outputQueue) &&
           (me->numberElementOnDisk == 0);
}

int CachedQueue_getSize(CachedQueue *const me)
{
    return me->queue->getSize(me->queue) +
           me->outputQueue->getSize(me->outputQueue) +
           me->numberElementOnDisk;
}

void CachedQueue_insert(CachedQueue *const me, int k)
{
    if (me->queue->isFull(me->queue))
        me->flush(me); // 将队列数据全部写入文件
    me->queue->insert(me->queue, k);
}

int CachedQueue_remove(CachedQueue *const me)
{
    /* 判断移除的位置 */
    if (!me->outputQueue->isEmpty(me->outputQueue))
        return me->outputQueue->remove(me->outputQueue);
    else if (me->numberElementOnDisk > 0)
    {
        me->load(me); // 从文件加载数据到队列
        return me->queue->remove(me->queue);
    }
    else
        return me->queue->remove(me->queue);
}

/* flush: called only when queue is full and filename is valid */
void CachedQueue_flush(CachedQueue *const me)
{
    // write file I/O statements here...

    // flush algorithm
    // if file is not open, then open file
    // while not queue->isEmpty()
    //       queue->remove()
    //        write data to disk
    // endwhile
}

/* operation load: called when outputQueue is empty and filename is valid */
void CachedQueue_load(CachedQueue *const me)
{
    // write file I/O statements here...

    // load algorithm
    // while(!outputQueue->isFull() && (numberElementsOnDisk > 0))
    //     read from start of file (i.e,oldest datum)
    //     numberElementsOnDisk--;
    //     outputQueue->insert()
    //  endwhile
}

CachedQueue *CachedQueue_Create(void)
{
    CachedQueue *me = (CachedQueue *)malloc(sizeof(CachedQueue));
    if (me != NULL)
    {
        CachedQueue_Init(me, "queuebuffer.dat",
                         CachedQueue_isFull, CachedQueue_isEmpty,
                         CachedQueue_getSize, CachedQueue_insert,
                         CachedQueue_remove, CachedQueue_flush,
                         CachedQueue_load);
    }

    return me;
}

void CachedQueue__Destory(CachedQueue *const me)
{
    if (me != NULL)
    {
        CachedQueue_Cleanup(me);
    }

    free(me);
}
