#ifndef _BCC_SH_
#define _BCC_SH_

#include "util.h"
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include <signal.h>
#include <sys/wait.h>

#define PROMPT_MAX PATH_MAX + NAME_MAX
#define PWD_MAX 1024

void prompt_update(char* prompt);

int kill_process(token* container);

int execute(token* container);

void cmd(token* container);

#endif

