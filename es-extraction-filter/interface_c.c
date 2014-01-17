

/* this ALWAYS GENERATED file contains the RPC client stubs */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "interface_h.h"

#define TYPE_FORMAT_STRING_SIZE   77                                
#define PROC_FORMAT_STRING_SIZE   611                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _interface_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } interface_MIDL_TYPE_FORMAT_STRING;

typedef struct _interface_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } interface_MIDL_PROC_FORMAT_STRING;

typedef struct _interface_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } interface_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const interface_MIDL_TYPE_FORMAT_STRING interface__MIDL_TypeFormatString;
extern const interface_MIDL_PROC_FORMAT_STRING interface__MIDL_ProcFormatString;
extern const interface_MIDL_EXPR_FORMAT_STRING interface__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: __MIDL_itf_interface_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: InterfaceName, ver. 1.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x12,0x34,0x56,0x78,0x9A,0xBC}} */



static const RPC_CLIENT_INTERFACE InterfaceName___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x00000000,0x0000,0x0000,{0x00,0x00,0x12,0x34,0x56,0x78,0x9A,0xBC}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE InterfaceName_v1_0_c_ifspec = (RPC_IF_HANDLE)& InterfaceName___RpcClientInterface;

extern const MIDL_STUB_DESC InterfaceName_StubDesc;

static RPC_BINDING_HANDLE InterfaceName__MIDL_AutoBindHandle;


TRANS_HANDLE Connect( 
    /* [in] */ handle_t IDL_handle,
    /* [string][in] */ unsigned char *szOutput)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&IDL_handle);
    return ( TRANS_HANDLE  )_RetVal.Pointer;
    
}


void Close( 
    /* [out][in] */ TRANS_HANDLE *ppTr)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[40],
                  ( unsigned char * )&ppTr);
    
}


int OpenNewSegFile( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ int index)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[76],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


int CloseSegFile( 
    /* [in] */ TRANS_HANDLE hTr)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[124],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


void DoEncode( 
    /* [in] */ TRANS_HANDLE hTr)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[166],
                  ( unsigned char * )&hTr);
    
}


int SetVideoType( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [full][in] */ const VIDEO_MEDIA_TYPE *_guid,
    /* [in] */ __int64 fps,
    /* [in] */ int width,
    /* [in] */ int height)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[202],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


int SetPicturesData( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in][size_is] */ unsigned char pData[  ],
    /* [in] */ int numbers)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[268],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


int SetPictureInfo( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ __int64 st,
    /* [in] */ __int64 et,
    /* [in] */ int data_len,
    /* [in] */ int bKeyFrame)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[322],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


/* [async] */ void  AsyncFunc( 
    /* [in] */ PRPC_ASYNC_STATE AsyncFunc_AsyncHandle,
    /* [in] */ TRANS_HANDLE hTr)
{

    NdrAsyncClientCall(
                      ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                      (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[388],
                      ( unsigned char * )&AsyncFunc_AsyncHandle);
    
}


int OpenCompressedData( 
    /* [in] */ TRANS_HANDLE hTr)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[424],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


void CloseCompressedData( 
    /* [in] */ TRANS_HANDLE hTr)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[466],
                  ( unsigned char * )&hTr);
    
}


int GetCompressedDataInfo( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [out] */ __int64 *numbers)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[502],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


int GetCompressedData( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ int nRead,
    /* [out] */ int *pSize,
    /* [size_is][out][in] */ unsigned char pData[  ])
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&InterfaceName_StubDesc,
                  (PFORMAT_STRING) &interface__MIDL_ProcFormatString.Format[550],
                  ( unsigned char * )&hTr);
    return ( int  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   [async] attribute, /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const interface_MIDL_PROC_FORMAT_STRING interface__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Connect */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x24 ),	/* 36 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 28 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter szOutput */

/* 34 */	NdrFcShort( 0x30 ),	/* Flags:  out, return, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Procedure Close */


	/* Return value */

