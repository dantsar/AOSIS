#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <ctype.h>
#include <string.h>

#include <common/cpu.h>
#include <terminal/tty.h>
#include <terminal/shell.h>
#include <interrupt/keyboard.h>

extern keyboard_t keyboard;

static bool cmd_clear();
static bool cmd_exit();
static bool cmd_demo();
static bool cmd_help();

bool (*run_cmd)() = NULL;
static struct shell_command commands[] = {
    {.name = "help",    .msg = "prints this message",   .cmd=cmd_help},
    {.name = "clear",   .msg = "clears the screen",     .cmd=cmd_clear},
    {.name = "exit",    .msg = "exit the console",      .cmd=cmd_exit},
    {.name = "demo",    .msg = "run a demo",            .cmd=cmd_demo},
};

bool cmd_help()
{
    for (size_t i = 0; i < sizeof(commands)/sizeof(struct shell_command); i++) {
        tty_putchar('\t');
        tty_printstr(commands[i].name);
        tty_printstr("  ");
        tty_printstr(commands[i].msg);
        tty_putchar('\n');
    }
    return true;
}

static bool cmd_clear() 
{
    tty_clear();
    return true;
}

bool cmd_exit()
{
    tty_printstr("\tdone!\n");
    for(;;);
    return true;
}

bool cmd_demo()
{
    tty_printstr("demo\n");
    return true;
}

void command_start() {
    tty_printstr("> ");
}

void command_run(char *cmd, size_t len) 
{
    // strip whitespace from cmd
    while (!isspace(cmd[len-1])) {
        cmd[len-1] = '\0';
        len--;
    }

    run_cmd = NULL;
    for (size_t i = 0; i < sizeof(commands); i++) {
        if (!strcmp(commands[i].name, cmd)) {
            run_cmd = commands[i].cmd;
            break;
        }
    }

    if (run_cmd != NULL) {
        run_cmd();
        return;
    }

    tty_printstr("Unknown command '");
    tty_write(cmd, len);
    tty_printstr("'\n");
}

void kconsole()
{
	tty_printstr("Welcome to AOSIS: Run 'help' to get started!\n");
    command_start();

    char cmd[1024] = {0};
    size_t cmd_len = 0;

    for(;;) {
        halt(); // halt until an interrupt occurs

        key_t k;
        if (kb_get_key(&k) == false) {
            continue;
        }
        // print to the screen and wait for KEY_ENTER
        if (k.key == KEY_BACKSPACE && cmd_len > 0) {
            kb_get_key(NULL); 
            cmd[cmd_len-1] = '\0';
            cmd_len--;
            tty_deleteprev();
        } else if (k.key == KEY_ENTER) {
            // handle the command
            tty_putchar('\n');
            command_run(cmd, cmd_len);

            memset(cmd, '\0', sizeof(cmd));
            cmd_len = 0;
            command_start();
        } 
        else if (!isprint(k.key) || !isspace(k.key)) {
            if (cmd_len >= sizeof(cmd)) {
                continue;
            }
            cmd[cmd_len] = k.key;
            cmd_len++;
            tty_putchar(k.key);
        }
    }
}
