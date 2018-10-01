#include "stdafx.h"

#include "risThreadsProcess.h"
#include "remoteSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::SocketInit1,    true);
   Prn::setFilter(Prn::SocketInit2,   false);
   Prn::setFilter(Prn::SocketError1,   true);
   Prn::setFilter(Prn::SocketError2,   true);
   Prn::setFilter(Prn::SocketRun1,    false);
   Prn::setFilter(Prn::SocketRun2,    false);
   Prn::setFilter(Prn::SocketRun3,    false);
   Prn::setFilter(Prn::SocketRun4,    false);

   Prn::setFilter(Prn::ThreadInit1,    true);
   Prn::setFilter(Prn::ThreadInit1,    true);
   Prn::setFilter(Prn::ThreadRun1,     true);
   Prn::setFilter(Prn::ThreadRun2,    false);
   Prn::setFilter(Prn::ThreadRun3,    false);
   Prn::setFilter(Prn::ThreadRun4,    false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   Remote::gSettings.readSection("TcpClient1");
   Remote::gSettings.show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0, "LocalProc Client************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0, "LocalProc Client************************************END");

   // Close print.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
