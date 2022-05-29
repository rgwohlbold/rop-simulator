#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>

#define MAX_ENTRIES 100

extern void *(gadget0)();
extern void *(gadget1)();
extern void *(gadget2)();
extern void *(gadget3)();
extern void *(gadget4)();

typedef struct {
    int num;
    void *addr;
    const char *text;
} gadget_t;

typedef struct {
    void *addr;
    const char *text;
} addr_t;

void setup_buffering() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

void print_help() {
    printf("Welcome to the (unofficial) CnR ROP simulator!\nPlease enter your gadgets below.\nAdditionally, you can enter double-quoted strings and constants prefixed with $.\n");
}

void print_gadgets(gadget_t gadgets[], size_t num) {
    printf("The following gadgets are available to you:\n");
    for (size_t i = 0; i < num; i++) {
        printf("%d\t%s\n", gadgets[i].num, gadgets[i].text);
    }
}

void print_addrs() {
    addr_t addrs[] = {
        { .addr = print_help, .text = "print_help"},
        { .addr = system, .text = "system"},
        { .addr = exit, .text = "exit"},
        { .addr = puts, .text = "puts"},
        { .addr = execve, .text = "execve"},
    };
    printf("Here are some useful addresses for you:\n");
    for (size_t i = 0; i < sizeof(addrs) / sizeof(addr_t); i++) {
        printf("0x%lx\t%s\n", (uint64_t) addrs[i].addr, addrs[i].text);
    }
}

char *read_input() {
    char *line;
    if (!(line = readline(">>> "))) {
        perror("readline");
        exit(EXIT_FAILURE);
    }
    return line;
}

void setup_stack(char *line, uint64_t *return_addr, gadget_t gadgets[], size_t num) {
    char *i = line;
    uint64_t *p = return_addr;
    while (1) {
        while (isspace(*i)) {
            i++;
        }
        if (!*i) {
            break;
        }
        if (p - return_addr == MAX_ENTRIES) {
            goto input_fail;
        }
        if (*i == '$') {
            i++;
            char *end;
            *p++ = strtoul(i, &end, 16);
            if (*end && *end != ' ') {
                goto input_fail;
            }
            if (errno == ERANGE) {
                goto input_fail;
            }
            i = end;
        } else if (*i == '"') {
            i++;
            char *end = strchr(i, '"');
            long len = end - i;
            char *str = malloc(len);
            memcpy(str, i, len);
            str[len] = 0;
            *p++ = (uint64_t) str;
            i = end + 1;
        } else if (isdigit(*i)) {
            char *end;
            unsigned long val = strtoul(i, &end, 10);
            if (*end && *end != ' ') {
                goto input_fail;
            }
            if (errno == ERANGE) {
                goto input_fail;
            }
            i = end;
            if (val >= num) {
                goto input_fail;
            }
            *p++ = (uint64_t) gadgets[val].addr;
        } else {
            goto input_fail;
        }
    }
    return;
input_fail:
    fprintf(stderr, "invalid input at pos %ld\n", i-line);
    free(line);
    exit(EXIT_FAILURE);
}

int main() {
    gadget_t gadgets[] = {
        { .num = 0, .addr = gadget0, .text = "pop rdi; ret"},
        { .num = 1, .addr = gadget1, .text = "call rax; ret"},
        { .num = 2, .addr = gadget2, .text = "pop rax; ret"},
        { .num = 3, .addr = gadget3, .text = "pop rbx; pop rbp; pop r12; pop r13; pop r14; pop r15; ret"},
        { .num = 4, .addr = gadget4, .text = "cmp rbp, rbx; jne crash_program; mov rdx, r15; mov rsi, r14; mov edi, r13d; ret"},
    };

    uint64_t *return_addr;
    __asm__("mov %%rbp, %0" : "=r"(return_addr));
    return_addr++;

    print_help();
    setup_buffering();
    print_gadgets(gadgets, sizeof(gadgets) / sizeof(gadget_t));
    print_addrs();

    char *line = read_input();
    setup_stack(line, return_addr, gadgets, sizeof(gadgets) / sizeof(gadget_t));

    free(line);
    return 0;
}
