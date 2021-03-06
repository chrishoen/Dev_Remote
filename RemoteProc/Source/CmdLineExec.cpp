
#include "stdafx.h"

#include "remoteSettings.h"
#include "remoteServerThread.h"
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
   if (aCmd->isCmd("TP"))      Remote::gServerThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TX"))      executeTx(aCmd);
   if (aCmd->isCmd("ECHO"))    executeEcho(aCmd);
   if (aCmd->isCmd("GO1"))     executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))     executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))     executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))     executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))     executeGo5(aCmd);
   if (aCmd->isCmd("Parms"))   executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0);
   int tSessionIndex = aCmd->argInt(1);

   Remote::TestMsg* tMsg = new Remote::TestMsg;
   Remote::gServerThread->sendMsg(tSessionIndex,tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   aCmd->setArgDefault(2, 0);
   int tSessionIndex = aCmd->argInt(1);
   int tNumWords = aCmd->argInt(2);

   Remote::EchoRequestMsg* tMsg = new Remote::EchoRequestMsg;
   Remote::gServerThread->sendMsg(tSessionIndex, tMsg);
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

