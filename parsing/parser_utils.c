/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momazouz <momazouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 21:16:40 by momazouz          #+#    #+#             */
/*   Updated: 2024/10/19 09:18:32 by momazouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_empty_argument(t_ParserState *state, t_arg_node **arg_list,
		t_redir_node **redir_list)
{
	if (state->find_red == 1)
	{
		append_redir_node(redir_list, create_redir_node(""));
		state->find_red = 0;
	}
	else
		append_arg_node(arg_list, create_arg_node(""));
	state->i += 2;
}

void	add_buffer_to_args(t_ParserState *state, t_arg_node **arg_list,
		t_redir_node **redir_list)
{
	if (state->buf_index > 0)
	{
		state->buffer[state->buf_index] = '\0';
		if (state->find_red == 1)
		{
			append_redir_node(redir_list, create_redir_node(state->buffer));
			state->buf_index = 0;
			state->find_red = 0;
		}
		else
		{
			append_arg_node(arg_list, create_arg_node(state->buffer));
			state->buf_index = 0;
			state->flag_backslash = 0;
		}
	}
	else
		state->flag_backslash = 1;
}

void	finalize_args(t_ParserState *state, t_arg_node **arg_list,
		t_redir_node **redir_list)
{
	if (state->buf_index > 0)
	{
		state->buffer[state->buf_index] = '\0';
		if (state->find_red == 1)
		{
			append_redir_node(redir_list, create_redir_node(state->buffer));
			state->find_red = 0;
		}
		else
			append_arg_node(arg_list, create_arg_node(state->buffer));
	}
	if (state->buf_index == 0 && state->flag_backslash == 0
		&& (*arg_list) == NULL)
		handle_empty_argument_backslash(state, arg_list, redir_list);
	else if (state->buf_index == 0 && state->flag_backslash == 1
		&& state->quots->empty == 1 && (*arg_list) == NULL
		&& (*redir_list) == NULL)
	{
		state->quots->empty = 2;
		append_arg_node(arg_list, create_arg_node(""));
	}
	else if (state->quots->empty == 0 && state->flag_backslash == 1
		&& (*arg_list) == NULL && (*redir_list) == NULL)
		append_arg_node(arg_list, create_arg_node(""));
}

void	init_parser_state(t_ParserState *state, char *input, t_env *env_var,
		t_quots *quots)
{
	state->input = input;
	state->buffer = malloc(ARG_MAX);
	state->buf_index = 0;
	state->redir_index = 0;
	state->i = 0;
	state->quote = 0;
	state->env_var = env_var;
	state->quots = quots;
	state->j = 0;
	state->find_red = 0;
	state->quots->empty = 0;
	state->flag_backslash = 0;
	state->check_last_space = 0;
	state->check_first_space = 0;
	state->find_dollar_herd = 0;
}

t_ParserContext	initialize_parser_context(t_ParserState *state,
		t_arg_node **arg_list, t_redir_node **redir_list)
{
	t_ParserContext	context;

	context.state = state;
	context.arg_list = arg_list;
	context.redir_list = redir_list;
	context.j = 0;
	context.check = 0;
	return (context);
}
