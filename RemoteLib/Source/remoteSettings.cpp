//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _REMOTESETTINGS_CPP_
#include "remoteSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Settings::Settings()
{
   reset();
}

void Settings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "Remote_Settings.txt");

   mMyAppNumber = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;
   mTcpMaxSessions = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Settings::show()
{
   printf("\n");
   printf("Settings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber             %16d\n",       mMyAppNumber);

   printf("TcpServer               %16s : %5d\n", mTcpServerIPAddress, mTcpServerPort);
   printf("TcpMaxSessions          %16d\n",       mTcpMaxSessions);

   printf("Settings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("MyAppNumber"))   mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("TcpServer"))
   {
      aCmd->copyArgString(1, mTcpServerIPAddress,cMaxStringSize);
      mTcpServerPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("TcpMaxSessions")) mTcpMaxSessions = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void Settings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace