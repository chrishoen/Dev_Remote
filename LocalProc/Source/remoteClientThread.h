#pragma once

/*==============================================================================
Prototype tcp client thread message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risNetTcpMsgClientThread.h"
#include "risThreadsTwoThread.h"

#include "remoteMsg.h"

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a tcp client message thread that connects to a tcp server message
// thread via a tcp client message child thread which manages a tcp message
// socket. It then communicates byte content messages with the server.
//
// It provides the capability to establish a tcp connection with the tcp
// server via the child thread. When the child thread connects or disconnects
// to the server it invokes a qcall that was registered by this thread to defer 
// execution of a session notification handler.
//
// It provides the capability to send messages via the child thread socket and
// it provides handlers for messages received via the child thread socket.
// When the child thread receives a message it invokes a qcall that was
// registered by this thread to defer execution of a message handler that is 
// a member of this thread.
// 
// It inherits from BaseTwoThread to obtain a call queue based thread
// functionality.
// 
// It uses the short thread to manage the message communications and it uses
// the long thread to send a request to the server and then wait for a
// response.

class ClientThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Tcp client thread, this manages session connections and message
   // transmission and reception via a tcp message socket.


   // Tcp client message socket child thread. It provides the thread execution
   // context for a tcp message socket and uses it to provide message
   // communication. It interfaces to this thread via the session and receive
   // message qcall callbacks.
   Ris::Net::TcpMsgClientThread* mTcpMsgClientThread;

   // Message monkey creator used by mTcpClientThread.
   Remote::MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This indicates if a server connection is valid.
   bool mConnectionFlag;

   // Control variables.
   bool mTPFlag;

   // Metrics.
   int  mPeriodicCount;
   int  mStatusCount1;
   int  mStatusCount2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ClientThread();
   ~ClientThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads:

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running. It starts the child thread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It shuts down the child thread.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer. It
   // sends an echo request message.
   void executeOnTimer(int aTimerCount) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Test1 qcall.

   // qcall used for testing. This is a call that is queued to this thread.
   // It is invoked by some other thread to request that this thread
   // performs a transaction with the server.
   Ris::Threads::QCall1<int> mTest1QCall;

   // Send a work request message to the server and then wait for a work 
   // response message from the server. This is bound to the qcall.
   void executeTest1(int  aCode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Session qcall.

   // qcall registered to the mTcpMsgThread child thread. It is invoked when
   // a session is established or disestablished (when this client connects or
   // disconnects to the server). 
   Ris::Net::TcpMsgClientThread::SessionQCall mSessionQCall;

   // Maintain session state variables. When a connection is established it
   // sends a FirstMessage to the server to inform it of it's identity. This
   // is bound to the qcall.
   void executeSession(bool aConnected);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive message qcall.

   // qcall registered to the mTcpMsgThread child thread. It is invoked when
   // a message is received. It process the received messages.
   Ris::Net::TcpMsgClientThread::RxMsgQCall mRxMsgQCall;

   // Call one of the specific receive message handlers. This is bound to the
   // qcall.
   void executeRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Specific receive message handlers.

   void processRxMsg(TestMsg* aMsg);
   void processRxMsg(EchoRequestMsg* aMsg);
   void processRxMsg(EchoResponseMsg* aMsg);
   void processRxMsg(WorkResponseMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via the child thread.
   void sendMsg(Remote::BaseMsg* aMsg);
   void sendTestMsg();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _REMOTECLIENTTHREAD_CPP_
         ClientThread* gClientThread;
#else
extern   ClientThread* gClientThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
