/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ewurstei <ewurstei@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:03:47 by ewurstei          #+#    #+#             */
/*   Updated: 2023/06/14 17:22:41 by ewurstei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void Server::signalHandler(int signal_num) {
	cout << YEL "SIGNAL : server user asking to terminate server, code : " << signal_num << NC << endl;
	gServer->closeServer();
}