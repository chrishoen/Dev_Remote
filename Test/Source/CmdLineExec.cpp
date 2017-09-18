
#include "stdafx.h"

#include "CmdLineExec.h"

using namespace std;

// change11202
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
   mX=101.123456;
   mT=0.1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);

   if(aCmd->isCmd("X"      ))  mX=aCmd->argDouble(1);
   if(aCmd->isCmd("T"      ))  mT=aCmd->argDouble(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,4.0);
   double tX = aCmd->argDouble(1);

   int tN = int(round(tX));
   Prn::print(0, "***********************");
   Prn::print(0, "X   %10.6f",tX);
   Prn::print(0, "N   %10d",  tN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,4.0);
   double tX = aCmd->argDouble(1);

   int tN = int(round(tX));
   Prn::print(0, "***********************");
   Prn::print(0, "X   %10.6f",tX);
   Prn::print(0, "N   %10d",  tN);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0.1);

   double tX = 101.123456;
   double tT = aCmd->argDouble(1);
   double tR = fmod(tX,tT);
   double tY = tX - tR;

   Prn::print(0, "***********************");
   Prn::print(0, "X   %10.6f",tX);
   Prn::print(0, "T   %10.6f",tT);
   Prn::print(0, "R   %10.6f",tR);
   Prn::print(0, "Y   %10.6f",tY);
   Prn::print(0, "");
   Prn::print(0, "D   %10.6f",tX-tY);
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,true);

   bool tX = aCmd->argBool(1);

   Prn::print(0, "X %d",tX);

   tX ^= true;

   Prn::print(0, "X %d",tX);
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0.0);
   double tX = aCmd->argDouble(1);
   double tY = sqrt(tX);
   Prn::print(0, "XY   %10.6f %10.6f",tX,tY);
   Prn::print(0, "norm %10d",isnormal(tY));
}

