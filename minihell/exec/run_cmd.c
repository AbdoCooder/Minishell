/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:11 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/03 18:26:01 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_cmd_error(char *cmd_name, char *error, int status)
{
    write(2, "minishell : ", 11);
    if (cmd_name)
    {
        write(2, cmd_name, ft_strlen(cmd_name));
        write(2, ": ", 2);
    }
    write(2, error, ft_strlen(error));
    exit(status);
}


void	handle_execution(t_cmdarg *current_cmd, t_list *env)
{
	char *cmd_path;
	char **envp = NULL;

	
	if(current_cmd == NULL || current_cmd->cmd[0] == NULL)
		exit(0);  
	cmd_path = check_exec(current_cmd->cmd[0], env);
	if(cmd_path == NULL)
		ft_cmd_error(current_cmd->cmd[0], "command not found\n", 127);
	envp = get_env(env);
	if(execve(cmd_path, current_cmd->cmd, envp) == -1)
	{
		free_dp(envp);
		if (errno == EACCES)
            ft_cmd_error(current_cmd->cmd[0], "Permission denied\n", 126);
        else
			ft_cmd_error(current_cmd->cmd[0], "execution failure\n", 1);
	}
}

void	handle_heredoc(t_redi_list *input)
{
	int fd;
	(void)input;
	fd = open(HEREDOC_FILE, O_RDONLY);
	if(fd == -1)
		ft_cmd_error(NULL, "open failure\n", 1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_cmd_error(NULL, "dup2 failure\n", 1);
	}
	close(fd);
}

int handel_append(t_redi_list *output)
{
	int out_fd;

	if(output->variable)
		if(is_ambiguous(output->file) == false)
				ft_cmd_error(output->file, "ambiguous redirect\n", 1);
	out_fd = open(output->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(out_fd == -1)
		ft_cmd_error(output->file, "open failure\n", 1);
	if(output->is_last)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			ft_cmd_error(NULL, "dup2 failure\n", 1);
		}
		close(out_fd);
	}
	return (1);
}

void	handle_output(t_redi_list *output)
 {
	int out_fd;

	while(output)
	{
		if(output->type == OUTPUT)
		{
			if(output->variable)
				if(is_ambiguous(output->file) == false)
					ft_cmd_error(output->file, "ambiguous redirect\n", 1);
			out_fd = open(output->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(out_fd == -1)
				ft_cmd_error(output->file, "failure to open out file\n", 27);
			if(output->is_last)
			{
				if(dup2(out_fd, STDOUT_FILENO) == -1)
				{
					close(out_fd);
					ft_cmd_error(NULL, "dup2 failure\n", 1);
				}
			}
			close(out_fd);
		}
		if(output->type == APPEND)
			handel_append(output);
		output = output->next;
	}
}

void         handle_input(t_redi_list *input)
{
	int in_fd;

	while(input)
	{
		if(input->type == INPUT)
		{
			if(input->variable)
			{
				if(is_ambiguous(input->file) == false)
					ft_cmd_error(input->file, "ambiguous redirect\n", 1);
			}
			in_fd = open(input->file, O_RDONLY);
			if(in_fd == -1)
				ft_cmd_error(input->file, "No such file or directory\n", 27);
			if(input->is_last)
			{
				if(dup2(in_fd, STDIN_FILENO) == -1)
				{
					close(in_fd);
					ft_cmd_error(NULL, "dup2 failure\n", 1);
				}
			}
			close(in_fd);
		}

		if(input->type == HEREDOC && input->is_last)
			handle_heredoc(input);
		input = input->next;
	}
}
// =====================/ end handle input redirection /========================//
void  ft_is_builtin(t_cmdarg *current_cmd, t_list **env)
{
	char **cmd;
	
	cmd = current_cmd->cmd;
	if (current_cmd->strags != NULL)
	{
		if (cmd && cmd[0] && is_builtin(cmd[0]) == 0)
		{
			if (run_built_in(current_cmd, env, NULL))
			{
				exit(g_exit_status);
			}
		}
	}
}
void     ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd)
{
	if(tmp_in != 0 && dup2(tmp_in, STDIN_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);
	if(current_cmd->next && dup2(p_fd[1], STDOUT_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);

	if(tmp_in != 0)
		close(tmp_in);
	if(current_cmd->next)
	{
		close(p_fd[1]);
		close(p_fd[0]);
	}
	// fprintf(stderr,"%s\n", "hello");
	
	handle_input(current_cmd->input);
	handle_output(current_cmd->output);
	ft_is_builtin(current_cmd, &env);
	handle_execution(current_cmd, env);
}
