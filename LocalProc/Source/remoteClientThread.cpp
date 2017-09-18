/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "remoteSettings.h"

#define  _PROCOCLIENTTHREAD_CPP_
#include "remoteClientThread.h"

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ClientThread::ClientThread()
{
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 10;

   mTcpClientThread = new Ris::Net::TcpMsgClientThread;
   mConnectionFlag=false;
   mPeriodicEnable=false;
   mPeriodicCount=0;
   mStatusCount1=0;
   mStatusCount2=0;

   // Initialize QCalls
   mSessionQCall.bind (this,&ClientThread::executeSession);
   mRxMsgQCall.bind   (this,&ClientThread::executeRxMsg);
}

//******************************************************************************

ClientThread::~ClientThread()
{
   delete mTcpClientThread;
}

//******************************************************************************
// This sets configuration members

void ClientThread::configure()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::configure");

   //--------------------------------------------------------------------------- 
   // Configure message monkey
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   //---------------------------------------------------------------------------
   // Configure child thread, client

   mTcpClientThread->configure(
      &mMonkeyCreator,
      "127.0.0.1",
      gSettings.mTcpServerPort,
      &mSessionQCall,
      &mRxMsgQCall);
}

//******************************************************************************

void ClientThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::launch");

   // Launch child thread
   mTcpClientThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  ClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadExitFunction");

   // Shutdown the tcp client thread
   mTcpClientThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall
void ClientThread::executeSession (bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      Remote::FirstMessageMsg* msg = new Remote::FirstMessageMsg;
      sendMsg(msg);
   }
   else
   {
      Prn::print(Prn::ThreadRun1, "ClientThread::notifyForSession DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
// QCall

void ClientThread::executeRxMsg(Ris::ByteContent* aRxMsg)
{
   if(!aRxMsg) return;

   // Put the message to the message processor
   Remote::BaseMsg* tRxMsg = (Remote::BaseMsg*)aRxMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tRxMsg->mMessageType)
   {
      case Remote::MsgIdT::cTestMsg :
         processRxMsg((Remote::TestMsg*)tRxMsg);
         break;
      case Remote::MsgIdT::cStatusRequestMsg :
         processRxMsg((Remote::StatusRequestMsg*)tRxMsg);
         break;
      case Remote::MsgIdT::cStatusResponseMsg :
         processRxMsg((Remote::StatusResponseMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg %d",tRxMsg->mMessageType);
         delete aRxMsg;
         break;
   }
}

//******************************************************************************
// Rx message handler - TestMsg

void ClientThread::processRxMsg(Remote::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
// executeOnTimer

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mPeriodicEnable) return;
   Prn::print(Prn::ThreadRun3, "ClientThread::executeOnTimer %d", aTimerCount);

   Remote::StatusRequestMsg* tMsg = new Remote::StatusRequestMsg;
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
// Rx message handler - StatusRequestMsg

void ClientThread::processRxMsg(Remote::StatusRequestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ClientThread::processRxMsg_StatusRequestMsg %d",mStatusCount1++);

   if (true)
   {
      Remote::StatusResponseMsg* tTxMsg = new Remote::StatusResponseMsg;
      sendMsg(tTxMsg);
   }

   delete aRxMsg;
}

//******************************************************************************
// Rx message handler - StatusResponseMsg

void ClientThread::processRxMsg(Remote::StatusResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ClientThread::processRxMsg_StatusResponseMsg %d",aRxMsg->mCode1);
   delete aRxMsg;
}

//******************************************************************************
// This sends a message via the tcp client thread

void ClientThread::sendMsg (Remote::BaseMsg* aTxMsg)
{
   mTcpClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void ClientThread::sendTestMsg()
{
   Remote::TestMsg* msg = new Remote::TestMsg;
   msg->mCode1=201;
 
   mTcpClientThread->sendMsg(msg);
}

}//namespace