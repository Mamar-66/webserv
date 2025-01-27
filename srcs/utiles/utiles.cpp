/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omfelk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:37:55 by omfelk            #+#    #+#             */
/*   Updated: 2025/01/14 18:10:21 by omfelk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utiles.hpp"

char *ft_strjoin_cpp(char const *s1, char const *s2)
{
	char *new_text = new char[std::strlen(s1)+ std::strlen(s2) + 1];

	if (new_text == NULL)
		return (NULL);

	std::strcpy(new_text, s1);
	std::strcat(new_text, s2);

	delete s1;

	return (new_text);
}
