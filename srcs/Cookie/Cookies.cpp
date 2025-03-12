/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:54:52 by marvin            #+#    #+#             */
/*   Updated: 2025/02/21 21:54:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.h"

Cookie::Cookie(const Cookie& cpy) : userID(cpy.userID), sessionID(cpy.sessionID), username(cpy.username), timestampLastUsed(cpy.timestampLastUsed) {}

Cookie& Cookie::operator=(const Cookie& cpy) {
    if (this != &cpy) {
        this->userID = cpy.userID;
        this->sessionID = cpy.sessionID;
        this->username = cpy.username;
        this->timestampLastUsed = cpy.timestampLastUsed;
    }
    return *this;
}

Cookie::Cookie() : userID("NoneID"), sessionID("NoneID"), username("NoneUsr"),timestampLastUsed(0) {}

Cookie::Cookie(std::string u, std::string s, std::string usr) : userID(u), sessionID(s), username(usr), timestampLastUsed(0) {}

Cookie::~Cookie() {}

/* Setters */
void Cookie::setUserID(std::string id) {
    this->userID = id;
}

void Cookie::setSessionID(std::string id) {
    this->sessionID = id;
}

void Cookie::setTimestamp(long newTs) {
    this->timestampLastUsed = newTs;
}


/* Checkers */
bool Cookie::isSameUserId(std::string id) {
    return this->userID == id;
}

bool Cookie::isSameSessionId(std::string id) {
    return this->sessionID == id;
}

bool Cookie::isSessionStillUp(long timestamp, long timeToExpire) {
    return this->timestampLastUsed + timeToExpire > timestamp;
}


/* Getters */
std::string& Cookie::getUserID( void ) {
    return this->userID;
}

std::string& Cookie::getSessionID( void ) {
    return this->sessionID;
}

std::string& Cookie::getUsername( void ) {
    return this->username;
}

long& Cookie::getTimestamp( void ) {
    return this->timestampLastUsed;
}

std::string Cookie::getRepertory( void ) {
    return (this->username + "_" + this->userID);
}

