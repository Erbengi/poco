//
// SMTPClientSession.cpp
//
// $Id: //poco/Main/Net/src/SMTPClientSession.cpp#8 $
//
// Library: Net
// Package: Mail
// Module:  SMTPClientSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/MailStream.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/NetException.h"
#include "Poco/Environment.h"


using Poco::Environment;


namespace Poco {
namespace Net {


SMTPClientSession::SMTPClientSession(const StreamSocket& socket):
	_socket(socket),
	_isOpen(false)
{
}


SMTPClientSession::SMTPClientSession(const std::string& host, Poco::UInt16 port):
	_socket(SocketAddress(host, port)),
	_isOpen(false)
{
}


SMTPClientSession::~SMTPClientSession()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
}


void SMTPClientSession::setTimeout(const Poco::Timespan& timeout)
{
	_socket.setReceiveTimeout(timeout);
}

	
Poco::Timespan SMTPClientSession::getTimeout() const
{
	return _socket.getReceiveTimeout();
}


void SMTPClientSession::login(const std::string& hostname)
{
	open();
	std::string response;
	int status = sendCommand("EHLO", hostname, response);
	if (isPermanentNegative(status))
		status = sendCommand("HELO", hostname, response);
	if (!isPositiveCompletion(status)) throw SMTPException("Login failed", response);
}


void SMTPClientSession::login()
{
	login(Environment::nodeName());
}


void SMTPClientSession::open()
{
	if (!_isOpen)
	{
		std::string response;
		int status = _socket.receiveStatusMessage(response);
		if (!isPositiveCompletion(status)) throw SMTPException("The mail service is unavailable", response);
		_isOpen = true;
	}
}


void SMTPClientSession::close()
{
	if (_isOpen)
	{
		std::string response;
		sendCommand("QUIT", response);
		_socket.close();
		_isOpen = false;
	}
}


void SMTPClientSession::sendMessage(const MailMessage& message)
{
	std::string response;
	std::string sender("<");
	sender.append(message.getSender());
	sender.append(">");
	int status = sendCommand("MAIL FROM:", sender, response);
	if (!isPositiveCompletion(status)) throw SMTPException("Cannot send message", response);
	for (MailMessage::Recipients::const_iterator it = message.recipients().begin(); it != message.recipients().end(); ++it)
	{
		std::string recipient("<");
		recipient.append(it->getAddress());
		recipient.append(">");
		int status = sendCommand("RCPT TO:", recipient, response);
		if (!isPositiveCompletion(status)) throw SMTPException(std::string("Recipient rejected: ") + recipient, response);
	}
	status = sendCommand("DATA", response);
	if (!isPositiveIntermediate(status)) throw SMTPException("Cannot send message data", response);
	SocketOutputStream socketStream(_socket);
	MailOutputStream mailStream(socketStream);
	message.write(mailStream);
	mailStream.close();
	socketStream.flush();
	status = _socket.receiveStatusMessage(response);
	if (!isPositiveCompletion(status)) throw SMTPException("The server rejected the message", response);
}


int SMTPClientSession::sendCommand(const std::string& command, std::string& response)
{
	_socket.sendMessage(command);
	return _socket.receiveStatusMessage(response);
}


int SMTPClientSession::sendCommand(const std::string& command, const std::string& arg, std::string& response)
{
	_socket.sendMessage(command, arg);
	return _socket.receiveStatusMessage(response);
}


} } // namespace Poco::Net