/* 40 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x1 ),	/* 1 */
/* 48 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 50 */	0x30,		/* FC_BIND_CONTEXT */
			0xe0,		/* Ctxt flags:  via ptr, in, out, */
/* 52 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 54 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 56 */	NdrFcShort( 0x38 ),	/* 56 */
/* 58 */	NdrFcShort( 0x38 ),	/* 56 */
/* 60 */	0x40,		/* Oi2 Flags:  has ext, */
			0x1,		/* 1 */
/* 62 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppTr */

/* 70 */	NdrFcShort( 0x118 ),	/* Flags:  in, out, simple ref, */
/* 72 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 74 */	NdrFcShort( 0xe ),	/* Type Offset=14 */

	/* Procedure OpenNewSegFile */

/* 76 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 78 */	NdrFcLong( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x2 ),	/* 2 */
/* 84 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 86 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 88 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 90 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 92 */	NdrFcShort( 0x2c ),	/* 44 */
/* 94 */	NdrFcShort( 0x8 ),	/* 8 */
/* 96 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 98 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 106 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 108 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 110 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter index */

/* 112 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 114 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 118 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 120 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloseSegFile */

/* 124 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 126 */	NdrFcLong( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x3 ),	/* 3 */
/* 132 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 134 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 136 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 138 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 140 */	NdrFcShort( 0x24 ),	/* 36 */
/* 142 */	NdrFcShort( 0x8 ),	/* 8 */
/* 144 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 146 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 154 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 156 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 158 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Return value */

/* 160 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoEncode */

/* 166 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 168 */	NdrFcLong( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0x4 ),	/* 4 */
/* 174 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 176 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 178 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 180 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 182 */	NdrFcShort( 0x24 ),	/* 36 */
/* 184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 186 */	0x40,		/* Oi2 Flags:  has ext, */
			0x1,		/* 1 */
/* 188 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 196 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 198 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 200 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Procedure SetVideoType */

/* 202 */	0x0,		/* 0 */
			0x49,		/* Old Flags:  full ptr, */
/* 204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x5 ),	/* 5 */
/* 210 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 212 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 214 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 216 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 218 */	NdrFcShort( 0x88 ),	/* 136 */
/* 220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 222 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 224 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 232 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 234 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 236 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter _guid */

/* 238 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 240 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 242 */	NdrFcShort( 0x16 ),	/* Type Offset=22 */

	/* Parameter fps */

/* 244 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 246 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 248 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter width */

/* 250 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 252 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 254 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter height */

/* 256 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 258 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 262 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 264 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPicturesData */

/* 268 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 270 */	NdrFcLong( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x6 ),	/* 6 */
/* 276 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 278 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 280 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 282 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 284 */	NdrFcShort( 0x2c ),	/* 44 */
/* 286 */	NdrFcShort( 0x8 ),	/* 8 */
/* 288 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 290 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x1 ),	/* 1 */
/* 296 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 298 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 300 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 302 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter pData */

/* 304 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 306 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 308 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter numbers */

/* 310 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 312 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 314 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 316 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 318 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 320 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPictureInfo */

/* 322 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 324 */	NdrFcLong( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x7 ),	/* 7 */
/* 330 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 332 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 334 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 336 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 338 */	NdrFcShort( 0x54 ),	/* 84 */
/* 340 */	NdrFcShort( 0x8 ),	/* 8 */
/* 342 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 344 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 352 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 354 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 356 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter st */

/* 358 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 360 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 362 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter et */

/* 364 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 366 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 368 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter data_len */

/* 370 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 372 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 374 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bKeyFrame */

/* 376 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 378 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 384 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AsyncFunc */

/* 388 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 396 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 398 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 402 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 404 */	NdrFcShort( 0x24 ),	/* 36 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	0xc0,		/* Oi2 Flags:  has ext, has async handle */
			0x1,		/* 1 */
/* 410 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 418 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 420 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 422 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Procedure OpenCompressedData */

