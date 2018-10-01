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
// Constructor.

ClientThread::ClientThread()
{
   // Set base class thread priority.
   BaseClass::mShortThread->setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mShortThread->mTimerPeriod = 1000;

   // Set base class call pointers
   BaseClass::mShortThread->mThreadInitCallPointer.bind(this, &ClientThread::threadInitFunction);
   BaseClass::mShortThread->mThreadExitCallPointer.bind(this, &ClientThread::threadExitFunction);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer.bind(this, &ClientThread::executeOnTimer);

   // Bind qcalls.
   mSessionQCall.bind(this->mShortThread, this, &ClientThread::executeSession);
   mRxMsgQCall.bind(this->mShortThread, this, &ClientThread::executeRxMsg);
   mTest1QCall.bind(this->mLongThread, this, &ClientThread::executeTest1);

   // Initialize variables.
   mTcpMsgClientThread = 0;
   mMonkeyCreator.configure(gSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

ClientThread::~ClientThread()
{
   delete mTcpMsgClientThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It starts the child thread.

void ClientThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setRemoteIp(gSettings.mTcpServerIPAddress, gSettings.mTcpServerPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mClientSessionQCall = mSessionQCall;
   tSettings.mRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mTcpMsgClientThread = new Ris::Net::TcpMsgClientThread(tSettings);

   // Launch the child thread.
   mTcpMsgClientThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the child thread.

void  ClientThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ClientThread::threadExitFunction");

   // Shutdown the child thread.
   mTcpMsgClientThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when this client connects or
// disconnects to the server). It maintains session state variables. When
// a connection is established it sends a FirstMessage to the server to
// inform it of it's identity.

void ClientThread::executeSession (bool aConnected)
{
   if (aConnected)
   {
      Prn::print(Prn::ThreadRun1, "ClientThread CONNECTED");

      // Transmit a FirstMessage to the server to inform it of who this 
      // client is.
      FirstMessageMsg* msg = new FirstMessageMsg;
      sendMsg(msg);
   }
   else
   {
      Prn::print(Prn::ThreadRun1, "ClientThread DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a message is received. It process the received messages. It calls one of
// the specific receive message handlers.

void ClientThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   BaseMsg* tMsg = (BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case MsgIdT::cTestMsg :
         processRxMsg((TestMsg*)tMsg);
         break;
      case MsgIdT::cEchoRequestMsg:
         processRxMsg((EchoRequestMsg*)tMsg);
         break;
      case MsgIdT::cEchoResponseMsg:
         processRxMsg((EchoResponseMsg*)tMsg);
         break;
      case MsgIdT::cWorkResponseMsg:
         processRxMsg((WorkResponseMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg.

void ClientThread::processRxMsg(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void ClientThread::processRxMsg(EchoRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoRequestMsg %d %d", aMsg->mCode1, aMsg->mNumWords);

   EchoResponseMsg* tTxMsg = new EchoResponseMsg;
   tTxMsg->mCode1 = aMsg->mCode1;
   tTxMsg->mNumWords = aMsg->mNumWords;
   sendMsg(tTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void ClientThread::processRxMsg(EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoResponseMsg %d %d", aMsg->mCode1, aMsg->mNumWords);
   delete aMsg;

   // Send a notification to the long thread.
   BaseClass::notify(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - WorkResponseMsg.

void ClientThread::processRxMsg(WorkResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_WorkResponseMsg %d %d", aMsg->mCode1, aMsg->mNumWords);
   delete aMsg;

   // Send a notification to the long thread.
   BaseClass::notify(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendMsg (BaseMsg* aTxMsg)
{
   mTcpMsgClientThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ClientThread::sendTestMsg()
{
   TestMsg* msg = new TestMsg;
   msg->mCode1=201;
 
   mTcpMsgClientThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer. It
// sends an echo request message.

void ClientThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   // Send an echo request message.
   EchoRequestMsg* tMsg = new EchoRequestMsg;
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
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
   // Send a work request to server and wait for a completion notification.

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
      BaseClass::waitForNotify(2000, 1);

      Prn::print(Prn::ProcRun1, "ClientThread::executeTest1<<<<<<<<<<<<<<<<<<<<<<<<<END");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If the preceding code section encountered a timeout or an abort
   // then this section executes. 

   catch (int aStatus)
   {
      Prn::print(Prn::ProcRun1, "ClientThread::executeTest1.....................ERROR %d", aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace