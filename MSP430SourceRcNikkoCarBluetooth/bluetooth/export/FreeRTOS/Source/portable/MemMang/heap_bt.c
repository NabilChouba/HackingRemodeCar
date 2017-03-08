
/**
 * Copyright (c) 2009 MindTree Ltd. All rights reserved.
 * \file    heap_bt.c
 * \brief   This file contains functions used for Bluetooth buffer management
 *          and is not expected to be modified by the end user.
 */

#include <string.h>
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "bt_sdk_error.h"
#include "BT_buffer.h"

/* Maximum Stack items */
#define     OS_MAX_STACK_ITEMS      15

/* Define the max pools for the allocation */
#define OS_MAX_POOLS                5

/* Define the number of pools for each type of memory block */
#define BT_NUM_SIZE_BLK_8           4
#define BT_NUM_SIZE_BLK_16          2
#define BT_NUM_SIZE_BLK_32          4
#define BT_NUM_SIZE_BLK_128         2
#define BT_NUM_SIZE_BLK_260         2

/* Maximum Heap Memory for the buffer management */
#define     OS_MAX_HEAP_MEMORY      \
            ((BT_MEM_SIZE_8*BT_NUM_SIZE_BLK_8) + \
             (BT_MEM_SIZE_16*BT_NUM_SIZE_BLK_16) + \
             (BT_MEM_SIZE_32*BT_NUM_SIZE_BLK_32) + \
             (BT_MEM_SIZE_128*BT_NUM_SIZE_BLK_128) + \
             (BT_MEM_SIZE_260*BT_NUM_SIZE_BLK_260) + \
             (4 * (BT_NUM_SIZE_BLK_8+BT_NUM_SIZE_BLK_16+BT_NUM_SIZE_BLK_32+BT_NUM_SIZE_BLK_128+BT_NUM_SIZE_BLK_260)) + \
              2)

static UCHAR heap_memory[OS_MAX_HEAP_MEMORY];

void sdk_error_handler(void);

typedef struct os_stack {
    OS_ADDRESS *item;
    UINT8 num_items;
    INT8 top;
} OS_STACK;


/* Function to initialize the stack */
OS_STATUS OS_stack_init(OS_STACK * stack, UINT32 num_items);

/* Function to push the item to the stack */
OS_STATUS OS_stack_push(OS_STACK * stack, OS_ADDRESS item);

/* Function to pop the item from the stack */
OS_STATUS OS_stack_pop(OS_STACK * stack, OS_ADDRESS * item_ptr);

/* Function to search the required item in the stack */
OS_STATUS OS_stack_search_item(OS_STACK * stack, OS_ADDRESS item,
                               OS_BOOL * result);

OS_HANDLE EtherMind_Pool[BT_POOL_MAX];

/* Function to initialize the allocated buffers */
void buffer_mgmt_init(void);

/* Function to vreate the buffer pool */
OS_HANDLE OS_create_pool(UINT16 chunk_size, UINT16 num_chunks);

/* Function to allocate the memory from the buffer pool */
void *os_malloc(UINT32 bytes);

/* Function to de allocate the allocated memory */
void os_free(void *);


/* Structure for the buffer pool */
typedef struct os_pool {
    OS_ADDRESS start_ptr;
    OS_STACK free_stack;
    UINT16 chunk_size;
    UINT8 num_chunks;
    UINT8 free_pools;
} OS_POOL;

typedef struct os_pool_mgr {
    OS_POOL pool_array[OS_MAX_POOLS];
    UINT32 num_pools;
} OS_POOL_MGR;

static OS_POOL_MGR pool_mgr;

/* Pointer for the heap memory */
static unsigned int heap_pointer = 0;

extern UINT32 sdk_error_code;

