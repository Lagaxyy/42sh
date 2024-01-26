#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mypipe.h"

static int exec_argv_left(struct Ast *ast, int pipe_fd[2])
{
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    int rep = exec_ast(ast);
    if (rep == -1)
    {
        fprintf(stderr, "mypipe: failed exp");
        return 1;
    }
    return rep;
}

static int exec_argv_right(struct Ast *ast, int *pipe_fd)
{
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
    int rep = exec_ast(ast);
    if (rep == -1)
    {
        fprintf(stderr, "mypipe: failed exp");
        return 1;
    }
    return rep;
}

int exec_pipe(struct Ast *ast_left, struct Ast *ast_right)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        fprintf(stderr, "mypipe: pipe failed");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "mypipe: Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        return exec_argv_left(ast_left, pipe_fd);
    }
    else
    {
        close(pipe_fd[1]);
        int status;
        waitpid(pid, &status, 0);
        pid_t r_pid = fork();
        if (r_pid == -1)
        {
            fprintf(stderr, "mypipe: Fork failed");
            return 1;
        }
        else if (r_pid == 0)
        {
            return exec_argv_right(ast_right, pipe_fd);
        }
        else
        {
            close(pipe_fd[0]);
            waitpid(r_pid, &status, 0);
            int r = WEXITSTATUS(status);
            return r;
        }
    }
}
