#ifndef _BCC_SH_
#define _BCC_SH_

#include "util.h"
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT_MAX PATH_MAX + NAME_MAX
#define PWD_MAX 1024

void cmd(int argc, char** argv);

void prompt_update(char* prompt);

#endif

