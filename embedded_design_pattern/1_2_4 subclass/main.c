#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "cachedqueue.h"

void test_queue(void);
void test_cachedQueue(void);

int main(void)
{
    test_queue();
    test_cachedQueue();
    return EXIT_SUCCESS;
}

void test_queue(void)
{
    int j, k, h, t;

    Queue* myQ;
    myQ = Queue_Create();
    k = 1000;

    for (j = 0; j < QUEUE_SIZE; j++)
    {
        h = myQ->head;
        myQ->insert(myQ, k);
        printf("inserting %d at position %d, size = %d\n", k--, h, myQ->getSize(myQ));
    }

    for (j = 0; j < QUEUE_SIZE; j++)
    {
        t = myQ->tail;
        k = myQ->remove(myQ);
        printf("removing %d at position %d, size = %d\n", k, t, myQ->getSize(myQ));
    }

    Queue_Destroy(myQ);
}

void test_cachedQueue(void)
{

}