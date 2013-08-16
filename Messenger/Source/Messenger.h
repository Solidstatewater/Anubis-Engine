#pragma once
//====================================================================================
//Messenger.h  : Messega pool
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

#include "Message.h"
#include "MessageHandler.h"

using namespace std;

namespace Anubis
{
	class Messenger
	{
	public:
		//Constructor and Destructor
		explicit Messenger();
		~Messenger();

		/** ============= **/
		//	  Singleton	   //
		/** ============= **/
		ASTATIC Messenger * const Get();

	private:

		//Usefull typedefs
		typedef vector<MessageHandlerPtr>			Handlers;					//list of handlers
		typedef pair<MessageType &, Handlers>		MessageTypeAndHandlersPair; //element in handlers map
		typedef list<MessageTypeAndHandlersPair>	HandlersMap;				//maps message type to handlers

		/*****
			Declare Messenger lists
								 *****/
		HandlersMap	m_register;

		/** ============================================ **/
		//		Private methods for internal stuff		  //
		/**	============================================ **/

		//register message type
		ABOOL RegisterMessageTypeInternal(MessageType & type);

		//register handler to react to certain message
		ABOOL RegisterHandlerInternal(MessageHandler const * const pHandler,
			MessageType & type);

		//stop caring about certain message
		ABOOL StopHandlingInternal(MessageHandler const * const pHandler, MessageType & type);

		//delete handler from messenger
		ABOOL RemoveHandlerInternal(MessageHandler const * const pHandler);
		//ABOOL RemoveHandlerInternal(ACHAR const * const handlerName);

		//send message to the handlers
		ABOOL SendMessageInternal(Message const & msg);

		//handle all events
	private:

		/** =========================================== **/
		//		Global methods for message-handling		 //
		//												 //
		//	 These are external methods (basically -	 //
		//   wrappers around !Internal! methods) and	 //
		//	 make needed assertions to safely use them   //
		//	 throught the code.							 //
		//												 //	
		/**	=========================================== **/

		//register message type
		friend ABOOL RegisterMessageType(MessageType & type);

		//register handler to react to certain message
		friend ABOOL RegisterHandler(MessageHandler const * const pHandler,
			MessageType & type);

		//stop caring about certain message
		friend ABOOL StopHandling(MessageHandler const * const pHandler, MessageType & type);

		//delete handler from messenger
		friend ABOOL RemoveHandler(MessageHandler const * const pHandler);
		//friend ABOOL RemoveHandler(ACHAR const * const handlerName);

		//send message to the handlers
		friend ABOOL SendGameMessage(Message const & msg);
	};

}; //Anubis