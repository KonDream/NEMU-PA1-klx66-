#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp()
{
	WP *f, *k;
	f = free_;
	free_ = free_->next;
	free_->next = NULL;
	k = head;
	if(k == NULL)
	{
		head = f;
		k = head;
	}	
	else
	{
		while(k->next != NULL)
		k = k->next;
		k->next = f;
	}
	return f;	
}

void free_wp(WP *wp)
{
	WP *f, *k;
	k = free_;
	if(k == NULL)
	{
		free_ = wp;
		k = free_;
	}
	else
	{
		while(k->next != NULL)
		k = k->next;
		k->next = wp;
	}
	f = head;
	if(head == NULL)
	assert(0);
	if(head->NO == wp->NO)
	head = head->next;
	else
	{
		while(f->next != NULL && f->next->NO != wp->NO)
		f = f->next;
		if(f->next == NULL && f->NO == wp->NO)
		printf("Illegal\n");
		else if(f->next->NO == wp->NO)
		f->next = f->next->next;
		else
		assert(0);
	}
	wp->next = NULL;
	wp->Val = 0;
	wp->klx = 0;
	wp->EXp[0] = '\0';
}





























