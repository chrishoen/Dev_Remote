/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "remoteSettings.h"

#define  _REMOTECLIENTTHREAD_CPP_ 
#include "remoteClientThread.h"

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ClientThread::ClientThread()
{
   // Set base class thread priority
   BaseClass::mShortThread->setThreadPriorityHigh();

   // Set timer period
   BaseClass::mShortThread->mTimerPeriod = 1000;

   // Set base class call pointers
   BaseClass::mShortThread->mThreadInitCallPointer.bind(this,&ClientThread::threadInitFunction);
   BaseClass::mShortThread->mThreadExitCallPointer.bind(this,&ClientThread::threadExitFunction);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer.bind(this,&ClientThread::executeOnTimer);

   // Initialize QCalls.
   mSessionQCall.bind (this->mShortThread,this,&ClientThread::executeSession);
   mRxMsgQCall.bind   (this->mShortThread,this,&ClientThread::executeRxMsg);
   mTest1QCall.bind   (this->mLongThread, this,&ClientThread::executeTest1);

   // Initialize variables.
   mTcpClientThread = 0;
   mConnectionFlag = false;
   mPeriodicEnable = false;
   mPeriodicCount = 0;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

ClientThread::~ClientThread()
{
   delete mTcpClientThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ClientThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::launch");

   // Configure message monkey.
   mMonkeyCreator.configure(gSettings.mMyAppNumber);

   // Create child thread.
   mTcpClientThread = new Ris::Net::TcpMsgClientThread;

   // Configure child thread.
   mTcpClientThread->configure(
      &mMonkeyCreator,
      gSettings.mTcpServerIPAddress,
      gSettings.mTcpServerPort,
      &mSessionQCall,
      &mRxMsgQCall);

   // Launch child thread
   mTcpClientThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
// executeOnTimer

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mPeriodicEnable) return;
// Prn::print(Prn::ThreadRun3, "ClientThread::executeOnTimer %d", aTimerCount);

   Remote::WorkRequestMsg* tMsg = new Remote::WorkRequestMsg;
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall.

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
//******************************************************************************
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
      case Remote::MsgIdT::cWorkResponseMsg :
         processRxMsg((Remote::WorkResponseMsg*)tRxMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg %d",tRxMsg->mMessageType);
         delete aRxMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg

void ClientThread::processRxMsg(Remote::TestMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun1, "ClientThread::processRxMsg_TestMsg");
   delete aRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - WorkResponseMsg

void ClientThread::processRxMsg(Remote::WorkResponseMsg* aRxMsg)
{
   Prn::print(Prn::ThreadRun2, "ClientThread::processRxMsg_WorkResponseMsg %d",aRxMsg->mCode1);
   delete aRxMsg;

   // Send a notification to the long thread.
   BaseClass::notify(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sends a message via the tcp client thread.

void ClientThread::sendMsg (Remote::BaseMsg* aTxMsg)
{
   mTcpClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a test message via the tcp client thread

void ClientThread::sendTestMsg()
{
   Remote::TestMsg* msg = new Remote::TestMsg;
   msg->mCode1=201;
 
   mTcpClientThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute the call in the context of the long thread.

void ClientThread::executeTest1(int  aCode)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Send a work request to the show target thread and wait for a completion
   // notification.

   try
   {
      Prn::print(Prn::ProcRun1, "ClientThread::executeTest1>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN");

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Locals.

      // Completion notification.
      Ris::Threads::TwoThreadNotify tNotify(this, 1);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Test1.

      // Reset thread notifications.
      BaseClass::resetNotify();

      // Send a work request message to the server.
      WorkRequestMsg* tMsg = new Remote::WorkRequestMsg;
      sendMsg(tMsg);

      // Wait for notification from the short thread for when the work 
      // response is received.
      BaseClass::waitForNotify(2000,1);

      Prn::print(Prn::ProcRun1, "ClientThread::executeTest1<<<<<<<<<<<<<<<<<<<<<<<<<END");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If the preceding code section encountered a timeout or an abort
   // then this section executes. 

   catch(int aStatus)
   {
      Prn::print(Prn::ProcRun1, "ClientThread::executeTest1.....................ERROR %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace