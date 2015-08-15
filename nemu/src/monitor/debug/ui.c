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



static int cmd_info(char *args) {
	printf("EAX = [%2x %x %x %x]\n",cpu.gpr[0]._byte[3],cpu.gpr[0]._byte[2],cpu.gpr[0]._byte[1],cpu.gpr[0]._byte[0]);
	printf("ECX = %2x %x %x %x\n",cpu.gpr[1]._byte[3],cpu.gpr[1]._byte[2],cpu.gpr[1]._byte[1],cpu.gpr[1]._byte[0]);
	printf("EDX = %x %x %x %x\n",cpu.gpr[2]._byte[3],cpu.gpr[2]._byte[2],cpu.gpr[2]._byte[1],cpu.gpr[2]._byte[0]);
	printf("EBX = %x %x %x %x\n",cpu.gpr[3]._byte[3],cpu.gpr[3]._byte[2],cpu.gpr[3]._byte[1],cpu.gpr[3]._byte[0]);
	printf("ESP = %x %x %x %x\n",cpu.gpr[4]._byte[3],cpu.gpr[4]._byte[2],cpu.gpr[4]._byte[1],cpu.gpr[4]._byte[0]);
	printf("EBP = %x %x %x %x\n",cpu.gpr[5]._byte[3],cpu.gpr[5]._byte[2],cpu.gpr[5]._byte[1],cpu.gpr[5]._byte[0]);
	printf("ESI = %x %x %x %x\n",cpu.gpr[6]._byte[3],cpu.gpr[6]._byte[2],cpu.gpr[6]._byte[1],cpu.gpr[6]._byte[0]);
	printf("EDI = %x %x %x %x\n",cpu.gpr[7]._byte[3],cpu.gpr[7]._byte[2],cpu.gpr[7]._byte[1],cpu.gpr[7]._byte[0]);

	printf("EAX=%x\n",cpu.eax);
	printf("ECX=%x\n",cpu.ecx);
	printf("EDX=%x\n",cpu.edx);
	printf("EBX=%x\n",cpu.ebx);
	printf("ESP=%x\n",cpu.esp);
	printf("EBP=%x\n",cpu.ebp);
	printf("ESI=%x\n",cpu.esi);
	printf("EDI=%x\n",cpu.edi);

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
	{ "i", "info register/watch [Info r/ info w ]", cmd_info },
    { "m", "Memory Scan", cmd_q },
	{ "s", "Step by Step", cmd_q },
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
