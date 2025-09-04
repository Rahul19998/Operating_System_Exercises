#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h> // For struct user_regs_struct
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;
    long syscall_num;
    long syscall_ret;
    struct user_regs_struct regs; // Holds the tracee's registers

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program_to_trace> [args...]\n", argv[0]);
        return 1;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // Child process (tracee)
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(1);
    } else { // Parent process (tracer)
        int in_syscall = 0;

        // Wait for the child to stop after PTRACE_TRACEME and execvp
        wait(&status);

        while (1) {
            // Restart the child, tracing system calls
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            wait(&status);

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                printf("Tracee exited or terminated by signal.\n");
                break;
            }

            // Get the register values when the tracee stops
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);

#ifdef __x86_64__
            syscall_num = regs.orig_rax;
#else
            syscall_num = regs.orig_eax;
#endif

            // A stop happens on entry and exit for each syscall.
            if (in_syscall == 0) { // Entry to syscall
                printf("SYSCALL %ld\n", syscall_num);
                in_syscall = 1;
            } else { // Exit from syscall
#ifdef __x86_64__
                syscall_ret = regs.rax;
#else
                syscall_ret = regs.eax;
#endif
                printf("SYSCALL %ld returned %ld\n", syscall_num, syscall_ret);
                in_syscall = 0;
            }
        }
    }
    return 0;
}