#pragma once

/*==============================================================================
Prototype communications message classes.
Parameters class whose values are read from a command file. 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Remote
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that contains parameter member variables. The values of
// the parameters are set by reading a text file that contains command lines.
// Each command line is of the form "command argument1 argument2 ...".
// 
// The command files are partitioned into different sections and only one
// section can be read at a time to set member variables that are specified
// in it.
//
// The command files are managed by a CmdLineFile object. This opens the 
// file, reads each line in it, parses the line into a CmdLineCmd command 
// object, passes the command object to this object for command execution,
// and then closes the file. 
//
// This class inherits from BaseCmdLineParms, which inherits from 
// BaseCmdLineExec. BaseCmdLineParms provides a method that uses a
// CmdLineFile object to read and process the file. BaseCmdLineExec provides
// an abstract execute(cmd) method to which inheritors provide an overload
// that is called by the CmdLineFile object for each command in the file.
// This execute method then sets a member variables, according to the
// command.
// 
// This class can contain member variables that also inherit from
// BaseCmdLineExec. This provides for command files that have a nested
// structure. If so, then this class is the root.
// 

class Settings : public Ris::BaseCmdLineParms
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Communications node type.
   static const int cNone       = 0;
   static const int cTcpServer  = 1;
   static const int cTcpClient  = 2;

   static const int cMaxStringSize = 30;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members that are read from the parms file.

   // Application settings.
   int  mMyAppNumber;
   int  mMyAppRole;

   // TCP server address and port.
   char mTcpServerIPAddress[cMaxStringSize];
   int  mTcpServerPort;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Expanded members that are not read from the parms file.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructor,
   typedef Ris::BaseCmdLineParms BaseClass;
   Settings();
   void reset();
   void show();

   // Base class override: Execute a command from the command file to set a 
   // member variable. This is called by the associated command file object
   // for each command in the file.
   void execute(Ris::CmdLineCmd* aCmd) override;

   // Calculate expanded member variables. This is called after the entire
   // section of the command file has been processed.
   void expand() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Helpers.

   static char* asStringAppRole (int aX);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _PROCOSETTINGS_CPP_
   Settings gSettings;
#else
   extern Settings gSettings;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace