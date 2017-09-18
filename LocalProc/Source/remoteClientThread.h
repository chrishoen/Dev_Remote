#ifndef _PROCOCLIENTTHREAD_H_
#define _PROCOCLIENTTHREAD_H_

/*==============================================================================
Remote client thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risContainers.h"
#include "risNetTcpMsgClientThread.h"
#include "risThreadsQCallThread.h"

#include "remoteMsg.h"

namespace Remote
{

//******************************************************************************
// Client Thread.
//
// This is the client thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The client thread acts in conjunction with the client message processor
// object. It passes received messages to the processor and gets back messages
// to transmit from the processor. The client thread provides the execution
// context for the message processor to process the messages.
// 
// The client thread has a member, mTcpClientThread that is an instance of
// Ris::Net::TcpClientThreadWithQCall. It is a child thread that manages
// connection session changes and receives messages as a Tcp client. So, there
// are two threads structured as two layers: The client thread and its member
// child thread mTcpClientThread.
//
// The client thread is based on a call queue and it uses QCalls to interface
// its mTcpClientThread. When mTcpClientThread detects a session change it
// invokes the client thread's mSessionQCall, which defers execution of its
// executeSession member function. Likewise, when mTcpClientThread receives
// a message it invokes the client thread's mRxMsgQCall, which defers 
// execution of its executeRxMsg member function. 
//
// mTcpClientThread provides the execution context for actually managing
// session changes and receiving messages. The session thread the provides
// the execution context for processing the session changes and the received 
// messages. The processing is done by the message processor object.
//
//******************************************************************************

class  ClientThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   ClientThread();
  ~ClientThread();

   //--------------------------------------------------------------
   // Configure:

   void configure();


   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members.
   // launch starts the child thread + this thread
   // threadInitFunction sets up the base class multiple objects.
   // threadExitFunction shuts down the child thread
   // executeOnTimer sends a periodic status message.
   void launchThread();
   void threadExitFunction(); 
   void executeOnTimer(int);

   //--------------------------------------------------------------
   // Tcp client thread, this manages session connections and 
   // message transmission and reception

   Ris::Net::TcpMsgClientThread*  mTcpClientThread;

   // Message monkey used by mTcpClientThread
   Remote::MsgMonkeyCreator mMonkeyCreator;

   //--------------------------------------------------------------
   // QCall:

   // QCalls registered to mTcpClientThread
   Ris::Net::TcpMsgClientThread::SessionQCall  mSessionQCall;
   Ris::Net::TcpMsgClientThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeSession (bool aConnected);
   void executeRxMsg   (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // Receive message handlers:

   void processRxMsg (Remote::TestMsg* aRxMsg);
   void processRxMsg (Remote::StatusRequestMsg* aRxMsg);
   void processRxMsg (Remote::StatusResponseMsg* aRxMsg);

   //--------------------------------------------------------------
   // Send a message via mTcpClientThread:

   void sendMsg (Remote::BaseMsg* aTxMsg);
   void sendTestMsg();   

   //--------------------------------------------------------------
   // State:

   // This indicates if a server connection is valid.
   bool mConnectionFlag;

   bool mPeriodicEnable;
   int  mPeriodicCount;
   int  mStatusCount1;
   int  mStatusCount2;

};
//******************************************************************************
// Global instance

#ifdef _PROCOCLIENTTHREAD_CPP_
         ClientThread* gClientThread;
#else
extern   ClientThread* gClientThread;
#endif

//******************************************************************************
}//namespace


#endif