/* 424 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 426 */	NdrFcLong( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0x9 ),	/* 9 */
/* 432 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 434 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 436 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 438 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 440 */	NdrFcShort( 0x24 ),	/* 36 */
/* 442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 444 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 446 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 454 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 456 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 458 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Return value */

/* 460 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 462 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 464 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CloseCompressedData */

/* 466 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 468 */	NdrFcLong( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0xa ),	/* 10 */
/* 474 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 476 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 478 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 480 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 482 */	NdrFcShort( 0x24 ),	/* 36 */
/* 484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 486 */	0x40,		/* Oi2 Flags:  has ext, */
			0x1,		/* 1 */
/* 488 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 496 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 498 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 500 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Procedure GetCompressedDataInfo */

/* 502 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 504 */	NdrFcLong( 0x0 ),	/* 0 */
/* 508 */	NdrFcShort( 0xb ),	/* 11 */
/* 510 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 512 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 514 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 516 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 518 */	NdrFcShort( 0x24 ),	/* 36 */
/* 520 */	NdrFcShort( 0x2c ),	/* 44 */
/* 522 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 524 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 532 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 534 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 536 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter numbers */

/* 538 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 540 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 542 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Return value */

/* 544 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 546 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCompressedData */

/* 550 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 552 */	NdrFcLong( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0xc ),	/* 12 */
/* 558 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 560 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* Ctxt flags:  in, */
/* 562 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 564 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 566 */	NdrFcShort( 0x2c ),	/* 44 */
/* 568 */	NdrFcShort( 0x24 ),	/* 36 */
/* 570 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 572 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 574 */	NdrFcShort( 0x1 ),	/* 1 */
/* 576 */	NdrFcShort( 0x1 ),	/* 1 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hTr */

/* 580 */	NdrFcShort( 0x8 ),	/* Flags:  in, */
/* 582 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 584 */	NdrFcShort( 0x12 ),	/* Type Offset=18 */

	/* Parameter nRead */

/* 586 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 588 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pSize */

/* 592 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 594 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pData */

/* 598 */	NdrFcShort( 0x1b ),	/* Flags:  must size, must free, in, out, */
/* 600 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 602 */	NdrFcShort( 0x40 ),	/* Type Offset=64 */

	/* Return value */

/* 604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 606 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const interface_MIDL_TYPE_FORMAT_STRING interface__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	0x30,		/* FC_BIND_CONTEXT */
			0x30,		/* Ctxt flags:  out, ret, */
/*  8 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 10 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	0x30,		/* FC_BIND_CONTEXT */
			0xe1,		/* Ctxt flags:  via ptr, in, out, can't be null */
/* 16 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 18 */	0x30,		/* FC_BIND_CONTEXT */
			0x41,		/* Ctxt flags:  in, can't be null */
/* 20 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 22 */	
			0x14, 0x0,	/* FC_FP */
/* 24 */	NdrFcShort( 0x8 ),	/* Offset= 8 (32) */
/* 26 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 28 */	NdrFcShort( 0x8 ),	/* 8 */
/* 30 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 32 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 34 */	NdrFcShort( 0x10 ),	/* 16 */
/* 36 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 38 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 40 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (26) */
			0x5b,		/* FC_END */
/* 44 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 46 */	NdrFcShort( 0x1 ),	/* 1 */
/* 48 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 50 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 52 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 54 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 56 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 58 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 60 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 62 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 64 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 66 */	NdrFcShort( 0x1 ),	/* 1 */
/* 68 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x0,		/*  */
/* 70 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 72 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 74 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short InterfaceName_FormatStringOffsetTable[] =
    {
    0,
    40,
    76,
    124,
    166,
    202,
    268,
    322,
    388,
    424,
    466,
    502,
    550
    };


static const MIDL_STUB_DESC InterfaceName_StubDesc = 
    {
    (void *)& InterfaceName___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &InterfaceName__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    interface__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x8000253, /* MIDL Version 8.0.595 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

