#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "pgm.h"

// Physical memory is 2^24 (0x1000000 OR 16777216) bytes, with 2^12 (4096) physical pages.
static const int physical_page_count = 4096; // Number of physical pages.
static const int physical_page_size = 4096;  // Size of each page.

// Virtual addresses are 22 bits, 10 bits for page number, 12 bits for offset in page.
// The macros PAGE(n) and OFFSET(n) extract the page number and the offset within the
// page from a virtual address.
#define PHYS_ADDR(phys_page, offset) (((phys_page) << 12) + (offset))
#define OFFSET(n) ((n)&0xFFF)
#define PAGE(n) (((n) >> 12) & 0x3FF)

int pgm_get(struct page_table *pgtab, int start_addr, void *buf, int size)
{

    if (start_addr < 0 || start_addr + size > (1 << 22))
    {
        return -1;
    }

    int start_page = PAGE(start_addr);
    int start_offset = OFFSET(start_addr);

    int end_page = PAGE(start_addr + size - 1);
    int end_offset = OFFSET(start_addr + size - 1);

    int cur_page = start_page;
    int cur_offset = start_offset;
    int cur_size = size;
    char *cur_buf = (char *)buf;
    while (cur_size > 0)
    {

        pthread_mutex_lock(&physical_pages[pgtab->entries[cur_page].phys_page].lock);

        int copy_size = physical_page_size - cur_offset;
        if (copy_size > cur_size)
        {
            copy_size = cur_size;
        }

        memcpy(cur_buf, physical_pages[pgtab->entries[cur_page].phys_page].data + cur_offset, copy_size);

        pthread_mutex_unlock(&physical_pages[pgtab->entries[cur_page].phys_page].lock);

        cur_page++;
        cur_offset = 0;
        cur_size -= copy_size;
        cur_buf += copy_size;
    }

    return 0;
}

void pgm_init()
{

    for (int i = 0; i < physical_page_count; i++)
    {
        pthread_mutex_init(&physical_pages[i].lock, NULL);
        physical_pages[i].is_allocated = 0;
    }

    global_page_table = (struct page_table *)malloc(sizeof(struct page_table));
    pthread_mutex_init(&global_page_table->lock, NULL);
    for (int i = 0; i < (1 << 10); i++)
    {
        global_page_table->entries[i].is_allocated = 0;
        global_page_table->entries[i].phys_page = -1;
    }
}

struct page_table_entry
{
    int is_allocated;
    int phys_page;
};

struct page_table
{
    pthread_mutex_t lock;
    struct page_table_entry entries[1 << 10];
};

static struct physical_page physical_pages[physical_page_count];
static struct page_table *global_page_table = NULL;

void pgm_destroy()
{

    for (int i = 0; i < physical_page_count; i++)
    {
        pthread_mutex_destroy(&physical_pages[i].lock);
    }

    free(global_page_table);
}

void pgm_discard(struct page_table *pgtab)
{

    for (int i = 0; i < (1 << 10); i++)
    {
        if (pgtab->entries[i].is_allocated)
        {
            physical_pages[pgtab->entries[i].phys_page].is_allocated = 0;
            pgtab->entries[i].is_allocated = 0;
            pgtab->entries[i].phys_page = -1;
        }
    }

    free(pgtab);
}

int pgm_put(struct page_table *pgtab, int start_addr, void *buf, int size)
{

    if (start_addr < 0 || start_addr + size > (1 << 22))
    {
        return -1;
    }

    int start_page = PAGE(start_addr);
    int start_offset = OFFSET(start_addr);

    int end_page = PAGE(start_addr + size - 1);
    int end_offset = OFFSET(start_addr + size - 1);

    pthread_mutex_lock(&pgtab->lock);

    for (int i = start_page; i <= end_page; i++)
    {
        if (!pgtab->entries[i].is_allocated)
        {

            int phys_page = -1;
            for (int j = 0; j < physical_page_count; j++)
            {
                if (!physical_pages[j].is_allocated)
                {

                    pthread_mutex_lock(&physical_pages[j].lock);
                    physical_pages[j].is_allocated = 1;
                    phys_page = j;
                    break;
                }
            }

            if (phys_page == -1)
            {
                pthread_mutex_unlock(&pgtab->lock);
                return -1;
            }

            pgtab->entries[i].is_allocated = 1;
            pgtab->entries[i].phys_page = phys_page;
        }
    }

    pthread_mutex_unlock(&pgtab->lock);

    int cur_page = start_page;
    int cur_offset = start_offset;
    int cur_size = size;
    char *cur_buf = (char *)buf;
    while (cur_size > 0)
    {

        pthread_mutex_lock(&physical_pages[pgtab->entries[cur_page].phys_page].lock);

        int copy_size = physical_page_size - cur_offset;
        if (copy_size > cur_size)
        {
            copy_size = cur_size;
        }

        memcpy(physical_pages[pgtab->entries[cur_page].phys_page].data + cur_offset, cur_buf, copy_size);

        pthread_mutex_unlock(&physical_pages[pgtab->entries[cur_page].phys_page].lock);

        cur_page++;
        cur_offset = 0;
        cur_size -= copy_size;
        cur_buf += copy_size;
    }

    return 0;
}

struct physical_page
{
    pthread_mutex_t lock;
    int is_allocated;
    char data[physical_page_size];
};

struct page_table *pgm_create()
{
    struct page_table *pgtab = (struct page_table *)malloc(sizeof(struct page_table));
    pthread_mutex_init(&pgtab->lock, NULL);
    for (int i = 0; i < (1 << 10); i++)
    {
        pgtab->entries[i].is_allocated = 0;
        pgtab->entries[i].phys_page = -1;
    }
    return pgtab;
}