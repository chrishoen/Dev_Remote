
#include "stdafx.h"

#include "remoteSettings.h"
#include "remoteClientThread.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
<<<<<<< HEAD
   if (aCmd->isCmd("TP"))     Remote::gClientThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("Tx"))     executeTx(aCmd);
   if (aCmd->isCmd("ECHO"))   executeEcho(aCmd);
   if (aCmd->isCmd("Test1"))  executeTest1(aCmd);
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("Parms"))  executeParms(aCmd);
=======
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("Tx"     ))  executeTx(aCmd); 
   if (aCmd->isCmd("Test1" ))  executeTest1(aCmd);
   if (aCmd->isCmd("TP"    ))  Remote::gClientThread->mPeriodicEnable = aCmd->argBool(1);

   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
   if(aCmd->isCmd("Parms"  ))  executeParms(aCmd);
>>>>>>> 259bf987233621c923bc3ed20de322b3a6e75d31
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);

   Remote::TestMsg* tMsg = new Remote::TestMsg;
   Remote::gClientThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   int tNumWords = aCmd->argInt(1);

   Remote::EchoRequestMsg* tMsg = new Remote::EchoRequestMsg;
   Remote::gClientThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);

   Remote::gClientThread->mTest1QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,4.0);
   double tX = aCmd->argDouble(1);

   Prn::print(0, "***********************");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   int* tPtr = 0;
   *tPtr = 123;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Remote::gSettings.show();
}

