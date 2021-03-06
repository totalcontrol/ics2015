#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

 static WP wp_list[NR_WP];
 static WP *head, *free_;


void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i].used = 0;
	
	}
	wp_list[NR_WP - 1].next = NULL;
	
	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

WP* get_wp_head()
{

return head;
}

//insert int0 tail of used list
WP* new_wp()
{
  WP* templink,*tail,*newwp;
  tail=head;
  if (tail)
    {while(tail->next!=NULL) 
     tail=tail->next;
      
    }
  if (free_!=NULL)
  {
    newwp=free_;
    templink=free_->next;
	free_->next=NULL;
	free_->used=1;
    if (tail)
	 tail->next=free_;
	else head=free_;
	free_=templink;
	return newwp;
  }
  assert(0);
  return NULL;
}
void free_wp(int n)
{
    WP *wp;
	
	WP * tempwp=head;
    assert(n<32);
	wp=&wp_list[n];

	if (wp->used==0) return;

	if (head==wp)
		{
		 head=head->next;
		}
	else
		{
	      while(tempwp->next!=wp) 
		  	{tempwp=tempwp->next;}
		  tempwp->next=wp->next;
		}
	wp->next=free_;
    wp->used=0;
 	free_=wp;
}


