/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "remoteMsg.h"


namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg ()
{
   mMessageType = MsgIdT::cTestMsg;
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
} 

void TestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( &mCode1 );
   aBuffer->copy( &mCode2 );
   aBuffer->copy( &mCode3 );
   aBuffer->copy( &mCode4 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

FirstMessageMsg::FirstMessageMsg ()
{
   mMessageType = MsgIdT::cFirstMessageMsg;
   mCode1 = 0;
} 

void FirstMessageMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoRequestMsg::EchoRequestMsg()
{
   mMessageType = MsgIdT::cEchoRequestMsg;

   mCode1 = 101;
   mCode2 = 102;
   mCode3 = 103;
   mCode4 = 104;

   mNumWords = 0;
   mNumWords = MaxWords;
}

void EchoRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumWords);
   for (int i = 0; i < mNumWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoResponseMsg::EchoResponseMsg()
{
   mMessageType = MsgIdT::cEchoResponseMsg;

   mCode1 = 201;
   mCode2 = 202;
   mCode3 = 203;
   mCode4 = 204;

   mNumWords = 0;
   mNumWords = MaxWords;
}

void EchoResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumWords);
   for (int i = 0; i < mNumWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

WorkRequestMsg::WorkRequestMsg()
{
   mMessageType = MsgIdT::cWorkRequestMsg;

   mCode1 = 101;
   mCode2 = 102;
   mCode3 = 103;
   mCode4 = 104;

   mNumWords = 0;
   mNumWords = MaxWords;
}

void WorkRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumWords);
   for (int i = 0; i < mNumWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

WorkResponseMsg::WorkResponseMsg()
{
   mMessageType = MsgIdT::cWorkResponseMsg;

   mCode1 = 201;
   mCode2 = 202;
   mCode3 = 203;
   mCode4 = 204;

   mNumWords = 0;
   mNumWords = MaxWords;
}

void WorkResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumWords);
   for (int i = 0; i < mNumWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message, based on a message type

void* createMsg (int aMessageType)
{
   Ris::ByteContent* tMsg = 0;

   switch (aMessageType)
   {
   case MsgIdT::cTestMsg :
      tMsg = new TestMsg;
      break;
   case MsgIdT::cFirstMessageMsg :
      tMsg = new FirstMessageMsg;
      break;
   case MsgIdT::cEchoRequestMsg:
      tMsg = new EchoRequestMsg;
      break;
   case MsgIdT::cEchoResponseMsg:
      tMsg = new EchoResponseMsg;
      break;
   case MsgIdT::cWorkRequestMsg:
      tMsg = new WorkRequestMsg;
      break;
   case MsgIdT::cWorkResponseMsg:
      tMsg = new WorkResponseMsg;
      break;
   default :
      return 0;
      break;
   }

   return tMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


