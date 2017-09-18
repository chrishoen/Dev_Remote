/*==============================================================================
ServerThread.cpp
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "remoteSettings.h"

#define  _REMOTESERVERTHREAD_CPP_
#include "remoteServerThread.h"

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ServerThread::ServerThread()
{
   // Initialize base class variables.
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Initialize variables.
   mPeriodicEnable=false;
   mPeriodicCount=0;
   mStatusCount1=0;
   mStatusCount2=0;

   mTcpServerThread  = 0;

   // Initialize qcalls.
   mSessionQCall.bind (this,&ServerThread::executeSession);
   mRxMsgQCall.bind   (this,&ServerThread::executeRxMsg);
}

ServerThread::~ServerThread()
{
   delete mTcpServerThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ServerThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadInitFunction");

   // Configure message monkey.
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   // Create server socket child thread.
   mTcpServerThread  = new Ris::Net::TcpMsgServerThread;

   // Create server socket child thread.
   mTcpServerThread->configure(
      &mMonkeyCreator,
      "0.0.0.0",
      gSettings.mTcpServerPort,
      MaxSessions,
      &mSessionQCall,
      &mRxMsgQCall);

   // Launch server socket child thread.
   mTcpServerThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
// executeOnTimer

void ServerThread::executeOnTimer(int aTimerCount)
{
   return;
   Prn::print(Prn::ThreadRun3, "ServerThread::executeOnTimer %d",mPeriodicCount++);
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
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
      case Remote::MsgIdT::cWorkRequestMsg :
         processRxMsg(aSessionIndex,(Remote::WorkRequestMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void ServerThread::processRxMsg(int aSessionIndex,Remote::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - FirstMessageMsg
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex,Remote::FirstMessageMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg_FirstMessageMsg %d %d",aSessionIndex,aRxMsg->mHeader.mSourceId);

   // Add session to state list
   mSessionStateList.add(aSessionIndex,aRxMsg->mHeader.mSourceId);

   // Delete the message
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - WorkRequestMsg

void ServerThread::processRxMsg(int aSessionIndex,Remote::WorkRequestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ServerThread::processRxMsg_WorkRequestMsg %d",aRxMsg->mCode1);

   if (true)
   {
      Remote::WorkResponseMsg* tTxMsg = new Remote::WorkResponseMsg;
      tTxMsg->mCode1 = aRxMsg->mCode1;
      sendMsg(aSessionIndex,tTxMsg);
   }

   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a message via the tcp server thread

void ServerThread::sendMsg(int aSessionIndex,Remote::BaseMsg* aTxMsg)
{
   mTcpServerThread->sendMsg(aSessionIndex,aTxMsg);
}

//******************************************************************************
//******************************************************************************
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

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
