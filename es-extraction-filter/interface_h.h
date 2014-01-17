

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Fri Jan 17 00:30:25 2014
 */
/* Compiler settings for ..\es_rpc_server\interface.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __interface_h_h__
#define __interface_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_interface_0000_0000 */
/* [local] */ 

typedef struct _VIDEO_MEDIA_TYPE
    {
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[ 8 ];
    } 	VIDEO_MEDIA_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_interface_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_interface_0000_0000_v0_0_s_ifspec;

#ifndef __InterfaceName_INTERFACE_DEFINED__
#define __InterfaceName_INTERFACE_DEFINED__

/* interface InterfaceName */
/* [version][uuid] */ 

typedef /* [context_handle] */ void *TRANS_HANDLE;

#define	DEFAULT_ASYNC_DELAY	( 100 )

#define	APP_ERROR	( -1 )

#define	DEFAULT_PROTOCOL_SEQUENCE	( "ncacn_ip_tcp" )

#define	DEFAULT_ENDPOINT	( "8765" )

TRANS_HANDLE Connect( 
    /* [in] */ handle_t IDL_handle,
    /* [string][in] */ unsigned char *szOutput);

void Close( 
    /* [out][in] */ TRANS_HANDLE *ppTr);

int OpenNewSegFile( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ int index);

int CloseSegFile( 
    /* [in] */ TRANS_HANDLE hTr);

void DoEncode( 
    /* [in] */ TRANS_HANDLE hTr);

int SetVideoType( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [full][in] */ const VIDEO_MEDIA_TYPE *_guid,
    /* [in] */ __int64 fps,
    /* [in] */ int width,
    /* [in] */ int height);

int SetPicturesData( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in][size_is] */ unsigned char pData[  ],
    /* [in] */ int numbers);

int SetPictureInfo( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ __int64 st,
    /* [in] */ __int64 et,
    /* [in] */ int data_len,
    /* [in] */ int bKeyFrame);

/* [async] */ void  AsyncFunc( 
    /* [in] */ PRPC_ASYNC_STATE AsyncFunc_AsyncHandle,
    /* [in] */ TRANS_HANDLE hTr);

int OpenCompressedData( 
    /* [in] */ TRANS_HANDLE hTr);

void CloseCompressedData( 
    /* [in] */ TRANS_HANDLE hTr);

int GetCompressedDataInfo( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [out] */ __int64 *numbers);

int GetCompressedData( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ int nRead,
    /* [out] */ int *pSize,
    /* [size_is][out][in] */ unsigned char pData[  ]);



extern RPC_IF_HANDLE InterfaceName_v1_0_c_ifspec;
extern RPC_IF_HANDLE InterfaceName_v1_0_s_ifspec;
#endif /* __InterfaceName_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

void __RPC_USER TRANS_HANDLE_rundown( TRANS_HANDLE );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


