//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"


#define  _PROCOSETTINGS_CPP_
#include "remoteSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
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
   mMyAppRole = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Settings::show()
{
   printf("\n");
   printf("Settings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber          %3d\n", mMyAppNumber);
   printf("MyAppRole            %5s\n", asStringAppRole(mMyAppRole));

   printf("TcpServer            %-12s   %5d\n",mTcpServerIPAddress,mTcpServerPort);
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

   if (aCmd->isCmd("MyAppRole"))
   {
      if (aCmd->isArgString(1,asStringAppRole(cTcpServer)))   mMyAppRole = cTcpServer;
      if (aCmd->isArgString(1,asStringAppRole(cTcpClient)))   mMyAppRole = cTcpClient;
   }

   if (aCmd->isCmd("TcpServer"))
   {
      aCmd->copyArgString(1, mTcpServerIPAddress,cMaxStringSize);
      mTcpServerPort = aCmd->argInt(2);
   }
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

char* Settings::asStringAppRole(int aX)
{
   switch (aX)
   {
   case cNone              : return "None";
   case cTcpServer         : return "TcpServer";
   case cTcpClient         : return "TcpClient";
   default : return "UNKNOWN";
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace