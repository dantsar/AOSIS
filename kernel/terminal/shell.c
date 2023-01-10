#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <ctype.h>
#include <string.h>

#include <common/cpu.h>
#include <terminal/tty.h>
#include <terminal/shell.h>
#include <interrupt/keyboard.h>
#include <interrupt/pic.h>

#define NUM_ELEMENTS(x) (sizeof(x) / sizeof((x)[0]))

extern keyboard_t keyboard;

static bool cmd_clear(void);
static bool cmd_exit(void);
static bool cmd_demo(void);
static bool cmd_help(void);
static bool cmd_amogus(void);
static bool cmd_print_tick(void);
static void command_start();
static void command_run(char *cmd, size_t len);

static bool (*run_cmd)() = NULL;
static struct shell_command commands[] = {
    {.name = "help",    .msg = "prints this message",   .cmd=cmd_help},
    {.name = "clear",   .msg = "clears the screen",     .cmd=cmd_clear},
    {.name = "exit",    .msg = "exit the console",      .cmd=cmd_exit},
    {.name = "demo",    .msg = "run a demo",            .cmd=cmd_demo},
    {.name = "amogus",  .msg = "sus",                   .cmd=cmd_amogus},
    {.name = "tick",    .msg = "print timer tick",      .cmd=cmd_print_tick}
};

static char *amogus = \
"   ###\n"
"  ##--\n"
"==##---\n"
"==##---\n"
"==#####\n"
"==#####\n"
"  ## ##\n"
"  ## ##\n";

static bool cmd_help()
{
    for (size_t i = 0; i < NUM_ELEMENTS(commands); i++)
    {
        tty_putc('\t');
        tty_printstr(commands[i].name);
        tty_printstr("\t");
        tty_printstr(commands[i].msg);
        tty_putc('\n');
    }
    return true;
}

static bool cmd_clear()
{
    tty_clear();
    return true;
}

static bool cmd_exit(void)
{
    tty_printstr("    done!\n");
    for(;;);

    return true;
}

static bool cmd_demo(void)
{
    tty_printstr("demo\n");

    return true;
}

static bool cmd_amogus(void)
{
    tty_printstr(amogus);

    return true;
}

static bool cmd_print_tick(void)
{
    uint32_t tick = pic_get_tick();
    kprintf("Tick: %u\n", tick);

    return true;
}

static void command_start() {
    tty_printstr("$> ");
}

static void command_run(char *cmd, size_t len)
{
    // strip whitespace from cmd
    while (!isspace(cmd[len-1])) {
        cmd[len-1] = '\0';
        len--;
    }

    run_cmd = NULL;
    for (size_t i = 0; i < NUM_ELEMENTS(commands); i++) {
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
            tty_putc('\n');
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
            tty_putc(k.key);
        }
    }
}
