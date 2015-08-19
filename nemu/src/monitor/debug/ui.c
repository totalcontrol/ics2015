#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);




/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

//exec n instruction
static int cmd_exec_n(char *args) {
    int number=0;
	
	if (NULL==args)
	{
		cpu_exec(1);
		return 1;
	}
    
	number=atoi(args);
	cpu_exec(number);
	return 0;
}

//display memory info 
static int cmd_display_mem(char *args) {
    char *p;
	int number;
	if (args==NULL)
		{
		 return 1;
		 }
	p=strtok(args," ");
	if (p)
		{
         number=atoi(p);
		 p=strtok(NULL," ");
		 if (p)
		 	{
		 	number=number+atoi(p);
		 	/*
		 	  if (strcmp(p,"esp")==0)
		 	  	{
		 	  	 printf("esp[%08X]+%d [%08X] ,",cpu.esp,number,swaddr_read(va_to_hwa(cpu.esp+number),4));
		 	  	}
			  if (strcmp(p,"ebp")==0)
		 	  	{printf("ebp[%08X]+%d [%08X] ,",cpu.ebp,number,swaddr_read(2,4));}
			  if (strcmp(p,"eip")==0)
		 	  	{printf("eip[%08X]+%d [%08X] ,",cpu.eip,number,swaddr_read(va_to_hwa(cpu.eip+number),4));}
			  printf("eip[%08X]+%d [%08X] ,",cpu.eip,number,swaddr_read(va_to_hwa(cpu.eip+number),4));}
*/           printf("Mem[%d] [%08X] ,",number,swaddr_read(number,4));
		 	}
		}
	
	return 0;
}



static int cmd_print_exp(char *args) {
int number=0;bool bb;
	number = expr(args,&bb);
  printf("___          %s = %8x          ____\n",args,number);
  return 0;

}


static int cmd_watch_exp(char *args) {
	bool bb;
	WP* tempWP;
    if (args==NULL)
		{
		 printf("error info format [info r/info w]\n");
		 return 1;
		}	

	tempWP=new_wp();
	strcpy(tempWP->expr,args);        //record expression
	tempWP->value=expr(args,&bb);  //compute expression
    return 0;
	
}

static int cmd_del_watch(char *args) {
    if (args==NULL)
		{
		 printf("error info format [info r/info w]\n");
		 return 1;
		}	
	int n;
	n=atoi(args);
	if (n>31){
		printf("no such watch point, number is too big\n");
		return 1;
		}
	
    free_wp(n);
    return 0;
	
}


static int cmd_info(char *args) {

	WP *UsedWP;
	
    if (args==NULL)
		{
		 printf("error info format [info r/info w]\n");
		 return 1;
		 }
	 if (0==strcmp(args,"r"))
		 {
		  	printf("EAX = [ %02X %02X %02X %02X ]   ",cpu.gpr[0]._byte[3],cpu.gpr[0]._byte[2],cpu.gpr[0]._byte[1],cpu.gpr[0]._byte[0]);
			printf("EAX = %08X\n",cpu.eax);

			printf("ECX = [ %02X %02X %02X %02X ]   ",cpu.gpr[1]._byte[3],cpu.gpr[1]._byte[2],cpu.gpr[1]._byte[1],cpu.gpr[1]._byte[0]);
			printf("ECX = %08X\n",cpu.ecx);

			printf("EDX = [ %02X %02X %02X %02X ]   ",cpu.gpr[2]._byte[3],cpu.gpr[2]._byte[2],cpu.gpr[2]._byte[1],cpu.gpr[2]._byte[0]);
			printf("EDX = %08X\n",cpu.edx);

			printf("EBX = [ %02X %02X %02X %02X ]   ",cpu.gpr[3]._byte[3],cpu.gpr[3]._byte[2],cpu.gpr[3]._byte[1],cpu.gpr[3]._byte[0]);
			printf("EBX = %08X\n",cpu.ebx);

			printf("ESP = [ %02X %02X %02X %02X ]   ",cpu.gpr[4]._byte[3],cpu.gpr[4]._byte[2],cpu.gpr[4]._byte[1],cpu.gpr[4]._byte[0]);
			printf("ESP = %08X\n",cpu.esp);

			printf("EBP = [ %02X %02X %02X %02X ]   ",cpu.gpr[5]._byte[3],cpu.gpr[5]._byte[2],cpu.gpr[5]._byte[1],cpu.gpr[5]._byte[0]);
			printf("EBP = %08X\n",cpu.ebp);

			printf("ESI = [ %02X %02X %02X %02X ]   ",cpu.gpr[6]._byte[3],cpu.gpr[6]._byte[2],cpu.gpr[6]._byte[1],cpu.gpr[6]._byte[0]);
			printf("ESI = %08X\n",cpu.esi);

			printf("EDI = [ %02X %02X %02X %02X ]   ",cpu.gpr[7]._byte[3],cpu.gpr[7]._byte[2],cpu.gpr[7]._byte[1],cpu.gpr[7]._byte[0]);
			printf("EDI = %08X\n",cpu.edi);
		    return 1;
		 }
	 
	 else if (0==strcmp(args,"w"))
		 {
			UsedWP=get_wp_head();
			while (UsedWP!=NULL)
				{printf("#%d----%2d----%s\n",UsedWP->NO,UsedWP->value,UsedWP->expr);
		         UsedWP=UsedWP->next;
			}
			return 0;
          }
	 return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "info", "info register/watch [Info r/ info w ]", cmd_info },
    { "x", "Memory Info[x n esp]", cmd_display_mem },
	{ "s", "Step by Step[s n]", cmd_exec_n},
	{ "p", "print expression value[p expr]", cmd_print_exp},
	{ "w", "watch expression[w expr] ", cmd_watch_exp},
	{ "d", "delete expression[d n] ", cmd_del_watch},
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