/**
 * \fn      OS_create_pool
 * \brief   This function creates a buffer pool of size "num_chunks"
 *          unit(s) where size of one unit is "chunk_size". All
 *          subsequent calls to allocate memory from this pool return a
 *          buffer of size "chunk_size". The function allocates memory
 *          dynamically and initialises its free address stack and
 *          for the pool returns a handle to the pool.
 * \param   chunk_size      Size of unit
 * \param   num_chunks      Number of Chunks
 * \return  OS_HANDLE       handle to the pool
*/
OS_HANDLE OS_create_pool(UINT16 chunk_size, UINT16 num_chunks)
{
    INT16 search_result = -1;
    UINT16 index = 0;
    UINT32 chunk_index = 0;
    void *free_address = NULL;

    /* Check for valid arguments */
    if ((chunk_size == 0) || (num_chunks == 0)) {
        return (-1);
    }

    /* Check for free pool */
    for (index = 0; index < OS_MAX_POOLS; index++) {
        if (pool_mgr.pool_array[index].start_ptr == NULL) {
            search_result = (INT16) index;
            break;
        }
    }

    if (search_result != OS_INVALID_VALUE) {
        if (OS_stack_init
            (&(pool_mgr.pool_array[search_result].free_stack),
             num_chunks) == ERROR) {
            return (-1);
        }

        pool_mgr.pool_array[search_result].chunk_size = chunk_size;
        pool_mgr.pool_array[search_result].num_chunks = num_chunks;
        pool_mgr.pool_array[search_result].free_pools = num_chunks;

        if ((pool_mgr.pool_array[search_result].start_ptr =
             (void *)os_malloc(chunk_size * num_chunks))
            == NULL) {
            return (-1);
        }

        free_address = pool_mgr.pool_array[search_result].start_ptr;

        for (chunk_index = 0; chunk_index < num_chunks; chunk_index++) {
            if (OS_stack_push
                (&(pool_mgr.pool_array[search_result].free_stack),
                 free_address) == ERROR) {
                return (-1);
            }

            free_address = (void *)
                ((INT32) free_address + (INT32) chunk_size);
        }
    }

    return (search_result);
}

/**
 * \fn      buffer_mgmt_init
 * \brief   Initialises the free buffer pools.
 * \param   void
 * \return  void
 */
void buffer_mgmt_init(void)
{
    INT16 index = 0;

    for (index = 0; index < OS_MAX_POOLS; index++) {
        pool_mgr.pool_array[index].start_ptr = NULL;
        pool_mgr.pool_array[index].chunk_size = 0;
        pool_mgr.pool_array[index].num_chunks = 0;
        pool_mgr.pool_array[index].free_pools = 0;
    }
    pool_mgr.num_pools = 0;

}

/**
 * \fn      OS_allocate_buffer
 * \brief   This function allocates memory of one unit from a specified
 *          pool. Unit size is fixed for each pool and is assigned
 *          during pool creation. Allocation of buffer is an inexpensive
 *          operation as it involves only poping an item from the pool's
 *          stack
 * \param   pool        Handle of the pool.
 * \return  OS_ADDRESS  OK if successful, else NULL;
 */
OS_ADDRESS OS_allocate_buffer(OS_HANDLE pool)
{
    OS_ADDRESS free_address = NULL;

    /* Check if pool is valid */
    if ((pool <= OS_INVALID_HANDLE) || (pool >= OS_MAX_POOLS)) {
        return (NULL);
    }

    if (pool_mgr.pool_array[pool].start_ptr == NULL) {
        return (NULL);
    }

    if (OS_stack_pop(&(pool_mgr.pool_array[pool].free_stack), &free_address) ==
        ERROR) {
        return (NULL);
    }

    pool_mgr.pool_array[pool].free_pools--;

    return (free_address);
}

/**
 * \fn      OS_deallocate_buffer
 * \brief  This function deallocates memory from a specified pool.
 *          Deallocation of buffer is an inexpensive operation as
 *          it involves only pushing an item onto the pool's stack.
 * \param   pool        Handle of the pool.
 * \param   address     Address of the pool.
 * \return  OS_STATUS   OK if successful, else ERROR
 */
