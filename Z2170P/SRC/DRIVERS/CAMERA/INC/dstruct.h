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

#ifndef _DSTRUCT_H
#define _DSTRUCT_H

#define dim(x) (sizeof(x) / sizeof(x[0]))

#define SAFEDELETE( pointer ) \
    if ( NULL != pointer )    \
    {                         \
        delete pointer;       \
        pointer = NULL;       \
    }


typedef struct _Supported_Video_Formats
{
    GUID                categoryGUID;
    ULONG               ulAvailFormats;
    PCS_DATARANGE_VIDEO *pCsDataRangeVideo;
} PINVIDEOFORMAT, * PPINVIDEOFORMAT;

typedef struct _Sensor_Property 
{
    ULONG                     ulDefaultValue;
    ULONG                     ulCurrentValue;
    ULONG                     ulFlags;
    ULONG                     ulCapabilities;
    PCSPROPERTY_VALUES        pCsPropValues;
    PCSPROPERTY_STEPPING_LONG pRangeNStep;
    BOOL                      fGetSupported;
    BOOL                      fSetSupported;
} SENSOR_PROPERTY, * PSENSOR_PROPERTY;

typedef struct _Video_Control_Caps
{
    ULONG DefaultVideoControlCaps;
    ULONG CurrentVideoControlCaps;
} VIDCONTROLCAPS, *PVIDCONTROLCAPS;

class CPinDevice;

typedef struct _StreamInstances
{
    DWORD        dwSize;
    ULONG        ulCInstances;
    ULONG        ulPossibleCount;
    CSSTATE      CsPrevState;
    VIDCONTROLCAPS   VideoCaps;
    PPINVIDEOFORMAT   pVideoFormat;
    CPinDevice * pPinDev;
} STREAM_INSTANCES, * PSTREAM_INSTANCES;


typedef struct _SensorModeInfo
{
    DWORD dwSize;
    ULONG MemoryModel;          // Memory model to be used for this sensor mode. Allowed values are 
                                // CSPROPERTY_BUFFER_CLIENT_LIMITED, CSPROPERTY_BUFFER_CLIENT_UNLIMITED
                                // and CSPROPERTY_BUFFER_DRIVER.
    ULONG MaxNumOfBuffers;      // Max Number of buffers of buffers for this sensor mode.
    ULONG PossibleCount;        // Max Number of Instances of this sensor Mode that this PDD/MDD support
                                // Usually set to 1;
    VIDCONTROLCAPS   VideoCaps; // VideoControl Caps corresponding to the sensor mode    
    PPINVIDEOFORMAT   pVideoFormat;// All the Video Formats supported by the sensor mode

} SENSORMODEINFO, *PSENSORMODEINFO;

typedef struct _AdapterInfo
{
    DWORD dwSize;
    ULONG ulCTypes;     // Total number of senosr modes implemented by this camera
    ULONG ulVersionID;  // The version number of MDD/PDD interface that the PDD implements    
    POWER_CAPABILITIES PowerCaps; // Power Capabilities   
    SENSOR_PROPERTY     SensorProps[NUM_PROPERTY_ITEMS]; // All ProcAmp and CameraControl props
} ADAPTERINFO, *PADAPTERINFO;


typedef struct __PDD_FuncTbl {
    DWORD dwSize;
    PVOID (*PDD_Init)( PVOID MDDContext, __PDD_FuncTbl * pPDDFuncTbl );
    DWORD (*PDD_DeInit)( LPVOID PDDContext );
    DWORD (*PDD_GetAdapterInfo)( LPVOID PDDContext, PADAPTERINFO pAdapterInfo );
    DWORD (*PDD_HandleVidProcAmpChanges)( LPVOID PDDContext, DWORD dwPropId, LONG lFlags, LONG lValue);
    DWORD (*PDD_HandleCamControlChanges)( LPVOID PDDContext, DWORD dwPropId, LONG lFlags, LONG lValue );
    DWORD (*PDD_HandleVideoControlCapsChanges)( LPVOID PDDContext, LONG lModeType ,ULONG ulCaps );
    DWORD (*PDD_SetPowerState)( LPVOID PDDContext, CEDEVICE_POWER_STATE PowerState );
    DWORD (*PDD_HandleAdapterCustomProperties)( LPVOID PDDContext, PUCHAR pInBuf, DWORD  InBufLen, PUCHAR pOutBuf, DWORD  OutBufLen, PDWORD pdwBytesTransferred );
    DWORD (*PDD_InitSensorMode)( LPVOID PDDContext, ULONG ulModeType, LPVOID ModeContext );
    DWORD (*PDD_DeInitSensorMode)( LPVOID PDDContext, ULONG ulModeType );
    DWORD (*PDD_SetSensorState)( LPVOID PDDContext, ULONG ulModeType, CSSTATE CsState );
    DWORD (*PDD_GetSensorModeInfo)( LPVOID PDDContext, ULONG ulModeType, PSENSORMODEINFO pSensorModeInfo );
    DWORD (*PDD_SetSensorModeFormat)( LPVOID PDDContext, ULONG ulModeType, PCS_DATARANGE_VIDEO pCsDataRangeVideo );
    PVOID (*PDD_AllocateBuffer)( LPVOID PDDContext, ULONG ulModeType );
    DWORD (*PDD_DeAllocateBuffer)( LPVOID PDDContext, ULONG ulModeType, PVOID pBuffer );
    DWORD (*PDD_RegisterClientBuffer)( LPVOID PDDContext, ULONG ulModeType, PVOID pBuffer );
    DWORD (*PDD_UnRegisterClientBuffer)( LPVOID PDDContext, ULONG ulModeType, PVOID pBuffer );
    DWORD (*PDD_FillBuffer)( LPVOID PDDContext, ULONG ulModeType, PUCHAR pImage );
    DWORD (*PDD_HandleModeCustomProperties)( LPVOID PDDContext, ULONG ulModeType, PUCHAR pInBuf, DWORD  InBufLen, PUCHAR pOutBuf, DWORD  OutBufLen, PDWORD pdwBytesTransferred );
} PDDFUNCTBL, *PPDDFUNCTBL;

#endif
