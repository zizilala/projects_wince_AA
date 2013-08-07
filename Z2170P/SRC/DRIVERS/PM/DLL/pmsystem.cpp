// All rights reserved ADENEO EMBEDDED 2010
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//

//
// This module contains platform-independent code for handling system power state 
// transitions.
//

#include <pmimpl.h>
#include <nkintr.h>

// This thread initializes the system power state management thread.  The actual
// work of deciding when and how to manage system power state transitions is managed
// in OEM-customizable routines in another module.
DWORD WINAPI 
SystemThreadProc(LPVOID lpvParam)
{
    HANDLE hevReady = (HANDLE) lpvParam;
    INT iPriority;

#ifndef SHIP_BUILD
    SETFNAME(_T("SystemThreadProc"));
#endif

    PMLOGMSG(ZONE_INIT, (_T("+%s: thread 0x%08x\r\n"), pszFname, GetCurrentThreadId()));

    // set the thread priority
    if(!GetPMThreadPriority(_T("SystemPriority256"), &iPriority)) {
        iPriority = DEF_SYSTEM_THREAD_PRIORITY;
    }
    CeSetThreadPriority(GetCurrentThread(), iPriority);

    // Call the platform routine to manage system power state transitions
    // and power notifications.  This routine implements a loop that should never
    // return.
    PlatformManageSystemPower(hevReady);

    PMLOGMSG(ZONE_INIT | ZONE_WARN, (_T("-%s: exiting\r\n"), pszFname));
    return 0;
}