OS_STATUS OS_deallocate_buffer(OS_HANDLE pool, OS_ADDRESS address)
{
    OS_ADDRESS start_address = 0, end_address = 0, chunk_address = 0;
    UINT32 chunk_size, num_chunks;
    UINT16 chunk_index;
    OS_BOOL is_address_valid = FALSE, is_address_freed = FALSE;

    /* Check if pool is valid */
    if ((pool <= OS_INVALID_HANDLE) || (pool >= OS_MAX_POOLS)) {
        return (ERROR);
    }

    if (pool_mgr.pool_array[pool].start_ptr == NULL) {
        return (ERROR);
    }

    /* Initialise local variables */
    start_address = pool_mgr.pool_array[pool].start_ptr;
    chunk_size = pool_mgr.pool_array[pool].chunk_size;
    num_chunks = pool_mgr.pool_array[pool].num_chunks;
    end_address = (OS_ADDRESS)
        ((UINT32) start_address + chunk_size * num_chunks);

    /* Check if the specified address is in pool memory range */
    if ((address < start_address) || (address >= end_address)) {
        return (ERROR);
    }

    /* If yes, check if the specified address is on a chunk boundary */
    for (chunk_index = 0; chunk_index < num_chunks; chunk_index++) {
        chunk_address = (OS_ADDRESS)
            ((UINT32) start_address + chunk_index * chunk_size);

        if (chunk_address == address) {
            is_address_valid = TRUE;
            break;
        }
    }

    if (is_address_valid == FALSE) {
        return (ERROR);
    }

    /*
     * The address belongs to the pool and is on its chunk boundary.
     * If its not on the free list, then only we should free it.
     * To make that check, we go down our free stack searching for this
     * address.
     */
    if (OS_stack_search_item
        (&(pool_mgr.pool_array[pool].free_stack), address,
         &is_address_freed) == ERROR) {
        return (ERROR);
    }

    if (is_address_freed == TRUE) {
        return (OK);
    }

    /*
     * The specified address is a valid address of the pool and is
     * not part of free list of addresses belonging to the pool, so we can
     * free it.
     */

    /* Zero out the memory location */
    memset(address, 0, pool_mgr.pool_array[pool].chunk_size);
    /* Put it in the free list */
    if (OS_stack_push(&(pool_mgr.pool_array[pool].free_stack), address)
        == ERROR) {
        return (ERROR);
    }

    pool_mgr.pool_array[pool].free_pools++;

    return (OK);
}

/**
 * \fn      os_malloc
 * \brief  This provides interface for malloc on platforms where malloc
 *          is not available. It actually allcoates memory from a
 *          statically allocated buffer. It is assumed that the memory
 *          is not freed dynamically
 * \param   bytes Number of bytes of memory required.
 * \return  void
 */
void *os_malloc(UINT32 bytes)
{
    void *free_address = NULL;

    /* Check for valid arguments */
    if (bytes == 0) {
        return (NULL);
    }

    if (heap_pointer + bytes < OS_MAX_HEAP_MEMORY) {
        free_address = (void *)(&heap_memory[heap_pointer]);
        heap_pointer += bytes;
    } else {
        sdk_error_code = SDK_POOL_MEM_ALLOC_FAIL;
        sdk_error_handler();
    }

    return (free_address);
}

/**
 * \fn      OS_stack_init
 * \brief   Initialises a given stack
 * \param   stack Pointer to the stack
 * \param   num_times stack size
 * \return  OS_STATUS OK if successful, else ERROR
 */
OS_STATUS OS_stack_init(OS_STACK * stack, UINT32 num_items)
{
    /* Check for valid arguments */
    if (stack == NULL) {
        return (ERROR);
    }

    if (num_items > OS_MAX_STACK_ITEMS) {
        return (ERROR);
    }

    if ((stack->item = (OS_ADDRESS *) os_malloc(sizeof(OS_ADDRESS) * num_items))
        == NULL) {
        return (ERROR);
    }

    stack->num_items = num_items;
    stack->top = OS_INVALID_VALUE;

    return (OK);
}


