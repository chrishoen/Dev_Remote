/*==============================================================================
ServerThread.cpp
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "remoteSettings.h"

#define  _PROCOSERVERTHREAD_CPP_
#include "remoteServerThread.h"

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ServerThread::ServerThread()
{
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   mPeriodicEnable=false;
   mPeriodicCount=0;
   mStatusCount1=0;
   mStatusCount2=0;

   mTcpServerThread  = new Ris::Net::TcpMsgServerThread;

   // Initialize QCalls
   mSessionQCall.bind (this,&ServerThread::executeSession);
   mRxMsgQCall.bind   (this,&ServerThread::executeRxMsg);
}

//******************************************************************************

ServerThread::~ServerThread()
{
   delete mTcpServerThread;
}

//******************************************************************************
// This sets configuration members

void ServerThread::configure()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message monkey
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   //--------------------------------------------------------------------------- 
   // Configure child thread, server

   mTcpServerThread->configure(
      &mMonkeyCreator,
      "0.0.0.0",
      gSettings.mTcpServerPort,
      MaxSessions,
      &mSessionQCall,
      &mRxMsgQCall);
}

//******************************************************************************

void ServerThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::launch");

   // Launch child thread
   mTcpServerThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.

void  ServerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadExitFunction");

   // Shutdown child thread
   mTcpServerThread->shutdownThread(); 

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void ServerThread::executeSession (int aSessionIndex,bool aConnected)
{
   if (aConnected) Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  CONNECTED     %d",aSessionIndex);
   else            Prn::print(Prn::ThreadRun1, "ServerThread CLIENT  DISCONNECTED  %d",aSessionIndex);

   if(!aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mSessionStateList.remove(aSessionIndex);
   }
}

//******************************************************************************
// QCall
void ServerThread::executeRxMsg(int aSessionIndex,Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   // Put the message to the message processor
   Remote::BaseMsg* tMsg = (Remote::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case Remote::MsgIdT::cTestMsg :
         processRxMsg(aSessionIndex,(Remote::TestMsg*)tMsg);
         break;
      case Remote::MsgIdT::cFirstMessageMsg :
         processRxMsg(aSessionIndex,(Remote::FirstMessageMsg*)tMsg);
         break;
      case Remote::MsgIdT::cStatusRequestMsg :
         processRxMsg(aSessionIndex,(Remote::StatusRequestMsg*)tMsg);
         break;
      case Remote::MsgIdT::cStatusResponseMsg :
         processRxMsg(aSessionIndex,(Remote::StatusResponseMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
// Rx message handler - TestMsg

void ServerThread::processRxMsg(int aSessionIndex,Remote::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - FirstMessageMsg
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex,Remote::FirstMessageMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_FirstMessageMsg %d %d",aSessionIndex,aRxMsg->mHeader.mSourceId);

   //--------------------------------------------------------------
   // Process message:

   // Add session to state list
   mSessionStateList.add(aSessionIndex,aRxMsg->mHeader.mSourceId);

   // Process message:
   //--------------------------------------------------------------

   // Delete the message
   delete aRxMsg;
}

//******************************************************************************
// executeOnTimer

void ServerThread::executeOnTimer(int aTimerCount)
{
   return;
   Prn::print(Prn::ThreadRun3, "ServerThread::executeOnTimer %d",mPeriodicCount++);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void ServerThread::processRxMsg(int aSessionIndex,Remote::StatusRequestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::processRxMsg_StatusRequestMsg %d",aRxMsg->mCode1);

   if (true)
   {
      Remote::StatusResponseMsg* tTxMsg = new Remote::StatusResponseMsg;
      tTxMsg->mCode1 = aRxMsg->mCode1;
      sendMsg(aSessionIndex,tTxMsg);
   }

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void ServerThread::processRxMsg(int aSessionIndex,Remote::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::processRxMsg_StatusResponseMsg");
   delete aRxMsg;
}

//******************************************************************************
// This sends a message via the tcp server thread
void ServerThread::sendMsg(int aSessionIndex,Remote::BaseMsg* aTxMsg)
{
   mTcpServerThread->sendMsg(aSessionIndex,aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread
void ServerThread::sendTestMsg(int aAppNumber)
{
   // Get session index
   int tSessionIndex = mSessionStateList.getIndex(aAppNumber);
   if (tSessionIndex == Ris::Net::SessionStateList::InvalidValue) return;

   // Send message on socket at the session index
   Remote::TestMsg* msg = new Remote::TestMsg;
   msg->mCode1=201;

   mTcpServerThread->sendMsg(tSessionIndex,msg);
}

}//namespace