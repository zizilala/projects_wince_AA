// All rights reserved ADENEO EMBEDDED 2010
//-----------------------------------------------------------------------------
//! \addtogroup	BOOTLOADER
//! @{
//!
//  All rights reserved ADENEO 2007
//-----------------------------------------------------------------------------
//! \file		AT91SAM9M10EK/SRC/BOOTLOADER/EBOOT/mmu.c
//!
//! \brief		Virtual/Physicall address translation
//!  Duplicata of memory.c provided by Microsoft except that there's no debug message.
//!  This is because at some point early in eboot execution we call those functions before every thing is set up in RAM and then debug messages break.
//!  Memory interface routines.
//!
//! \if subversion
///   @URL: $URL: http://centaure/svn/interne-ce_bsp_atmel/TRUNK60/PLATFORM/AT91SAM9M10EK/SRC/BOOTLOADER/EBOOT/mmu.c $
//!   @author $Author: pgal $
//!   @version $Revision: 1247 $
//!   @date $Date: 2007-08-09 10:28:06 +0200 (jeu., 09 août 2007) $
//! \endif
//-----------------------------------------------------------------------------
//! \addtogroup EBOOT
//! @{
//!
#pragma warning (push)
#pragma warning (disable:4214 4201 4213 4115)
#include <windows.h>
#include <oal.h>
#include <oal_log.h>
#include <oal_memory.h>
#pragma warning (pop)
#include "omap_cpuver.h"
 

// This table is define in << cfg.inc >>
extern OAL_ADDRESS_TABLE g_oalAddressTable[];
extern OAL_ADDRESS_TABLE g_oalAddressTableHynix[];

extern UINT32 g_CPUFamily;

//--------------------------------------------------------------------------------
//! \fn VOID* OALPAtoVA(UINT32 pa, BOOL cached)
//!
//!  Converts a physical address (PA) to a virtual address (VA). This routine
//!  uses the OEMAddressTable defined in the platform.
//!  THIS FUNCTION MUSTN T WRITE ANY DEBUG MESSAGE
//!
//! \param pa the physical address
//!
//! \param cached	return the cached address if set, uncached address otherwise
//!
//! \return the virtual address if found, NULL otherwise
//!
//--------------------------------------------------------------------------------
VOID* OALPAtoVA(UINT32 pa, BOOL cached)
{
    OAL_ADDRESS_TABLE *pTable = NULL;    
    VOID *va = NULL;

    if(g_CPUFamily == CPU_FAMILY_DM37XX)
    {
        pTable =   g_oalAddressTableHynix; 
    }
    else if (g_CPUFamily == CPU_FAMILY_OMAP35XX)
    {
        pTable =   g_oalAddressTable; 
    }
    else
    {
        OALLog(L"INFO: UnKnown CPU family:%d....\r\n", g_CPUFamily);
        pTable =   g_oalAddressTable; 
    }
    // Search the table for address range
    while (pTable->size != 0) {
        if (
            pa >= pTable->PA && 
            pa <= (pTable->PA + (pTable->size << 20) - 1)
        ) break;                      // match found 
        pTable++;
    }

    // If address table entry is valid compute the VA
    if (pTable->size != 0) {
        va = (VOID *)(pTable->CA + (pa - pTable->PA));
        // If VA is uncached, set the uncached bit
        if (!cached) va = (VOID*) ((DWORD) va | OAL_MEMORY_CACHE_BIT);
    }

    return va;
}

//--------------------------------------------------------------------------------
//! \fn UINT32 OALVAtoPA(VOID *pVA)
//!
//!  Converts a virtual address (VA) to a physical address (PA). This routine
//!  uses the OEMAddressTable defined in the platform.
//!  THIS FUNCTION MUSTN T WRITE ANY DEBUG MESSAGE
//!
//! \param pVA the virtual address
//!
//! \return the physicall address if founf, 0 otherwise
//!
//--------------------------------------------------------------------------------
UINT32 OALVAtoPA(VOID *pVA)
{
    OAL_ADDRESS_TABLE *pTable = NULL;
    UINT32 va = (UINT32)pVA;
    UINT32 pa = 0;

    if(g_CPUFamily == CPU_FAMILY_DM37XX)
    {
        pTable =   g_oalAddressTableHynix; 
    }
    else if (g_CPUFamily == CPU_FAMILY_OMAP35XX)
    {
        pTable =   g_oalAddressTable; 
    }
    else
    {
        OALLog(L"INFO: UnKnown CPU family:%d....\r\n", g_CPUFamily);
        pTable =   g_oalAddressTable; 
    }

    // Virtual address must be in CACHED or UNCACHED regions.
    if (va < 0x80000000 || va >= 0xC0000000)
	{
        goto cleanUp;
    }

    // Address must be cached, as entries in OEMAddressTable are cached address.
    va = va&~OAL_MEMORY_CACHE_BIT;

    // Search the table for address range
    while (pTable->size != 0) {
        if (va >= pTable->CA && va <= pTable->CA + (pTable->size << 20) - 1) {
            break;
        }
        pTable++;
    }

    // If address table entry is valid compute the PA
    if (pTable->size != 0) pa = pTable->PA + va - pTable->CA;

cleanUp:
    // Indicate physical address
    return pa;
}

//------------------------------------------------------------------------------
//! End of $URL: http://centaure/svn/interne-ce_bsp_atmel/TRUNK60/PLATFORM/AT91SAM9M10EK/SRC/BOOTLOADER/EBOOT/mmu.c $
//------------------------------------------------------------------------------

//
//! @}
//
//
//! @}
//