/**
 * \fn      OS_stack_push
 * \brief   Pushes an "item" on the "stack"
 * \param   stack       Pointer to the stack
 * \param   item        item to be pushed
 * \return  OS_STATUS   OK if successful, else ERROR
 */
OS_STATUS OS_stack_push(OS_STACK * stack, OS_ADDRESS item)
{

    /* check for valid arguments */
    if (stack == NULL) {
        return (ERROR);
    }

    if (item == NULL) {
        return (ERROR);
    }

    /* check if stack is full */
    if (stack->top >= ((INT32) stack->num_items - 1)) {
        return (ERROR);
    }

    /*
     * Increment stack top pointer
     * NOTE  Top pointer points to the most recently occupied entry.
     */
    stack->top++;
    /* push item on the stack */
    stack->item[stack->top] = item;

    return (OK);
}

/**
 * \fn      OS_stack_pop
 * \brief   Pops an "item" off the "stack".
 * \param   stack           Pointer to the stack
 * \param   item_ptr        address of the item to be popped
 * \return  OS_STATUS       OK if successful, else ERROR
 */
OS_STATUS OS_stack_pop(OS_STACK * stack, OS_ADDRESS * item_ptr)
{

    /* check for valid arguments */
    if (stack == NULL) {
        return (ERROR);
    }

    if (item_ptr == NULL) {
        return (ERROR);
    }

    /* check if stack is empty */
    if (stack->top <= OS_INVALID_VALUE) {
        return (ERROR);
    }

    /* Pop item from the stack */
    *item_ptr = stack->item[stack->top];

    /* Decrement the stack pointer */
    stack->top--;

    return (OK);
}

/**
 * \fn      OS_stack_search_item
 * \brief   Searches the specified item in the stack. This function does
 *           not modify the stack.
 * \param   stack       Pointer to the stack
 * \param   item        address of the item to be searched
 * \param   result      pointer which stores the result of the search
 * \return  OS_STATUS   OK if successful, else ERROR
 */
OS_STATUS OS_stack_search_item(OS_STACK * stack, OS_ADDRESS item,
                               OS_BOOL * result)
{
    INT32 index = 0;
    OS_BOOL found = FALSE;

    /* check for valid arguments */
    if (stack == NULL) {
        return (ERROR);
    }

    for (index = 0; index <= stack->top; index++) {
        if (stack->item[index] == item) {
            found = TRUE;
            break;
        }
    }

    *result = found;

    return (OK);
}

/**
 * \fn      init_bt_buffer_pools
 * \brief   Allocates memory for BT buffer pools
 * \param   void
 * \return  void
 */
void init_bt_buffer_pools(void)
{
    UCHAR index;

    buffer_mgmt_init();     /* Initialises the free buffer pools */

    for (index = 0; index < BT_POOL_MAX; index++) {
        EtherMind_Pool[index] = 0;
    }

    heap_pointer = 0;
    if (((UINT32) (&heap_memory) & BIT0)) {
        heap_pointer += 1;
    }

    EtherMind_Pool[BT_POOL_8] =
        OS_create_pool(BT_MEM_SIZE_8, BT_NUM_SIZE_BLK_8);

    EtherMind_Pool[BT_POOL_16] =
        OS_create_pool(BT_MEM_SIZE_16, BT_NUM_SIZE_BLK_16);

    EtherMind_Pool[BT_POOL_32] =
        OS_create_pool(BT_MEM_SIZE_32, BT_NUM_SIZE_BLK_32);

    EtherMind_Pool[BT_POOL_128] =
        OS_create_pool(BT_MEM_SIZE_128, BT_NUM_SIZE_BLK_128);

    EtherMind_Pool[BT_POOL_260] =
        OS_create_pool(BT_MEM_SIZE_260, BT_NUM_SIZE_BLK_260);

}
