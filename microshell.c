#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int tmp;

int print(char *str)
{
	while (*str)
		write(2, str++, 1);
	return 1;
}

int echo_error(char **str, int index)
{
	if (index != 2)
		return (print("error: cd: bad arguments\n"));
	if (chdir(str[1]))
		return (print("error: cd: cannot chage directory to ") & print(str[1]) & print("\n"));
	return 0;
}

int execute(char **str, char **env, int index)
{
	int tmp2[2], res;
	int pip = (str[index] && !strcmp(str[index], "|"));
	int pid;

	if (pip && (pipe(tmp2)))
		return (print("error: fatal\n"));
	if (!(pid = fork())) {
		str[index] = 0;
		if ((dup2(tmp, 0) == -1) | (close(tmp) == -1) | ((pip && (dup2(tmp2[1], 1) == -1) | (close(tmp2[0]) == -1) | (close(tmp2[1]) == -1))))
			return (print("error: fatal\n"));
		execve(*str, str, env);
		return (print("error: cannot execute ") & print(*str) & print("\n"));
	}
	if (((pip && (dup2(tmp2[0], tmp) == -1) | (close(tmp2[0]) == -1) | (close(tmp2[1]) == -1))) | (!pip && dup2(0, tmp) == -1) | (waitpid(pid, &res, 0) == -1))
		return (print("error: fatal\n"));
	return (WIFEXITED(res) && WEXITSTATUS(res));
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	int index = 0;
	int res = 0;
	tmp = dup(0);

	while (av[index] && av[++index])
	{
		av = av + index;
		index = 0;
		while (av[index] && strcmp(av[index], "|") && strcmp(av[index], ";"))
			index++;
		if (!strcmp(*av, "cd"))
			res = echo_error(av, index);
		else if (index)
			res = execute(av, env, index);
	}

	return ((dup2(0, tmp) == -1 && print("error: fatal\n")) | res);
}
		av += index;
		index = 0;
		while (av[index] && strcmp(av[index], "|") && strcmp(av[index], ";"))
			index++;
		if (!strcmp(*av, "cd"))
			res = echo_error(av, index);
		else if (index)
			res = execute(av, env, index);
	}
	return ((dup2(0, tmp) == -1 && print("error: fatal\n")) | res);
}
