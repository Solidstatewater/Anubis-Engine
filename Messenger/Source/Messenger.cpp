//====================================================================================
//Messenger.cpp
//
//This code is part of Anubis Engine.
//
//Anubis Engine is a free game engine created as a fan project to be
//awesome platform for developing games!
//
//All sources can be found here:
//	https://github.com/Dgek/Engine
//
//Demos based on Anubis Engine can be found here:
//	https://github.com/Dgek/Demos
//
//Copyright (c) 2013, Muralev Evgeny
//All rights reserved.
//
//Redistribution and use in source and binary forms, with
//or without modification, are permitted provided that the
//following conditions are met:
//
//Redistributions of source code must retain the above copyright notice,
//this list of conditions and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice,
//this list of conditions and the following disclaimer in the documentation
//and/or other materials provided with the distribution.
//
//Neither the name of the Minotower Games nor the names of its contributors 
//may be used to endorse or promote products derived from this software 
//without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY MURALEV EVGENY "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED. IN NO EVENT SHALL MURALEV EVGENY BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//====================================================================================

#include "Messenger_pch.h"
#include "Messenger.h"

using namespace Anubis;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Global pointer to the messenger
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ASTATIC Messenger * g_pMessenger = NULL;

/*** Constructor ***/
Messenger::Messenger()
{
	//assign global pointer to the instance
	g_pMessenger = this;
}

/*** Destructor ***/
Messenger::~Messenger()
{
	//set global pointer to NULL
	if (g_pMessenger)
		g_pMessenger = NULL;
}

/*** Sengleton ***/
Messenger * const Messenger::Get()
{
	return g_pMessenger;
}

// ===================================
//	Private methods for internal stuff
// ===================================

//register message type
ABOOL Messenger::RegisterMessageTypeInternal(MessageType & type)
{
	//Iterate through the register
	//auto reg = m_register.begin();
	HandlersMap::iterator reg = m_register.begin();

	for (; reg != m_register.end(); reg++)
	{
		//such message type already registered
		if (reg->first == type)
		{
			ASSERT(0 && "Such message type has been already registered!");
			return false;
		}
	}

	//Add new message type to the register
	//m_register.push_back(make_pair(type, decltype(reg->second)()));
	m_register.push_back(make_pair(type, reg->second));

	return true;
}

//register handler to react to certain message
ABOOL Messenger::RegisterHandlerInternal(MessageHandler const * const pHandler, MessageType & type)
{
	//Iterate though the register
	for (auto reg = m_register.begin(); reg != m_register.end(); reg++)
	{
		//we have found message type to subscribe handler to
		if (reg->first == type)
		{
			//now, iterate through the handlers and make we don't duplicate one
			auto handlers = &reg->second;
			for (auto handler = handlers->begin(); handler != handlers->end(); handler++)
			{
				//this handler has already been registered
				if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
				{
					ASSERT(0 && "Handler for the message type has already been registered!");
					return false;
				}
			}

			//add new handler to the list for this message type
			handlers->push_back(make_shared<MessageHandler>(*pHandler));

			return true; //handler has been added
		}
	}

	//Such message type is not registered
	return false;
}

//stop caring about certain message
ABOOL Messenger::StopHandlingInternal(MessageHandler const * const pHandler, MessageType & type)
{
	//Iterate though the register
	auto reg = m_register.begin();
	for (; reg != m_register.end(); reg++)
	{
		//we have found message type to unsubscribe handler from
		if (reg->first == type)
		{
			//now, iterate through the handlers
			auto handlers = &reg->second;
			auto handler = handlers->begin();
			for (; handler != handlers->end(); handler++)
			{
				//the registered handler was found
				if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
					break;
			}

			if (handler != handlers->end())
			{
				//remove handler from the list of this message type
				handlers->erase(handler);
				return true;
			}
			
			//the handler wasn't handling this message type
			ASSERT(0 && "The Handler wasn't handling this message type!");
			return false;
		}
	}

	ASSERT(0 && "Such message type is not registered!");
	return true;
}

//delete handler from messenger
ABOOL Messenger::RemoveHandlerInternal(MessageHandler const * const pHandler)
{
	//flag to track whether the handler was registered
	ABOOL registered = false;

	//Iterate though the register
	for (auto reg = m_register.begin(); reg != m_register.end(); reg++)
	{
		//now, iterate through the handlers
		auto handlers = &reg->second;
		auto handler = handlers->begin();
		for (; handler != handlers->end(); handler++)
		{
			//the registered handler was found
			if ((&**handler) == pHandler) //unpack iterator and retrieve pointer from shared_ptr
				break;
		}

		//the handler was subscribed for this message, so delete it
		if (handler != handlers->end())
		{
			handlers->erase(handler);
			registered = true;
		}
	}

	//return false if listener wasn't deleted from anywhere
	return registered;
}

/*
ABOOL Messenger::RemoveHandlerInternal(ACHAR const * const handlerName)
{
	return true;
} */

//send message to the handlers
ABOOL Messenger::SendMessageInternal(Message const & msg)
{
	//Iterate through register and find message we are interested in
	for (auto reg = m_register.begin(); reg != m_register.end(); reg++)
	{
		//message type is found
		if (msg.VGetMessageType() == reg->first)
		{
			//true if message was handled by at least one of the listeners
			ABOOL handled = false;

			//now, iterate through the handlers
			auto handlers = &reg->second;
			for (auto handler = handlers->begin(); handler != handlers->end(); handler++)
			{
				if ((*handler)->VOnEvent(msg))
					handled = true;
			}

			//return false is no listener handled the event
			return handled;
		}
	}

	ASSERT(0 && "Such event type is not registered!");
	return false;
}

//=================================================
//	Global external methods for using the messenger
//=================================================

//register message type
ABOOL Anubis::RegisterMessageType(MessageType & type)
{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RegisterMessageTypeInternal(type);
}

//register handler to react to certain message
ABOOL Anubis::RegisterHandler(MessageHandler const * const pHandler, MessageType & type)
{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RegisterHandlerInternal(pHandler, type);
}

//stop caring about certain message
ABOOL Anubis::StopHandling(MessageHandler const * const pHandler, MessageType & type)
{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->StopHandlingInternal(pHandler, type);
}

//delete handler from messenger
ABOOL Anubis::RemoveHandler(MessageHandler const * const pHandler)
	{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RemoveHandlerInternal(pHandler);
}

/*ABOOL Anubis::RemoveHandler(ACHAR const * const handlerName)
	{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->RemoveHandlerInternal(handlerName);
} */

//send message to the handlers
ABOOL Anubis::SendGameMessage(Message const & msg)
	{
	//assert that we have valid pointer to the messenger
	ASSERT(Messenger::Get());

	//execute internal methods
	return Messenger::Get()->SendMessageInternal(msg);
}
