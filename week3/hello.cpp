#include <iostream>


#include <unistd.h>
#include <stddef.h>
#include <string.h>

using namespace std;

// 内存对齐要求（例如8字节）
#define ALIGNMENT 8
// 计算对齐后的尺寸
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
// 块头部和尾部的大小（假设 size_t 占 8 字节）
#define HEADER_SIZE sizeof(size_t)
#define FOOTER_SIZE sizeof(size_t)
// 合并相邻空闲块的最小剩余空间（避免分割后产生过小碎片）
#define MIN_SPLIT_SIZE (ALIGNMENT * 2)

// 空闲块链表节点结构
typedef struct free_block {
    size_t header;              // 头部：块大小 + 分配状态（最低位）
    struct free_block *next;    // 下一个空闲块
    struct free_block *prev;    // 上一个空闲块
    size_t footer;              // 尾部：与头部一致（用于合并）
} free_block;

// 全局变量：堆的起始位置和空闲链表头
static free_block *free_list_head = NULL;
static void *heap_start = NULL;

// 初始化堆：申请初始内存块
static void *init_heap(size_t size) {
    void *ptr = sbrk(0);
    if (sbrk(size) == (void*)-1) return NULL;
    return ptr;
}

// 合并当前块与相邻空闲块
static void coalesce(free_block *block) {
    size_t block_size = block->header & ~1;

    // 合并前一块
    if ((void*)block > heap_start) {
        size_t *prev_footer = (size_t*)block - 1;
        if ((*prev_footer & 1) == 0) {  // 前一块空闲
            size_t prev_size = *prev_footer & ~1;
            free_block *prev_block = (free_block*)((char*)block - prev_size - HEADER_SIZE - FOOTER_SIZE);
            // 从链表中移除前一块
            if (prev_block->prev) prev_block->prev->next = prev_block->next;
            if (prev_block->next) prev_block->next->prev = prev_block->prev;
            if (prev_block == free_list_head) free_list_head = prev_block->next;
            // 合并到当前块
            block_size += prev_size + HEADER_SIZE + FOOTER_SIZE;
            block = prev_block;
        }
    }

    // 合并后一块
    free_block *next_block = (free_block*)((char*)block + block_size + HEADER_SIZE + FOOTER_SIZE);
    if ((void*)next_block < sbrk(0)) {
        if ((next_block->header & 1) == 0) {  // 后一块空闲
            size_t next_size = next_block->header & ~1;
            // 从链表中移除后一块
            if (next_block->prev) next_block->prev->next = next_block->next;
            if (next_block->next) next_block->next->prev = next_block->prev;
            if (free_list_head == next_block) free_list_head = next_block->next;
            // 合并到当前块
            block_size += next_size + HEADER_SIZE + FOOTER_SIZE;
        }
    }

    // 更新合并后的块信息
    block->header = block_size | 0;
    block->footer = block_size | 0;
    // 将合并后的块加入链表头部
    block->next = free_list_head;
    block->prev = NULL;
    if (free_list_head) free_list_head->prev = block;
    free_list_head = block;
}

// 分配内存：核心逻辑
void *malloc(size_t size) {
    if (size == 0) return NULL;

    // 计算实际需要的总空间（包括头部、尾部、对齐）
    size_t total_size = ALIGN(size + HEADER_SIZE + FOOTER_SIZE);
    if (total_size < MIN_SPLIT_SIZE) total_size = MIN_SPLIT_SIZE;

    // 首次适配：遍历空闲链表
    free_block *current = free_list_head;
    while (current) {
        size_t current_size = current->header & ~1;
        if (current_size >= total_size) {
            // 找到足够大的块，尝试分割
            size_t remaining = current_size - total_size;
            if (remaining >= MIN_SPLIT_SIZE) {
                // 分割为已分配块和剩余空闲块
                current->header = total_size | 1;
                current->footer = total_size | 1;

                // 创建剩余的空闲块
                free_block *remaining_block = (free_block*)((char*)current + total_size + HEADER_SIZE + FOOTER_SIZE);
                remaining_block->header = remaining | 0;
                remaining_block->footer = remaining | 0;
                // 将剩余块加入链表
                remaining_block->next = current->next;
                remaining_block->prev = current->prev;
                if (current->prev) current->prev->next = remaining_block;
                if (current->next) current->next->prev = remaining_block;
                if (free_list_head == current) free_list_head = remaining_block;
            } else {
                // 直接分配整个块
                current->header |= 1;
                current->footer |= 1;
                // 从链表中移除
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (free_list_head == current) free_list_head = current->next;
            }
            // 返回用户可用地址（跳过头部）
            return (void*)((size_t*)current + 1);
        }
        current = current->next;
    }

    // 无合适块，扩展堆
    size_t extend_size = (total_size > 4096) ? total_size : 4096;
    free_block *new_block = (free_block*)sbrk(0);
    if (sbrk(extend_size) == (void*)-1) return NULL;
    new_block->header = extend_size | 0;
    new_block->footer = extend_size | 0;
    coalesce(new_block);  // 合并到空闲链表
    return malloc(size);   // 递归调用以分配
}

// 释放内存
void free(void *ptr) {
    if (!ptr) return;

    // 获取块头部并标记为空闲
    free_block *block = (free_block*)((size_t*)ptr - 1);
    block->header &= ~1;
    block->footer &= ~1;

    // 合并相邻空闲块
    coalesce(block);
}


int main()
{
    std::cout<<"hello world\n";

    return 0;
}