/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momazouz <momazouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 21:14:06 by momazouz          #+#    #+#             */
/*   Updated: 2024/10/19 09:42:10 by momazouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_exit_code(t_ParserState *state)
{
	char	*env_val;

	env_val = ft_itoa(g_exit_code);
	ft_strcpy(state->buffer + state->buf_index, env_val);
	state->buf_index += ft_strlen(env_val);
	free(env_val);
	state->i++;
}

void	process_env_variable(t_ParserState *state, t_arg_node **arg_list,
		t_redir_node **redir_list)
{
	char			*env;
	char			**result;
	int				i;
	int				check;
	t_ParserContext	context;

	context = initialize_parser_context(state, arg_list, redir_list);
	i = 0;
	check = 0;
	if (state->input[state->i + 1] == '?')
		process_exit_code(state);
	else
	{
		env = handle_env_variable(state);
		if (state->quote == 0 && env != NULL && state->find_red != 1)
		{
			result = split_string(env, state);
			i = handle_split_env_result(state, arg_list, result, &check);
			free_env_result(&env, result, i, check);
		}
		handle_null_env_or_redir(&context, env, check, i);
		process_buffer_until_special_char(state);
		state->i--;
	}
}

void	handle_dollar_sign(t_ParserState *state, t_arg_node **arg_list,
		t_redir_node **redir_list)
{
	int	temp_i;
	int	check_dollar;

	check_dollar = 0;
	state->quots->empty = 0;
	if (state->quote == 0 && state->quots->x == 1)
		state->quots->x = 0;
	if (state->input[state->i] == '$' && (state->quote == 0
			|| state->quote != '\''))
	{
		if (state->quote == 0)
			state->quots->empty = 1;
		if (is_dollar_followed_by_quote(state))
		{
			state->i++;
			check_dollar = 1;
		}
		temp_i = state->i + 1;
		while (is_special_char(state->input[temp_i]))
			temp_i++;
		if (state->input[temp_i] == '\0' && check_dollar != 1)
			state->buffer[state->buf_index++] = '$';
		else
			process_env_variable(state, arg_list, redir_list);
	}
}

void	handle_quotes(t_ParserState *state)
{
	if (state->input[state->i] == '\'' || state->input[state->i] == '"')
	{
		if (state->input[state->i] == '\"')
			state->quots->x = 0;
		else
			state->quots->x = 1;
		if (state->quote == 0)
			state->quote = state->input[state->i];
		else if (state->quote == state->input[state->i])
			state->quote = 0;
	}
}

int	handle_consecutive_quotes(t_ParserState *state)
{
	if ((state->input[state->i] == '\"' && state->input[state->i + 1] == '\"')
		&& is_next_char_not_dquote_or_null(state, state->i + 2)
		&& is_next_char_space_or_null(state, state->i + 2) && state->quote == 0
		&& state->buf_index == 0)
	{
		return (1);
	}
	if ((state->input[state->i] == '\'' && state->input[state->i + 1] == '\'')
		&& is_next_char_not_squote_or_null(state, state->i + 2)
		&& is_next_char_space_or_null(state, state->i + 2) && state->quote == 0
		&& state->buf_index == 0)
	{
		return (1);
	}
	return (0);
}
