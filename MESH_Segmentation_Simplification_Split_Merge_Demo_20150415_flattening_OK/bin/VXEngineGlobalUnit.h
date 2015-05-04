/**
 * @mainpage    Manager Frame of VXFramework
 *
 * @section intro 소개
 *      - VXFramework의 Manager Frame을 이루는 Global Data Structures, Helper Functions 그리고 VXEngine APIs를 설명한다.
 *
 * @section CREATEINFO 작성정보
 *      - 작성자      :   김동준
 *      - 작성일      :   2012/8/6
 *      - Contact     :   korfriend@gmail.com
 *
 * @section MODIFYINFO 수정정보
 *      - 2012.7.27    :   이 일짜 소스를 기준으로 VXFramework Doxygen 0.0.1 최초 작성
 *      - 2012.7.30    :   namespace [vxenums, vxmathproj] 문서 추가 / 완료
 *      - 2012.7.31    :   namespace [vxhelpers] 문서 추가 / 완료
 *      - 2012.8.1     :   namespace [vxengineapi] 문서 추가, vxenums, vxmathproj, vxhelpers 문서 보완
 *      - 2012.8.2     :   namespace [vxengineapi] 문서 완료, [vxobjects] 문서 추가
 *      - 2012.8.3     :   namespace [vxobjects] 문서 완료, vxengineapi 문서 보완, class library refactoring
 *      - 2012.8.6     :   namespace [vxgpuinterfaces] 문서 완료, 그 외 internal 자료구조에 대한 문서 작업 완료
 *      - 2012.8.15    :   vxengineapi::VXEVolumeSetVolumeInfo 추가, vxenums::EnumVXRMetricType 제거 및 관련 자료 구조 수정
 *      - 2012.11.26    :  Entire Framework is refactored
 */
 
/**
 * @file VXEngineGlobalUnit.h
 * @brief Global Data Structures 및 Classes 그리고 Helper Functions 선언된 파일.
 * @section Include & Link 정보
 *		- Include : VXEngineGlobalUnit.h
 *		- Library : VXEngineGlobalUnit.lib
 *		- Linking Binary : VXEngineGlobalUnit.dll
 */
 
#pragma once

#define __WINDOWS//__MACOS//, __LINUX

#include <map>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <float.h>
#include <math.h>

#ifdef __WINDOWS
#include <windows.h>
#include <tchar.h>
#endif
#ifdef __MACOS
#include <dlfcn.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#endif

/**
 * @brief VXFramework에서 사용하는 STL(standard template library)를 위한 namespace
 */
using namespace std;

//=====================================================================
// VX Group's Data structures are available
// Please, project's character set as UNICODE
// Copyright (c) 3Dii Corporation & VPLab in SNU. All rights reserved.
//=====================================================================

//=========================
// Global Definitions
//=========================
#define VXENGINEVERSION 0x1D7F6	// 120601(allocating 20 bits) and  12 bits for modules and engine enhancement version
#define VXSAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }
#define VXSAFE_DELETEARRAY(p)	{ if(p) { delete[] (p); (p)=NULL; } }
#define VXSAFE_DELETE2DARRAY(pp, numPtrs)	{ if(pp){ for(int i = 0; i < numPtrs; i++){ VXSAFE_DELETEARRAY(pp[i]);} VXSAFE_DELETEARRAY(pp); } }
#define VXSATURATETOBYTE(src, dest)	{ dest = max(src, 0); dest = min(dest, 255); }
#define VXSATURATETOUSHORT(src, dest){ dest = max(src, 0); dest = min(dest, 65535); }
#define VXSTRUCTURENULLSET(src)	{ memset(&src, 0, sizeof(src)); }
//#define VXERRORMESSAGE(str){ ::MessageBox(NULL, _T(str), NULL, MB_OK); }
#define VXERRORMESSAGE(str){ printf("%s\n", str); }

#define VX_PI_FLOAT    ((float)  3.141592654f)
#define VX_PI_DOUBLE       3.14159265358979323846

#define STRINGPRECISION 30

//MacMac
#ifdef __MACOS
#define VXLOADLIBRARY(hModule, filename)     {int nLenModuleName = wcslen(filename) + 1; \
	char* pModuleName = new char[nLenModuleName]; \
	wcstombs(pModuleName, filename, nLenModuleName); \
	hModule = dlopen(pModuleName, RTLD_LAZY);\
	delete [] pModuleName; }
#define VXGETPROCADDRESS(pModule, pProcName)       dlsym(pModule, pProcName)
#define VXFREELIBRARY(pModule)    dlclose(pModule)
#endif
#ifdef __WINDOWS
#define VXLOADLIBRARY(hModule, filename)     hModule = LoadLibrary(filename)
#define VXGETPROCADDRESS(pModule, pProcName)    GetProcAddress(pModule, pProcName)
#define VXFREELIBRARY(pModule)    FreeLibrary(pModule)
#define VXCONVERTCHAR2WSTRING(ptr_char, wstr)    {if(ptr_char == NULL){ wstr =  _T("");}\
else{\
	string strName_ = string(ptr_char);\
	int isleng = (int)strName_.length() + 1;\
	int ileng = MultiByteToWideChar(CP_ACP, 0, strName_.c_str(),  isleng, 0, 0);\
	wstr = wstring(strName_.length(), L' ');	\
	if(strName_.length() > 0) MultiByteToWideChar(CP_ACP, 0, strName_.c_str(),  isleng, &wstr[0], ileng);}\
}
#endif

//#define __vxstatic extern __declspec(dllexport)
#ifdef __WINDOWS
#define __vxstatic extern "C" __declspec(dllexport)
#define __vxstaticclass class __declspec(dllexport)
#define __vxstaticstruct struct __declspec(dllexport)
#endif
#ifdef __MACOS
#define __vxstatic extern "C"
#define __vxstaticclass class
#define __vxstaticstruct struct
#define __cdecl __attribute__((__cdecl__))
#endif

#ifdef __WINDOWS
typedef HMODULE VXHMODULE;
#endif
#ifdef __MACOS
typedef void* VXHMODULE;
#endif

// VxDelegation //
typedef void (*VxDelegateDefault)(void* pv);

const int g_iBitMask[32] = {0x00000001,0x00000002,0x00000004,0x00000008
	,0x00000010,0x00000020,0x00000040,0x00000080
	,0x00000100,0x00000200,0x00000400,0x00000800
	,0x00001000,0x00002000,0x00004000,0x00008000
	,0x00010000,0x00020000,0x00040000,0x00080000
	,0x00100000,0x00200000,0x00400000,0x00800000
	,0x01000000,0x02000000,0x04000000,0x08000000
	,0x10000000,0x20000000,0x40000000,0x80000000};

#ifndef ushort
typedef unsigned short ushort;
#endif
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef byte
typedef unsigned char byte;
#endif
#ifndef ullong
typedef unsigned long long ullong;
#endif
#ifndef llong
typedef long long llong;
#endif

#ifndef _ttoi
#define _ttoi(s) wcstol(s, NULL, 10)
#endif
#ifndef _ttof
#define _ttof(s) wcstof(s, NULL)
#endif
#ifndef _tstof
#define _tstof(s) wcstof(s, NULL)
#endif
#ifdef _UNICODE
#ifndef _T
#define _T(s) L ## s
#endif
#else
#ifndef _T
#define _T(s) s
#endif
#endif

//=====================================
// Global Enumerations
//=====================================
/**
 * @package vxenums
 * @brief Common Data Structure로 사용되는 enumeration 을 모은 namespace
 */
namespace vxenums{
/*! VXFramework에서 지원하는 변환 좌표계의 종류, 4x4 matrix가 정의하는 Projecting 변환에서 닫혀 있음 */
enum EnumVXVCoordSpace{	
	vxvCoordSpaceSCREEN = 0,/*!< Pixel로 정의되는 Screen Space */
	vxvCoordSpacePROJECTION,/*!< Normalized Frustrum으로 정의되는 Projection Space */
	vxvCoordSpaceCAMERA,/*!< Viewing Frustrum으로 정의되는 Camera Space */
	vxvCoordSpaceWORLD,/*!< 객체가 실제로 배치되는 World Space */
	vxvCoordSpaceOBJECT/*!< 객체가 정의되는 Object Space */
};

/*! Camera의 초기 States(위치, View 및 Up Vector)에 대한 종류 */
enum EnumVXVStageViewType{
	vxvStageViewORTHOBOXOVERVIEW = 0,/*!< 3D View에서 OS의 Object Bounding Box의 대각선 방향에 대한 Overview를 정의 */
	vxvStageViewCENTERFRONT,/*!< 단면 영상에서 OS의 Object Bounding Box의 기준 Front (or Coronal) View 정의 */
	vxvStageViewCENTERRIGHT,/*!< 단면 영상에서 OS의 Object Bounding Box의 기준 Right (or Sagittal) View 정의 */
	vxvStageViewCENTERHORIZON/*!< 단면 영상에서 OS의 Object Bounding Box의 기준 Top (or Axial) View 정의 */
};

/*! Polygonal VObject를 정의하는 Primitive 종류 */
enum EnumVXRPrimitiveType{
	vxrPrimitiveTypeUNDEFINED = 0,/*!< Undefined */
	vxrPrimitiveTypeLINE,/*!< Line */
	vxrPrimitiveTypeTRIANGLE,/*!< Triangle */
	vxrPrimitiveTypePOINT/*!< Point */
};

enum EnumVXRVertexType{
	vxrVertexTypePOSITION = 0,/*!< Position defined by vxfloat3 */
	vxrVertexTypeNORMAL,/*!< Normal Vector defined by vxfloat3 */
	vxrVertexTypeTEXCOORD0,/*!< Texture Coordinate 0 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD1,/*!< Texture Coordinate 1 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD2,/*!< Texture Coordinate 2 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD3,/*!< Texture Coordinate 3 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD4,/*!< Texture Coordinate 4 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD5,/*!< Texture Coordinate 5 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD6,/*!< Texture Coordinate 6 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD7,/*!< Texture Coordinate 7 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD8,/*!< Texture Coordinate 8 defined by vxfloat3 */
	vxrVertexTypeTEXCOORD9 /*!< Texture Coordinate 9 defined by vxfloat3 */
};

/*! Module-Platform interoperation에 사용되는 VXObject의 type 종류 */
enum EnumVXRObjectType{	
	vxrObjectTypeNONE = 0,/*!< Undefined */
	vxrObjectTypeVOLUME,/*!< Volume VXObject */
	vxrObjectTypePRIMITIVE,/*!< Polygon VXObject */
	vxrObjectTypeCUSTOMLIST,/*!< 임의의 List 자료구조 기반의 inter-operation을 위한 VXObject */
	vxrObjectTypeTRANSFERFUNCTION,/*!< OTF를 정의하는 VXObject */
	vxrObjectTypeIMAGEPLANE/*!< Image Plane을 정의하는 VXObject이며 Camera Object를 갖음 */
};

/*! VXFramework에서 범용적으로 사용되는 Data types, 2010.10.15~Current Version */
/*! VXFramework에서 범용적으로 사용되는 Data types, 2012.10.18 : vxrDataTypeINT3, vxrDataTypeSTRING 추가 */
enum EnumVXRDataType{	
	vxrDataTypeUNDEFINED = 0,/*!< No Flip */
	vxrDataTypeBYTE,/*!< 1byte, unsigned char, defined as byte */
	vxrDataTypeBYTE4CHANNEL,/*!< 4bytes, unsigned char with 4 channels, defined as vxbyte4 */
	vxrDataTypeCHAR,/*!< 1byte, signed char, defined as char*/
	vxrDataTypeUNSIGNEDSHORT,/*!< 2bytes, unsigned short, defined as ushort*/
	vxrDataTypeSHORT,/*!< 2bytes, signed short, defined as short*/
	vxrDataTypeUNSIGNEDINT,/*!< 4bytes, unsigned int, defined as uint*/
	vxrDataTypeINT,/*!< 4bytes, signed int, defined as int*/
	vxrDataTypeINT2,/*!< 8bytes, signed int, defined as vxint2*/
	vxrDataTypeINT3,/*!< 12bytes, signed int, defined as vxint3*/
	vxrDataTypeFLOAT,/*!< 4bytes, float, defined as float*/
	vxrDataTypeFLOAT2CHANNEL,/*!< 8bytes, float with 2 channels, defined as vxfloat2*/
	vxrDataTypeFLOAT3CHANNEL,/*!< 12bytes, float with 3 channels, defined as vxfloat3*/
	vxrDataTypeFLOAT4CHANNEL,/*!< 16bytes, float with 4 channels, defined as vxfloat4*/
	vxrDataTypeDOUBLE,/*!< 8bytes, float, defined as double*/
	vxrDataTypeDOUBLE2CHANNEL,/*!< 16bytes, double with 2 channels, defined as vxdouble2*/
	vxrDataTypeDOUBLE3CHANNEL,/*!< 24bytes, double with 3 channels, defined as vxdouble3*/
	vxrDataTypeDOUBLE4CHANNEL,/*!< 32bytes, double with 4 channels, defined as vxdouble4*/
	vxrDataTypeBITBINARY32,/*!< 4bytes, 1bits with 32 channels, defined as int and g_iBitMask*/
	vxrDataTypeSTRUCTURED,/*!< custom defined bytes, defined as custom structure*/
	vxrDataTypeSTRING /*!< undefined bytes size, wstring is used*/
};

/*! CVXIObject에서 사용하는 Frame Buffer 종류 */
enum EnumVXRFrameBufferUsage{
	vxrFrameBufferUsageNONE = 0,/*!< Undefined, There is no allocated frame buffer */
	// Render //
	vxrFrameBufferUsageRENDEROUT,/*!< Used for rendering out buffer, the buffer should have vxrDataTypeBYTE4CHANNEL as data type */
	// Depth //
	vxrFrameBufferUsageDEPTH,/*!< Used for depth buffer, the buffer should have vxrDataTypeFLOAT as data type */
	// Custom //
	vxrFrameBufferUsageCUSTOM,/*!< Used for customized purpose, the buffer may have any type */
};

/*! VXFramework에서 사용하는 Module 종류, CVXModuleArbiter에서 Module을 관리하는 단위로 사용 */
enum EnumVXMModuleType{	
	vxmModuleTypeNONE = 0,/*!< Undefined, There is no such a module */
	vxmModuleTypeRENDER,/*!< Rendering module */
	vxmModuleTypeTRANSFERFUNCTION,/*!< OTF setting 관련 module */
	vxmModuleTypeVGENERATION,/*!< VObject 생성/처리/분석 관련 module */
	vxmModuleTypeRECONSTRUCTION,/*!< CT Reconstruction 관련 Module */
	vxmModuleTypeHELPER/*!< Customized-Define Module */
};
}
using namespace vxenums;

/**
 * @package vxmathproj
 * @brief VXFramework에서 사용되는 projection space에 대한 math 자료구조 및 math helper functions을 모은 namespace
 */
namespace vxmathproj{
/// Describes a unsigned int two-component vector
typedef struct SVXUnsignedInteger2D{
	/// x-component.
	uint x;
	/// y-component.
	uint y;
	SVXUnsignedInteger2D(){x = y = 0;}
} SVXUnsignedInteger2D;
/// Describes a unsigned int three-component vector
typedef struct SVXUnsignedInteger3D{
	/// x-component.
	uint x;
	/// y-component.
	uint y;
	/// z-component.
	uint z;
	SVXUnsignedInteger3D(){x = y = z = 0;}
} SVXUnsignedInteger3D;
/// Describes a unsigned int four-component vector
typedef struct SVXUnsignedInteger4D{
	/// x-component.
	uint x;
	/// y-component.
	uint y;
	/// z-component.
	uint z;
	/// w-component.
	uint w;
	SVXUnsignedInteger4D(){x = y = z = w = 0;}
} SVXUnsignedInteger4D;
/// Describes a unsigned short two-component vector
typedef struct SVXUnsignedShort2D{
	/// x-component.
	ushort x;
	/// y-component.
	ushort y;
	SVXUnsignedShort2D(){x = y = 0;}
} SVXUnsignedShort2D;
/// Describes a unsigned short three-component vector
typedef struct SVXUnsignedShort3D{
	/// x-component.
	ushort x;
	/// y-component.
	ushort y;
	/// z-component.
	ushort z;
	SVXUnsignedShort3D(){x = y = z = 0;}
} SVXUnsignedShort3D;
/// Describes a unsigned char two-component vector
typedef struct SVXChar2D{
	/// x-component.
	char x;
	/// y-component.
	char y;
	SVXChar2D(){x = y = 0;}
} SVXChar2D;
/// Describes a unsigned byte two-component vector
typedef struct SVXByte2D{
	/// x-component.
	byte x;
	/// y-component.
	byte y;
	SVXByte2D(){x = y = 0;}
} SVXByte2D;
/// Describes a unsigned double two-component vector
typedef struct SVXDouble2D{
	/// x-component.
	double x;
	/// y-component.
	double y;
	SVXDouble2D(){x = y = 0;}
} SVXDouble2D;
/// Describes a unsigned double three-component vector
typedef struct SVXDouble3D{
	/// x-component.
	double x;
	/// y-component.
	double y;
	/// z-component.
	double z;
	SVXDouble3D(){x = y = z = 0;}
} SVXDouble3D;
/// Describes a unsigned double four-component vector
typedef struct SVXDouble4D{
	/// x-component.
	double x;
	/// y-component.
	double y;
	/// z-component.
	double z;
	/// w-component.
	double w;
	SVXDouble4D(){x = y = z = w = 0;}
} SVXDouble4D;
/// Describes a int two-component vector
typedef struct SVXInteger2D{
	/// x-component.
	int x;
	/// y-component.
	int y;
	SVXInteger2D(){x = y = 0;}
} SVXInteger2D;
/// Describes a int three-component vector
typedef struct SVXInteger3D{
	/// x-component.
	int x;
	/// y-component.
	int y;
	/// z-component.
	int z;
	SVXInteger3D(){x = y = z = 0;}
} SVXInteger3D;
/// Describes a int four-component vector
typedef struct SVXInteger4D{
	/// x-component.
	int x;
	/// y-component.
	int y;
	/// z-component.
	int z;
	/// w-component.
	int w;
	SVXInteger4D(){x = y = z = w = 0;}
} SVXInteger4D;
/// Describes a float two-component vector
typedef struct SVXFloat2D{
	/// x-component.
	float x;
	/// y-component.
	float y;
	SVXFloat2D(){x = y = 0;}
} SVXFloat2D;
/// Describes a float three-component vector
typedef struct SVXFloat3D{
	/// x-component.
	float x;
	/// y-component.
	float y;
	/// z-component.
	float z;
	SVXFloat3D(){x = y = z = 0;}
} SVXFloat3D;
/// Describes a float four-component vector
typedef struct SVXFloat4D{
	/// x-component.
	float x;
	/// y-component.
	float y;
	/// z-component.
	float z;
	/// w-component.
	float w;
	SVXFloat4D(){x = y = z = w = 0;}
} SVXFloat4D;
/// Describes a byte four-component vector
typedef struct SVXByte4D{
	// BGRA
	/// x-component.
	byte x;
	/// y-component.
	byte y;
	/// z-component.
	byte z;
	/// w-component.
	byte w;
	SVXByte4D(){x = y = z = w = 0;}
} SVXByte4D;
/// Describes a short two-component vector
typedef struct SVXShort2D{
	/// x-component.
	short x;
	/// y-component.
	short y;
	SVXShort2D(){x = y = 0;}
} SVXShort2D;
#pragma warning (push, 2)
/// Describes a double 4x4 matrix that contains methods and operator overloads.
typedef struct SVXMatrixDouble{
	union {
		struct {
			double        _m11, _m12, _m13, _m14;
			double        _m21, _m22, _m23, _m24;
			double        _m31, _m32, _m33, _m34;
			double        _m41, _m42, _m43, _m44;

		};
		/// m[i][j](also _mij) is a component of the matrix, where i is the row number and j is the column number
		double m[4][4];
	};

	SVXMatrixDouble(){_m11 = 1; _m12 = 0; _m13 = 0; _m14 = 0; _m21 = 0; _m22 = 1; _m23 = 0; _m24 = 0;
	_m31 = 0; _m32 = 0; _m33 = 1; _m34 = 0; _m41 = 0; _m42 = 0; _m43 = 0; _m44 = 1;
	};
} SVXMatrixDouble;
/// Describes a float 4x4 matrix that contains methods and operator overloads.
typedef struct SVXMatrix{
	union {
		struct {
			float        _m11, _m12, _m13, _m14;
			float        _m21, _m22, _m23, _m24;
			float        _m31, _m32, _m33, _m34;
			float        _m41, _m42, _m43, _m44;

		};
		/// m[i][j](also _mij) is a component of the matrix, where i is the row number and j is the column number
		float m[4][4];
	};
	SVXMatrix(){_m11 = 1; _m12 = 0; _m13 = 0; _m14 = 0; _m21 = 0; _m22 = 1; _m23 = 0; _m24 = 0;
	_m31 = 0; _m32 = 0; _m33 = 1; _m34 = 0; _m41 = 0; _m42 = 0; _m43 = 0; _m44 = 1;
	};
} SVXMatrix;
#pragma warning (pop)
/// unsigned int two-component vector including operator overloads
typedef struct vxuint2 : public SVXUnsignedInteger2D{
	/// constructor
	vxuint2(){}
	/// constructor, initialize with _x, _y
	vxuint2(const uint _x, const uint _y){x = _x; y = _y;}
	
	// assignment operators
	/// vxuint2 += vxuint2
	vxuint2& operator += ( const vxuint2& );
	/// vxuint2 *= float
	vxuint2& operator *= ( float );
	/// vxuint2 /= float
	vxuint2& operator /= ( float );

	// binary operators
	/// vxuint2 = vxuint2 + vxuint2
	vxuint2 operator + ( const vxuint2& ) const;
	/// vxuint2 = vxuint2 * float
	vxuint2 operator * ( float ) const;
	/// vxuint2 = vxuint2 / float
	vxuint2 operator / ( float ) const;

	/// friend vxuint2 = (float * vxuint2) * vxuint2
	friend vxuint2 operator * ( float, const struct vxuint2& );

	/// vxuint2 == vxuint2
	bool operator == ( const vxuint2& ) const;
	/// vxuint2 != vxuint2
	bool operator != ( const vxuint2& ) const;
} vxuint2;
/// unsigned int three-component vector including operator overloads
typedef struct vxuint3 : public SVXUnsignedInteger3D{
	/// constructor
	vxuint3(){}
	/// constructor, initialize with _x, _y, _z
	vxuint3(const uint _x, const uint _y, const uint _z){x = _x; y = _y; z = _z;}

	// assignment operators
	/// vxuint3 += vxuint3
	vxuint3& operator += ( const vxuint3& );
	/// vxuint3 *= vxuint3
	vxuint3& operator *= ( float );
	/// vxuint3 /= vxuint3
	vxuint3& operator /= ( float );

	// binary operators
	/// vxuint3 = vxuint3 + vxuint3
	vxuint3 operator + ( const vxuint3& ) const;
	/// vxuint3 = vxuint3 * float
	vxuint3 operator * ( float ) const;
	/// vxuint3 = vxuint3 / float
	vxuint3 operator / ( float ) const;

	/// friend vxuint3 = (float * vxuint3) * vxuint3
	friend vxuint3 operator * ( float, const struct vxuint3& );

	/// vxuint3 == vxuint3
	bool operator == ( const vxuint3& ) const;
	/// vxuint3 != vxuint3
	bool operator != ( const vxuint3& ) const;
} vxuint3;
/// unsigned int four-component vector including operator overloads
typedef struct vxuint4 : SVXUnsignedInteger4D{
	/// constructor
	vxuint4(){}
	/// constructor, initialize with _x, _y, _z, _w
	vxuint4(const uint _x, const uint _y, const uint _z, const uint _w){x = _x; y = _y; z = _z; w = _w;}

	// assignment operators
	/// vxuint4 += vxuint4
	vxuint4& operator += ( const vxuint4& );
	/// vxuint4 *= float
	vxuint4& operator *= ( float );
	/// vxuint4 /= float
	vxuint4& operator /= ( float );

	// binary operators
	/// vxuint4 = vxuint4 + vxuint4
	vxuint4 operator + ( const vxuint4& ) const;
	/// vxuint4 = vxuint4 * float
	vxuint4 operator * ( float ) const;
	/// vxuint4 = vxuint4 / float
	vxuint4 operator / ( float ) const;

	/// friend vxuint4 = (float * vxuint4) * vxuint4
	friend vxuint4 operator * ( float, const struct vxuint4& );

	/// vxuint4 == vxuint4
	bool operator == ( const vxuint4& ) const;
	/// vxuint4 != vxuint4
	bool operator != ( const vxuint4& ) const;
} vxuint4;
/// byte two-component vector including operator overloads
typedef struct vxbyte2 : SVXByte2D{
	/// constructor
	vxbyte2(){}
	/// constructor, initialize with _x, _y
	vxbyte2(const byte _x, const byte _y){x = _x; y = _y;}

	// assignment operators
	/// vxbyte2 += vxbyte2
	vxbyte2& operator += ( const vxbyte2& );
	/// vxbyte2 *= float
	vxbyte2& operator *= ( float );
	/// vxbyte2 /= float
	vxbyte2& operator /= ( float );

	// binary operators
	/// vxbyte2 = vxbyte2 + vxbyte2
	vxbyte2 operator + ( const vxbyte2& ) const;
	/// vxbyte2 = vxbyte2 * float
	vxbyte2 operator * ( float ) const;
	/// vxbyte2 = vxbyte2 / float
	vxbyte2 operator / ( float ) const;

	/// friend vxbyte2 = (float * vxbyte2) * vxbyte2
	friend vxbyte2 operator * ( float, const struct vxbyte2& );

	/// vxbyte2 == vxbyte2
	bool operator == ( const vxbyte2& ) const;
	/// vxbyte2 != vxbyte2
	bool operator != ( const vxbyte2& ) const;
} vxbyte2;
/// byte four-component vector including operator overloads
typedef struct vxbyte4 : SVXByte4D{
	/// constructor
	vxbyte4(){}
	// BGRA
	/// constructor, initialize with _x, _y, _z, _w
	vxbyte4(const byte _x, const byte _y, const byte _z, const byte _w){x = _x; y = _y; z = _z; w = _w;}

	// assignment operators
	/// vxbyte4 += vxbyte4
	vxbyte4& operator += ( const vxbyte4& );
	/// vxbyte4 *= float
	vxbyte4& operator *= ( float );
	/// vxbyte4 /= float
	vxbyte4& operator /= ( float );

	// binary operators
	/// vxbyte4 = vxbyte4 + vxbyte4
	vxbyte4 operator + ( const vxbyte4& ) const;
	/// vxbyte4 = vxbyte4 * float
	vxbyte4 operator * ( float ) const;
	/// vxbyte4 = vxbyte4 / float
	vxbyte4 operator / ( float ) const;

	/// friend vxbyte4 = (float * vxbyte4) * vxbyte4
	friend vxbyte4 operator * ( float, const struct vxbyte4& );

	/// vxbyte4 == vxbyte4
	bool operator == ( const vxbyte4& ) const;
	/// vxbyte4 != vxbyte4
	bool operator != ( const vxbyte4& ) const;
} vxbyte4;
/// unsigned short two-component vector including operator overloads
typedef struct vxushort2 : SVXUnsignedShort2D{
	/// constructor
	vxushort2(){}
	/// constructor, initialize with _x, _y
	vxushort2(const ushort _x, const ushort _y){x = _x; y = _y;}

	// assignment operators
	/// vxushort2 += vxushort2
	vxushort2& operator += ( const vxushort2& );
	/// vxushort2 *= float
	vxushort2& operator *= ( float );
	/// vxushort2 /= float
	vxushort2& operator /= ( float );

	// binary operators
	/// vxushort2 = vxushort2 + vxushort2
	vxushort2 operator + ( const vxushort2& ) const;
	/// vxushort2 = vxushort2 * float
	vxushort2 operator * ( float ) const;
	/// vxushort2 = vxushort2 / float
	vxushort2 operator / ( float ) const;

	/// friend vxushort2 = (float * vxushort2) * vxushort2
	friend vxushort2 operator * ( float, const struct vxushort2& );

	/// vxushort2 == vxushort2
	bool operator == ( const vxushort2& ) const;
	/// vxushort2 != vxushort2
	bool operator != ( const vxushort2& ) const;
} vxushort2;
/// unsigned short three-component vector including operator overloads
typedef struct vxushort3 : SVXUnsignedShort3D{
	/// constructor
	vxushort3(){}
	/// constructor, initialize with _x, _y, _z
	vxushort3(const int _x, const int _y, const int _z){x = _x; y = _y; z = _z;}

	// assignment operators
	/// vxushort3 += vxushort3
	vxushort3& operator += ( const vxushort3& );
	/// vxushort3 -= vxushort3
	vxushort3& operator -= ( const vxushort3& );
	/// vxushort3 *= float
	vxushort3& operator *= ( float );
	/// vxushort3 /= float
	vxushort3& operator /= ( float );

	// unary operators
	/// vxushort3 = +vxushort3
	vxushort3 operator + () const;
	/// vxushort3 = -vxushort3
	vxushort3 operator - () const;

	// binary operators
	/// vxushort3 = vxushort3 + vxushort3
	vxushort3 operator + ( const vxushort3& ) const;
	/// vxushort3 = vxushort3 - vxushort3
	vxushort3 operator - ( const vxushort3& ) const;
	/// vxushort3 = vxushort3 * float
	vxushort3 operator * ( float ) const;
	/// vxushort3 = vxushort3 / float
	vxushort3 operator / ( float ) const;

	/// friend vxushort3 = (float * vxushort3) * vxushort3
	friend vxushort3 operator * ( float, const struct vxushort3& );

	/// vxushort3 == vxushort3
	bool operator == ( const vxushort3& ) const;
	/// vxushort3 != vxushort3
	bool operator != ( const vxushort3& ) const;
} vxushort3;
/// short two-component vector including operator overloads
typedef struct vxshort2 : SVXShort2D{
	/// constructor
	vxshort2(){}
	/// constructor, initialize with _x, _y
	vxshort2(const short _x, const short _y){x = _x; y = _y;}

	// assignment operators
	/// vxshort2 += vxshort2
	vxshort2& operator += ( const vxshort2& );
	/// vxshort2 -= vxshort2
	vxshort2& operator -= ( const vxshort2& );
	/// vxshort2 *= float
	vxshort2& operator *= ( float );
	/// vxshort2 /= float
	vxshort2& operator /= ( float );

	// unary operators
	/// vxshort2 = +vxshort2
	vxshort2 operator + () const;
	/// vxshort2 = -vxshort2
	vxshort2 operator - () const;

	// binary operators
	/// vxshort2 = vxshort2 + vxshort2
	vxshort2 operator + ( const vxshort2& ) const;
	/// vxshort2 = vxshort2 - vxshort2
	vxshort2 operator - ( const vxshort2& ) const;
	/// vxshort2 = vxshort2 * float
	vxshort2 operator * ( float ) const;
	/// vxshort2 = vxshort2 / float
	vxshort2 operator / ( float ) const;

	/// friend vxshort2 = (float * vxshort2) * vxshort2
	friend vxshort2 operator * ( float, const struct vxshort2& );

	/// vxshort2 == vxshort2
	bool operator == ( const vxshort2& ) const;
	/// vxshort2 != vxshort2
	bool operator != ( const vxshort2& ) const;
} vxshort2;
/// char two-component vector including operator overloads
typedef struct vxchar2 : SVXChar2D{
	/// constructor
	vxchar2(){}
	/// constructor, initialize with _x, _y
	vxchar2(const char _x, const char _y){x = _x; y = _y;}

	// assignment operators
	/// vxchar2 += vxchar2
	vxchar2& operator += ( const vxchar2& );
	/// vxchar2 -= vxchar2
	vxchar2& operator -= ( const vxchar2& );
	/// vxchar2 *= float
	vxchar2& operator *= ( float );
	/// vxchar2 /= float
	vxchar2& operator /= ( float );

	// unary operators
	/// vxchar2 = +vxchar2
	vxchar2 operator + () const;
	/// vxchar2 = -vxchar2
	vxchar2 operator - () const;

	// binary operators
	/// vxchar2 = vxchar2 + vxchar2
	vxchar2 operator + ( const vxchar2& ) const;
	/// vxchar2 = vxchar2 - vxchar2
	vxchar2 operator - ( const vxchar2& ) const;
	/// vxchar2 = vxchar2 * float
	vxchar2 operator * ( float ) const;
	/// vxchar2 = vxchar2 / float
	vxchar2 operator / ( float ) const;

	/// friend vxchar2 = (float * vxchar2) * vxchar2
	friend vxchar2 operator * ( float, const struct vxchar2& );

	/// vxchar2 == vxchar2
	bool operator == ( const vxchar2& ) const;
	/// vxchar2 != vxchar2
	bool operator != ( const vxchar2& ) const;
} vxchar2;
/// int two-component vector including operator overloads
typedef struct vxint2 : SVXInteger2D{
	/// constructor
	vxint2(){}
	/// constructor, initialize with _x, _y
	vxint2(const int _x, const int _y){x = _x; y = _y;}

	// assignment operators
	/// vxint2 += vxint2
	vxint2& operator += ( const vxint2& );
	/// vxint2 -= vxint2
	vxint2& operator -= ( const vxint2& );
	/// vxint2 *= float
	vxint2& operator *= ( float );
	/// vxint2 /= float
	vxint2& operator /= ( float );

	// unary operators
	/// vxint2 = +vxint2
	vxint2 operator + () const;
	/// vxint2 = -vxint2
	vxint2 operator - () const;

	// binary operators
	/// vxint2 = vxint2 + vxint2
	vxint2 operator + ( const vxint2& ) const;
	/// vxint2 = vxint2 - vxint2
	vxint2 operator - ( const vxint2& ) const;
	/// vxint2 = vxint2 * float
	vxint2 operator * ( float ) const;
	/// vxint2 = vxint2 / float
	vxint2 operator / ( float ) const;

	/// friend vxint2 = (float * vxint2) * vxint2
	friend vxint2 operator * ( float, const struct vxint2& );

	/// vxint2 == vxint2
	bool operator == ( const vxint2& ) const;
	/// vxint2 != vxint2
	bool operator != ( const vxint2& ) const;
} vxint2;
/// int three-component vector including operator overloads
typedef struct vxint3 : SVXInteger3D{
	/// constructor
	vxint3(){}
	/// constructor, initialize with _x, _y, _z
	vxint3(const int _x, const int _y, const int _z){x = _x; y = _y; z = _z;}

	// assignment operators
	/// vxint3 += vxint3
	vxint3& operator += ( const vxint3& );
	/// vxint3 -= vxint3
	vxint3& operator -= ( const vxint3& );
	/// vxint3 *= float
	vxint3& operator *= ( float );
	/// vxint3 /= float
	vxint3& operator /= ( float );

	// unary operators
	/// vxint3 = +vxint3
	vxint3 operator + () const;
	/// vxint3 = -vxint3
	vxint3 operator - () const;

	// binary operators
	/// vxint3 = vxint3 + vxint3
	vxint3 operator + ( const vxint3& ) const;
	/// vxint3 = vxint3 - vxint3
	vxint3 operator - ( const vxint3& ) const;
	/// vxint3 = vxint3 * float
	vxint3 operator * ( float ) const;
	/// vxint3 = vxint3 / float
	vxint3 operator / ( float ) const;

	/// friend vxint3 = (float * vxint3) * vxint3
	friend vxint3 operator * ( float, const struct vxint3& );

	/// vxint3 == vxint3
	bool operator == ( const vxint3& ) const;
	/// vxint3 != vxint3
	bool operator != ( const vxint3& ) const;
} vxint3;
/// int four-component vector including operator overloads
typedef struct vxint4 : SVXInteger4D{
	/// constructor
	vxint4(){}
	/// constructor, initialize with _x, _y, _z, _w
	vxint4(const int _x, const int _y, const int _z, const int _w){x = _x; y = _y; z = _z; w = _w;}

	// assignment operators
	/// vxint4 += vxint4
	vxint4& operator += ( const vxint4& );
	/// vxint4 -= vxint4
	vxint4& operator -= ( const vxint4& );
	/// vxint4 *= float
	vxint4& operator *= ( float );
	/// vxint4 /= float
	vxint4& operator /= ( float );

	// unary operators
	/// vxint4 = +vxint4
	vxint4 operator + () const;
	/// vxint4 = -vxint4
	vxint4 operator - () const;

	// binary operators
	/// vxint4 = vxint4 + vxint4
	vxint4 operator + ( const vxint4& ) const;
	/// vxint4 = vxint4 - vxint4
	vxint4 operator - ( const vxint4& ) const;
	/// vxint4 = vxint4 * float
	vxint4 operator * ( float ) const;
	/// vxint4 = vxint4 / float
	vxint4 operator / ( float ) const;

	/// friend vxint4 = (float * vxint4) * vxint4
	friend vxint4 operator * ( float, const struct vxint4& );

	/// vxint4 == vxint4
	bool operator == ( const vxint4& ) const;
	/// vxint4 != vxint4
	bool operator != ( const vxint4& ) const;
} vxint4;
/// double two-component vector including operator overloads
typedef struct vxdouble2 : SVXDouble2D{
	/// constructor
	vxdouble2(){}
	/// constructor, initialize with _x, _y
	vxdouble2(const double _x, const double _y){x = _x; y = _y;}

	// assignment operators
	/// vxdouble2 += vxdouble2
	vxdouble2& operator += ( const vxdouble2& );
	/// vxdouble2 -= vxdouble2
	vxdouble2& operator -= ( const vxdouble2& );
	/// vxdouble2 *= double
	vxdouble2& operator *= ( double );
	/// vxdouble2 /= double
	vxdouble2& operator /= ( double );

	// unary operators
	/// vxdouble2 = +vxdouble2
	vxdouble2 operator + () const;
	/// vxdouble2 = -vxdouble2
	vxdouble2 operator - () const;

	// binary operators
	/// vxdouble2 = vxdouble2 + vxdouble2
	vxdouble2 operator + ( const vxdouble2& ) const;
	/// vxdouble2 = vxdouble2 - vxdouble2
	vxdouble2 operator - ( const vxdouble2& ) const;
	/// vxdouble2 = vxdouble2 * double
	vxdouble2 operator * ( double ) const;
	/// vxdouble2 = vxdouble2 / double
	vxdouble2 operator / ( double ) const;

	/// friend vxdouble2 = (float * vxdouble2) * vxdouble2
	friend vxdouble2 operator * ( double, const struct vxdouble2& );

	/// vxdouble2 == vxdouble2
	bool operator == ( const vxdouble2& ) const;
	/// vxdouble2 != vxdouble2
	bool operator != ( const vxdouble2& ) const;
} vxdouble2;
/// double three-component vector including operator overloads
typedef struct vxdouble3 : SVXDouble3D{
	/// constructor
	vxdouble3(){}
	/// constructor, initialize with _x, _y, _z
	vxdouble3(const double _x, const double _y, const double _z){x = _x; y = _y; z = _z;}
	/// constructor, initialize with SVXFloat3D 
	vxdouble3(const SVXFloat3D& f3){ x = (double)f3.x; y = (double)f3.y; z = (double)f3.z;}

	// assignment operators
	/// vxdouble3 += vxdouble3
	vxdouble3& operator += ( const vxdouble3& );
	/// vxdouble3 -= vxdouble3
	vxdouble3& operator -= ( const vxdouble3& );
	/// vxdouble3 *= double
	vxdouble3& operator *= ( double );
	/// vxdouble3 /= double
	vxdouble3& operator /= ( double );

	// unary operators
	/// vxdouble3 = +vxdouble3
	vxdouble3 operator + () const;
	/// vxdouble3 = -vxdouble3
	vxdouble3 operator - () const;

	// binary operators
	/// vxdouble3 = vxdouble3 + vxdouble3
	vxdouble3 operator + ( const vxdouble3& ) const;
	/// vxdouble3 = vxdouble3 - vxdouble3
	vxdouble3 operator - ( const vxdouble3& ) const;
	/// vxdouble3 = vxdouble3 * double
	vxdouble3 operator * ( double ) const;
	/// vxdouble3 = vxdouble3 / double
	vxdouble3 operator / ( double ) const;

	/// friend vxdouble3 = (float * vxdouble3) * vxdouble3
	friend vxdouble3 operator * ( double, const struct vxdouble3& );

	/// vxdouble3 == vxdouble3
	bool operator == ( const vxdouble3& ) const;
	/// vxdouble3 != vxdouble3
	bool operator != ( const vxdouble3& ) const;
} vxdouble3;
/// double four-component vector including operator overloads
typedef struct vxdouble4 : SVXDouble4D{
	/// constructor
	vxdouble4(){}
	/// constructor, initialize with _x, _y, _z, _w
	vxdouble4(const double _x, const double _y, const double _z, const double _w){x = _x; y = _y; z = _z; w = _w;}

	// assignment operators
	/// vxdouble4 += vxdouble4
	vxdouble4& operator += ( const vxdouble4& );
	/// vxdouble4 -= vxdouble4
	vxdouble4& operator -= ( const vxdouble4& );
	/// vxdouble4 *= double
	vxdouble4& operator *= ( double );
	/// vxdouble4 /= double
	vxdouble4& operator /= ( double );

	// unary operators
	/// vxdouble4 = +vxdouble4
	vxdouble4 operator + () const;
	/// vxdouble4 = -vxdouble4
	vxdouble4 operator - () const;

	// binary operators
	/// vxdouble4 = vxdouble4 + vxdouble4
	vxdouble4 operator + ( const vxdouble4& ) const;
	/// vxdouble4 = vxdouble4 - vxdouble4
	vxdouble4 operator - ( const vxdouble4& ) const;
	/// vxdouble4 = vxdouble4 * double
	vxdouble4 operator * ( double ) const;
	/// vxdouble4 = vxdouble4 / double
	vxdouble4 operator / ( double ) const;

	/// friend vxdouble4 = (float * vxdouble4) * vxdouble4
	friend vxdouble4 operator * ( double, const struct vxdouble4& );

	/// vxdouble4 == vxdouble4
	bool operator == ( const vxdouble4& ) const;
	/// vxdouble4 != vxdouble4
	bool operator != ( const vxdouble4& ) const;
} vxdouble4;
/// float two-component vector including operator overloads
typedef struct vxfloat2 : SVXFloat2D{
	/// constructor
	vxfloat2(){}
	/// constructor, initialize with _x, _y
	vxfloat2(const float _x, const float _y){x = _x; y = _y;}

	// assignment operators
	/// vxfloat2 += vxfloat2
	vxfloat2& operator += ( const vxfloat2& );
	/// vxfloat2 -= vxfloat2
	vxfloat2& operator -= ( const vxfloat2& );
	/// vxfloat2 *= double
	vxfloat2& operator *= ( float );
	/// vxfloat2 /= double
	vxfloat2& operator /= ( float );

	// unary operators
	/// vxfloat2 = +vxfloat2
	vxfloat2 operator + () const;
	/// vxfloat2 = -vxfloat2
	vxfloat2 operator - () const;

	// binary operators
	/// vxfloat2 = vxfloat2 + vxfloat2
	vxfloat2 operator + ( const vxfloat2& ) const;
	/// vxfloat2 = vxfloat2 - vxfloat2
	vxfloat2 operator - ( const vxfloat2& ) const;
	/// vxfloat2 = vxfloat2 * float
	vxfloat2 operator * ( float ) const;
	/// vxfloat2 = vxfloat2 / float
	vxfloat2 operator / ( float ) const;

	/// friend vxfloat2 = (float * vxfloat2) * vxfloat2
	friend vxfloat2 operator * ( float, const struct vxfloat2& );

	/// vxfloat2 == vxfloat2
	bool operator == ( const vxfloat2& ) const;
	/// vxfloat2 != vxfloat2
	bool operator != ( const vxfloat2& ) const;
} vxfloat2;
/// float three-component vector including operator overloads
typedef struct vxfloat3 : SVXFloat3D{
	/// constructor
	vxfloat3(){}
	/// constructor, initialize with _x, _y, _z
	vxfloat3(const float _x, const float _y, const float _z){x = _x; y = _y; z = _z;}
	/// constructor, initialize with vxdouble3
	vxfloat3(const vxdouble3& d3){ x = (float)d3.x; y = (float)d3.y; z = (float)d3.z;}

	// assignment operators
	/// vxfloat3 += vxfloat3
	vxfloat3& operator += ( const vxfloat3& );
	/// vxfloat3 -= vxfloat3
	vxfloat3& operator -= ( const vxfloat3& );
	/// vxfloat3 *= float
	vxfloat3& operator *= ( float );
	/// vxfloat3 /= float
	vxfloat3& operator /= ( float );

	// unary operators
	/// vxfloat3 = +vxfloat3
	vxfloat3 operator + () const;
	/// vxfloat3 = -vxfloat3
	vxfloat3 operator - () const;

	// binary operators
	/// vxfloat3 = vxfloat3 + vxfloat3
	vxfloat3 operator + ( const vxfloat3& ) const;
	/// vxfloat3 = vxfloat3 - vxfloat3
	vxfloat3 operator - ( const vxfloat3& ) const;
	/// vxfloat3 = vxfloat3 * float
	vxfloat3 operator * ( float ) const;
	/// vxfloat3 = vxfloat3 / float
	vxfloat3 operator / ( float ) const;

	/// friend vxfloat3 = (float * vxfloat3) * vxfloat3
	friend vxfloat3 operator * ( float, const struct vxfloat3& );

	/// vxfloat3 == vxfloat3
	bool operator == ( const vxfloat3& ) const;
	/// vxfloat3 != vxfloat3
	bool operator != ( const vxfloat3& ) const;
} vxfloat3;
/// float four-component vector including operator overloads
typedef struct vxfloat4 : SVXFloat4D{
	/// constructor
	vxfloat4(){}
	/// constructor, initialize with _x, _y, _z, _w
	vxfloat4(const float _x, const float _y, const float _z, const float _w){x = _x; y = _y; z = _z; w = _w;}
	/// constructor, initialize with vxdouble4
	vxfloat4(const vxdouble4& d4){ x = (float)d4.x; y = (float)d4.y; z = (float)d4.z; w = (float)d4.w;}

	// assignment operators
	/// vxfloat4 += vxfloat4
	vxfloat4& operator += ( const vxfloat4& );
	/// vxfloat4 -= vxfloat4
	vxfloat4& operator -= ( const vxfloat4& );
	/// vxfloat4 *= float
	vxfloat4& operator *= ( float );
	/// vxfloat4 /= float
	vxfloat4& operator /= ( float );

	// unary operators
	/// vxfloat4 = +vxfloat4
	vxfloat4 operator + () const;
	/// vxfloat4 = -vxfloat4
	vxfloat4 operator - () const;

	// binary operators
	/// vxfloat4 = vxfloat4 + vxfloat4
	vxfloat4 operator + ( const vxfloat4& ) const;
	/// vxfloat4 = vxfloat4 - vxfloat4
	vxfloat4 operator - ( const vxfloat4& ) const;
	/// vxfloat4 = vxfloat4 * float
	vxfloat4 operator * ( float ) const;
	/// vxfloat4 = vxfloat4 / float
	vxfloat4 operator / ( float ) const;

	/// friend vxfloat4 = (float * vxfloat4) * vxfloat4
	friend vxfloat4 operator * ( float, const struct vxfloat4& );

	/// vxfloat4 == vxfloat4
	bool operator == ( const vxfloat4& ) const;
	/// vxfloat4 != vxfloat4
	bool operator != ( const vxfloat4& ) const;
} vxfloat4;
/// double 4x4-matrix including operator overloads
typedef struct vxmatrixdouble : SVXMatrixDouble{
	/// constructor
	vxmatrixdouble(){}
	/// constructor, initialize with SVXMatrix
	vxmatrixdouble(SVXMatrix& mat){_m11 = (double)mat._m11; _m12 = (double)mat._m12; _m13 = (double)mat._m13; _m14 = (double)mat._m14;
	_m21 = (double)mat._m21; _m22 = (double)mat._m22; _m23 = (double)mat._m23; _m24 = (double)mat._m24;
	_m31 = (double)mat._m31; _m32 = (double)mat._m32; _m33 = (double)mat._m33; _m34 = (double)mat._m34; 
	_m41 = (double)mat._m41; _m42 = (double)mat._m42; _m43 = (double)mat._m43; _m44 = (double)mat._m44;
	};
	/// constructor, initialize with doubles, _m[ij]
	vxmatrixdouble( double _m11, double _m12, double _m13, double _m14,
		double _m21, double _m22, double _m23, double _m24,
		double _m31, double _m32, double _m33, double _m34,
		double _m41, double _m42, double _m43, double _m44 );

	// access grants
	/// double = vxmatrixdouble(i, j)
	double& operator () ( uint Row, uint Col );
	/// double = vxmatrixdouble(i, j)
	double  operator () ( uint Row, uint Col ) const;

	// casting operators
	/// casting operator
	operator double* ();
	/// casting operator
	operator const double* () const;

	/// assignment operators * vxmatrixdouble
	vxmatrixdouble& operator *= ( const vxmatrixdouble& );
	/// assignment operators + vxmatrixdouble
	vxmatrixdouble& operator += ( const vxmatrixdouble& );
	/// assignment operators - vxmatrixdouble
	vxmatrixdouble& operator -= ( const vxmatrixdouble& );
	/// assignment operators * double
	vxmatrixdouble& operator *= ( double );
	/// assignment operators / double
	vxmatrixdouble& operator /= ( double );

	/// unary operators +
	vxmatrixdouble operator + () const;
	/// unary operators *
	vxmatrixdouble operator - () const;

	/// binary operators * vxmatrixdouble
	vxmatrixdouble operator * ( const vxmatrixdouble& ) const;
	/// binary operators + vxmatrixdouble
	vxmatrixdouble operator + ( const vxmatrixdouble& ) const;
	/// binary operators - vxmatrixdouble
	vxmatrixdouble operator - ( const vxmatrixdouble& ) const;
	/// binary operators * double
	vxmatrixdouble operator * ( double ) const;
	/// binary operators / double
	vxmatrixdouble operator / ( double ) const;

	/// friend operators * (double, vxmatrixdouble)
	friend vxmatrixdouble operator * ( double, const vxmatrixdouble& );

	/// condition operators ==
	bool operator == ( const vxmatrixdouble& ) const;
	/// condition operators !=
	bool operator != ( const vxmatrixdouble& ) const;
} vxmatrixdouble;
/// float 4x4-matrix including operator overloads
typedef struct vxmatrix : SVXMatrix{
	/// constructor
	vxmatrix(){}
	/// constructor, initialize with vxmatrixdouble
	vxmatrix(vxmatrixdouble mat){_m11 = (float)mat._m11; _m12 = (float)mat._m12; _m13 = (float)mat._m13; _m14 = (float)mat._m14;
	_m21 = (float)mat._m21; _m22 = (float)mat._m22; _m23 = (float)mat._m23; _m24 = (float)mat._m24;
	_m31 = (float)mat._m31; _m32 = (float)mat._m32; _m33 = (float)mat._m33; _m34 = (float)mat._m34; 
	_m41 = (float)mat._m41; _m42 = (float)mat._m42; _m43 = (float)mat._m43; _m44 = (float)mat._m44;
	};
	/// constructor, initialize with doubles, _m[ij]
	vxmatrix( float _m11, float _m12, float _m13, float _m14,
		float _m21, float _m22, float _m23, float _m24,
		float _m31, float _m32, float _m33, float _m34,
		float _m41, float _m42, float _m43, float _m44 );
	
	// access grants
	/// float = vxmatrix(i, j)
	float& operator () ( uint Row, uint Col );
	/// float = vxmatrix(i, j)
	float  operator () ( uint Row, uint Col ) const;

	// casting operators
	/// casting operator
	operator float* ();
	/// casting operator
	operator const float* () const;

	// assignment operators
	/// assignment operators * vxmatrix
	vxmatrix& operator *= ( const vxmatrix& );
	/// assignment operators + vxmatrix
	vxmatrix& operator += ( const vxmatrix& );
	/// assignment operators - vxmatrix
	vxmatrix& operator -= ( const vxmatrix& );
	/// assignment operators * float
	vxmatrix& operator *= ( float );
	/// assignment operators / float 
	vxmatrix& operator /= ( float );

	/// unary operators +
	vxmatrix operator + () const;
	/// unary operators -
	vxmatrix operator - () const;

	// binary operators
	/// binary operators * vxmatrix
	vxmatrix operator * ( const vxmatrix& ) const;
	/// binary operators + vxmatrix
	vxmatrix operator + ( const vxmatrix& ) const;
	/// binary operators - vxmatrix
	vxmatrix operator - ( const vxmatrix& ) const;
	/// binary operators * double
	vxmatrix operator * ( float ) const;
	/// binary operators / float
	vxmatrix operator / ( float ) const;

	/// friend operators * (float, vxmatrix)
	friend vxmatrix operator * ( float, const vxmatrix& );

	/// condition operators ==
	bool operator == ( const vxmatrix& ) const;
	/// condition operators !=
	bool operator != ( const vxmatrix& ) const;
} vxmatrix;

//==========================================
// Simple VXMath Defined in Projection Space
//==========================================
/*! \fn __vxstatic void vxmathproj::VXHMTransformPoint(vxfloat3* pf3PosOut, const vxfloat3* pf3PosIn, const vxmatrix* psvxMatrix)
 *  \brief 3D position을 4x4 matrix로 변환하는 function. Homogeneous factor w = 1 로 두고 projecting 처리.
 *  \param pf3PosOut [out] \n vxfloat3 \n operation의 결과인 vxfloat3의 포인터.
 *  \param pf3PosIn [in] \n vxfloat3 \n operation의 source 로 사용될 3D position에 대한 포인터.
 *  \param psvxMatrix [in] \n vxmatrix \n operation의 변환을 정의하는 4x4 matrix에 대한 포인터
 *  \remarks row major operation
 */
__vxstatic void VXHMTransformPoint(vxfloat3* pf3PosOut, const vxfloat3* pf3PosIn, const vxmatrix* psvxMatrix);

/*! \fn __vxstatic void vxmathproj::VXHMTransformVector(vxfloat3* pf3VecOut, const vxfloat3* pf3VecIn, const vxmatrix* psvxMatrix)
 *  \brief 3D vector를 4x4 matrix로 변환하는 function. Homogeneous factor w = 1 로 두고 projecting 처리.
 *  \param pf3VecOut [out] \n vxfloat3 \n operation의 결과인 vxfloat3의 포인터.
 *  \param pf3VecIn [in] \n vxfloat3 \n operation의 source 로 사용될 3D vector에 대한 포인터.
 *  \param psvxMatrix [in] \n vxmatrix \n operation의 변환을 정의하는 4x4 matrix에 대한 포인터
 *  \remarks row major operation
 */
__vxstatic void VXHMTransformVector(vxfloat3* pf3VecOut, const vxfloat3* pf3VecIn, const vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixMultiply(vxmatrix* psvxMatrix , const vxmatrix* psvxMatrix1st, const vxmatrix* psvxMatrix2nd)
 * @brief 두 개의 4x4 matrix를 곱함.
 * @param psvxMatrix [out] \n vxmatrix \n 4x4 matrix가 곱해진 결과인 vxmatrix의 포인터
 * @param psvxMatrix1st [in] \n vxmatrix \n operation의 source인 4x4 matrix에 대한 포인터 (왼쪽)
 * @param psvxMatrix2nd [in] \n vxmatrix \n operation의 source인 4x4 matrix에 대한 포인터 (오른쪽)
 * @remarks n row major operation이며 곱의 순서가 왼쪽에서 오른쪽으로 진행됨.
 */
__vxstatic void VXHMMatrixMultiply(vxmatrix* psvxMatrix , const vxmatrix* psvxMatrix1st, const vxmatrix* psvxMatrix2nd);

/*!
 * @fn __vxstatic void vxmathproj::VXHMAddVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief 두 개의 3D vector를 더함.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector가 더해진 결과인 vxfloat3의 포인터
 * @param pf3Vec1 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @param pf3Vec2 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @remarks 
*/
__vxstatic void VXHMAddVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMSubstractVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief 두 개의 3D vector를 뺌.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector가 빼진 결과인 vxfloat3의 포인터
 * @param pf3Vec1 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @param pf3Vec2 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @remarks 
*/
__vxstatic void VXHMSubstractVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic float vxmathproj::VXHMDotVector(const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief 두 개의 3D vector에 대한 dot product를 수행.
 * @param pf3Vec1 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @param pf3Vec2 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터
 * @return float \n dot product의 scalar 결과
 * @remarks 
*/
__vxstatic float VXHMDotVector(const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMCrossDotVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief 두 개의 3D vector에 대한 cross dot product를 수행.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector가 cross dot product된 결과인 vxfloat3의 포인터
 * @param pf3Vec1 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터 (왼쪽)
 * @param pf3Vec2 [in] \n vxfloat3 \n operation의 source인 3D vector에 대한 포인터 (오른쪽)
 * @remarks pf3Vec1에서 pf3Vec2방향으로 cross dot을 수행, (왼쪽에서 오른쪽)
*/
__vxstatic void VXHMCrossDotVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixIdentity(vxmatrix* psvxMatrix)
 * @brief identity matrix를 생성하는 함수
 * @param psvxMatrix [out] \n vxmatrix \n identity matrix가 저장될 vxmatrix의 포인터
 * @remarks 
*/
__vxstatic void VXHMMatrixIdentity(vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixWS2CS(vxmatrix* psvxMatrix, const vxfloat3* pf3PosEye, const vxfloat3* pf3VecUp, const vxfloat3* pf3VecView)
 * @brief 오른손 방향으로 정의된 Space를 기준으로 look-at Space로 변환하는 matrix를 생성.
 * @param psvxMatrix [out] \n vxmatrix \n look-at Space(또는 Camera Space or Viewing Space)로 변환하는 vxmatrix에 대한 포인터
 * @param pf3PosEye [in] \n vxfloat3 \n Camera(또는 Eye)의 position을 정의하는 vxfloat3의 포인터
 * @param pf3VecUp [in] \n vxfloat3 \n 현재 Camera(또는 Eye)가 정의된 Space에서의 위쪽 방향을 정의하는 vector에 대한 vxfloat3의 포인터
 * @param pf3VecView [in] \n vxfloat3 \n 현재 Camera(또는 Eye)가 정의된 Space에서의 시선의 방향을  정의하는 vector에 대한 vxfloat3의 포인터
 * @remarks 
 * RHS 기준이며 row major 기준으로 matrix가 생성 \n
 * f3PosEye, f3VecUp, f3VecView의 Space는 모두 동일해야 하며 하나의 World Space로 통일 \n
 * Camera Space에서의 View 방향은 -z축 방향, Up 방향은 y축 방향 \n
*/
__vxstatic void VXHMMatrixWS2CS(vxmatrix* psvxMatrix, const vxfloat3* pf3PosEye, const vxfloat3* pf3VecUp, const vxfloat3* pf3VecView);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixOrthogonalCS2PS(vxmatrix* psvxMatrix, float fWidthFrustum, float fHeightFrustum, float fNearPlane, float fFarPlane)
 * @brief 오른손 방향으로 정의된 Space를 기준으로 Orhogonal Projecting을 수행하는 matrix를 생성.
 * @param psvxMatrix [out] \n vxmatrix \n Projection Space(또는 Camera Space or Viewing Space)로 변환하는 vxmatrix에 대한 포인터
 * @param fWidthFrustum [in] \n float \n View Frustum 의 width로 Camera Space에서 정의된 단위 사용
 * @param fHeightFrustum [in] \n float \n View Frustum 의 height로 Camera Space에서 정의된 단위 사용
 * @param fNearPlane [in] \n float \n View Frustum 의 minimum z 값으로 Camera Space에서 정의된 단위 사용
 * @param fFarPlane [in] \n float \n View Frustum 의 maximum z 값으로 Camera Space에서 정의된 단위 사용
 * @remarks 
 * RHS 기준이며 row major 기준으로 matrix가 생성 \n
 * look-at Space(또는 Camera Space or Viewing Space)에서 Projection Space로 변환 \n
 * View Frustum 외의 영역은 모두 Cripping out 되어 Projection 되지 않음 \n
 * 좌표계의 방향은 Camera Space와 동일
*/
__vxstatic void VXHMMatrixOrthogonalCS2PS(vxmatrix* psvxMatrix, float fWidthFrustum, float fHeightFrustum, float fNearPlane, float fFarPlane);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixPerspectiveCS2PS(vxmatrix* psvxMatrix, float fFovY, float fAspectRatio, float fNearPlane, float fFarPlane)
 * @brief 오른손 방향으로 정의된 Space를 기준으로 Perspective Projecting을 수행하는 matrix를 생성.
 * @param psvxMatrix [out] \n vxmatrix \n Projection Space(또는 Camera Space or Viewing Space)로 변환하는 vxmatrix에 대한 포인터
 * @param fFovY [in] \n float \n Perspective View Frustum 의 y 방향에 대한 field of view를 radian으로 정의한 값
 * @param fAspectRatio [in] \n float \n Perspective View Frustum 의 Aspect Ratio로 Viewing Space에거 정의되는 View Plane의 (width / height)
 * @param fNearPlane [in] \n float \n View Frustum 의 minimum z 값으로 Camera Space에서 정의된 단위 사용
 * @param fFarPlane [in] \n float \n View Frustum 의 maximum z 값으로 Camera Space에서 정의된 단위 사용
 * @remarks 
 * RHS 기준이며 row major 기준으로 matrix가 생성 \n
 * look-at Space(또는 Camera Space or Viewing Space)에서 Projection Space로 변환 \n
 * View Frustum 외의 영역은 모두 Cripping out 되어 Projection 되지 않음 \n
 * 좌표계의 방향은 Camera Space와 동일
*/
__vxstatic void VXHMMatrixPerspectiveCS2PS(vxmatrix* psvxMatrix, float fFovY, float fAspectRatio, float fNearPlane, float fFarPlane);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixPS2SS(vxmatrix* psvxMatrix, float fWidthScreen, float fHeightScreen)
 * @brief 오른손 방향으로 정의된 Space를 기준으로 Screen의 Pixel Plane으로 정의되는 Screen Space로 변환하는 matrix 생성.
 * @param psvxMatrix [out] \n vxmatrix \n Screen Space로 변환하는 vxmatrix에 대한 포인터
 * @param fWidthScreen [in] \n float \n Screen의 width로 pixel 단위로 정의
 * @param fHeightScreen [in] \n float \n Screen의 height로 pixel 단위로 정의
 * @remarks 
 * RHS 기준이며 row major 기준으로 matrix가 생성 
 * Projection Space에서 Screen Space로 변환 \n
 * Projection Space에서 정의되는 View Frustum의 Near Plane에 Screen Plane이 Mapping됨. \n
 * Screen Space에서 Screen의 오른쪽이 x축, 아래쪽이 y축, Viewing Depth 방향이 z축으로 정의
*/
__vxstatic void VXHMMatrixPS2SS(vxmatrix* psvxMatrix, float fWidthScreen, float fHeightScreen);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixInverse(vxmatrix* psvxMatrixInv, const vxmatrix* psvxMatrix)
 * @brief Inverse matrix를 생성
 * @param psvxMatrixInv [out] \n vxmatrix \n inverse matrix의 결과인 vxmatrix의 포인터
 * @param psvxMatrix [in] \n vxmatrix \n operation의 source인 matrix에 대한 포인터
 * @remarks Determinant가 0인 경우 psvxMatrixInv의 값을 변환하지 않고 반환
*/
__vxstatic void VXHMMatrixInverse(vxmatrix* psvxMatrixInv, const vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixRotationAxis(vxmatrix* psvxMatrix, const vxfloat3* pf3VecRotationAxis, float fRotAngleRad)
 * @brief 원점을 기준으로 주어진 축을 중심으로 회전하는 matrix 생성
 * @param psvxMatrix [out] \n vxmatrix \n 회전 matrix의 결과인 vxmatrix의 포인터
 * @param pf3VecRotationAxis [in] \n vxfloat3 \n 회전축을 정의하는 3D vector를 정의하는 포인터
 * @param fRotAngleRad [in] \n float \n 회전각을 정의. Radian 단위.
 * @remarks 좌표계의 방향에 따라 회전 방향이 정해짐 (ex. RHS의 경우 오른나사 방향, LHS의 경우 왼나사 방향)
*/
__vxstatic void VXHMMatrixRotationAxis(vxmatrix* psvxMatrix, const vxfloat3* pf3VecRotationAxis, float fRotAngleRad);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixScaling(vxmatrix* psvxMatrix, vxfloat3 f3ScaleXYZ)
 * @brief 현재 좌표계가 정의하는 축의 방향을 따라 scale 하는 matrix 생성
 * @param psvxMatrix [out] \n vxmatrix \n scaling matrix의 결과인 vxmatrix의 포인터
 * @param f3ScaleXYZ [in] \n vxfloat3 \n (x,y,z) 각 축 방향에 따른 scaling factor를 정의하는 vxfloat3의 포인터
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixScaling(vxmatrix* psvxMatrix, vxfloat3 f3ScaleXYZ);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixTranslation(vxmatrix* psvxMatrix, vxfloat3 f3TranslateXYZ)
 * @brief 현재 좌표계가 정의하는 축의 방향을 따라 평행이동하는 matrix 생성
 * @param psvxMatrix [out] \n vxmatrix \n 평행이동 matrix의 결과인 vxmatrix의 포인터
 * @param f3TranslateXYZ [in] \n vxfloat3 \n (x,y,z) 각 축 방향에 따른 translation factor를 정의하는 vxfloat3의 포인터
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixTranslation(vxmatrix* psvxMatrix, vxfloat3 f3TranslateXYZ);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixTranspose(vxmatrix* psvxMatrixDst, const vxmatrix* psvxMatrixSrc)
 * @brief transpose 연산을 수행
 * @param psvxMatrixDst [out] \n vxmatrix \n transpose 될 matrix가 저장될 vxmatrix의 포인터
 * @param psvxMatrixSrc [in] \n vxmatrix \n transpose 할 matrix가 저장된 vxmatrix의 포인터
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixTranspose(vxmatrix* psvxMatrixDst, const vxmatrix* psvxMatrixSrc);

/*!
 * @fn __vxstatic void vxmathproj::VXHMNormalizeVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec)
 * @brief 3D vector에 대해 길이가 1인 단위 vector를 생성
 * @param pf3VecOut [out] \n vxfloat3 \n 평행이동 matrix의 결과인 vxmatrix의 포인터
 * @param pf3Vec [in] \n vxfloat3 \n 평행이동 matrix의 결과인 vxmatrix의 포인터
 * @remarks 길이가 0인 3D vector가 주어지면 무의미한 3D vector 생성.
*/
__vxstatic void VXHMNormalizeVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec);

/*!
 * @fn __vxstatic float vxmathproj::VXHMLengthVector(const vxfloat3* pf3Vec)
 * @brief 3D vector의 길이를 계산
 * @param pf3Vec [in] \n vxfloat3 \n operation의 source 3D vector의 포인터
 * @return \n 3D vector의 길이
*/
__vxstatic float VXHMLengthVector(const vxfloat3* pf3Vec);

/*!
 * @fn __vxstatic float vxmathproj::VXHMLengthVectorSq(const vxfloat3* pf3Vec)
 * @brief 3D vector의 길이의 제곱값 계산
 * @param pf3Vec [in] \n vxfloat3 \n operation의 source 3D vector의 포인터
 * @return \n 3D vector의 길이의 제곱
 * @remarks 3D vector의 길이 계산을 위한 식에서 square root를 취하지 않은 값을 반환
*/
__vxstatic float VXHMLengthVectorSq(const vxfloat3* pf3Vec);

// double precision
/*!
 * @fn __vxstatic void vxmathproj::VXHMDoubleMatrixMultiply(vxmatrixdouble* psvxMatrix , const vxmatrixdouble* psvxMatrix1st, const vxmatrixdouble* psvxMatrix2nd)
 * @brief 두 개의 4x4 matrix를 곱함.
 * @param psvxMatrix [out] \n vxmatrix \n 4x4 matrix가 곱해진 결과인 vxmatrix의 포인터
 * @param psvxMatrix1st [in] \n vxmatrix \n operation의 source인 4x4 matrix에 대한 포인터 (왼쪽)
 * @param psvxMatrix2nd [in] \n vxmatrix \n operation의 source인 4x4 matrix에 대한 포인터 (오른쪽)
 * @remarks 
 * n row major operation이며 곱의 순서가 왼쪽에서 오른쪽으로 진행됨.
 * VXHMMatrixMultiply와 동일한 연산이며 float precision이 대신 double precision으로 처리 \n
*/
__vxstatic void VXHMDoubleMatrixMultiply(vxmatrixdouble* psvxMatrix , const vxmatrixdouble* psvxMatrix1st, const vxmatrixdouble* psvxMatrix2nd);

//==============================================
// Projection Space (4x4 Matrix and 4 Vector)
// Inline Math Units
// 2012-05-04
// 2012-06-01 : Add Multiple Types' Math
//==============================================
#pragma region // vxfloat3 Operators
inline vxfloat3& vxfloat3::operator += ( const vxfloat3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}
inline vxfloat3& vxfloat3::operator -= ( const vxfloat3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}
inline vxfloat3& vxfloat3::operator *= ( float a )
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}
inline vxfloat3& vxfloat3::operator /= ( float a )
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}
inline vxfloat3 vxfloat3::operator + () const
{
	return (*this);
}
inline vxfloat3 vxfloat3::operator - () const
{
	return vxfloat3(-x, -y, -z);
}
inline vxfloat3 vxfloat3::operator + ( const vxfloat3& a ) const
{
	return vxfloat3(x + a.x, y + a.y, z + a.z);
}
inline vxfloat3 vxfloat3::operator - ( const vxfloat3& a ) const
{
	return vxfloat3(x - a.x, y - a.y, z - a.z);
}
inline vxfloat3 vxfloat3::operator * ( float a ) const
{
	return vxfloat3(x * a, y * a, z * a);
}
inline vxfloat3 vxfloat3::operator / ( float a ) const
{
	return vxfloat3(x / a, y / a, z / a);
}
inline vxfloat3 operator * ( float f, const struct vxfloat3& a)
{
	return vxfloat3(f * a.x, f * a.y, f * a.z);
}
inline bool	vxfloat3::operator == ( const vxfloat3& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}
inline bool vxfloat3::operator != ( const vxfloat3& a ) const
{
	return x != a.x && y != a.y && z != a.z;
}
#pragma endregion

#pragma region // vxuint2 Operators
inline vxuint2& vxuint2::operator += ( const vxuint2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxuint2& vxuint2::operator *= ( float a )
{
	x = (uint)((float)x*a + 0.5f);
	y = (uint)((float)y*a + 0.5f);
	return *this;
}
inline vxuint2& vxuint2::operator /= ( float a )
{
	x = (uint)((float)x/a + 0.5f);
	y = (uint)((float)y/a + 0.5f);
	return *this;
}
inline vxuint2 vxuint2::operator + ( const vxuint2& a ) const
{
	return vxuint2(x + a.x, y + a.y);
}
inline vxuint2 vxuint2::operator * ( float a ) const
{
	return vxuint2((uint)((float)x * a + 0.5f), (uint)((float)y * a + 0.5f));
}
inline vxuint2 vxuint2::operator / ( float a ) const
{
	return vxuint2((uint)((float)x / a + 0.5f), (uint)((float)y / a + 0.5f));
}
inline vxuint2 operator * ( float f, const struct vxuint2& a)
{
	return vxuint2((uint)(f * (float)a.x + 0.5f), (uint)(f * (float)a.y + 0.5f));
}
inline bool	vxuint2::operator == ( const vxuint2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxuint2::operator != ( const vxuint2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxuint3 Operators
inline vxuint3& vxuint3::operator += ( const vxuint3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}
inline vxuint3& vxuint3::operator *= ( float a )
{
	x = (uint)((float)x*a + 0.5f);
	y = (uint)((float)y*a + 0.5f);
	z = (uint)((float)z*a + 0.5f);
	return *this;
}
inline vxuint3& vxuint3::operator /= ( float a )
{
	x = (uint)((float)x/a + 0.5f);
	y = (uint)((float)y/a + 0.5f);
	z = (uint)((float)z/a + 0.5f);
	return *this;
}
inline vxuint3 vxuint3::operator + ( const vxuint3& a ) const
{
	return vxuint3(x + a.x, y + a.y, z + a.z);
}
inline vxuint3 vxuint3::operator * ( float a ) const
{
	return vxuint3((uint)((float)x * a + 0.5f), (uint)((float)y * a + 0.5f), (uint)((float)z * a + 0.5f));
}
inline vxuint3 vxuint3::operator / ( float a ) const
{
	return vxuint3((uint)((float)x / a + 0.5f), (uint)((float)y / a + 0.5f), (uint)((float)z / a + 0.5f));
}
inline vxuint3 operator * ( float f, const struct vxuint3& a)
{
	return vxuint3((uint)(f * (float)a.x + 0.5f), (uint)(f * (float)a.y + 0.5f), (uint)(f * (float)a.z + 0.5f));
}
inline bool	vxuint3::operator == ( const vxuint3& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}
inline bool vxuint3::operator != ( const vxuint3& a ) const
{
	return x != a.x && y != a.y && z != a.z;
}
#pragma endregion

#pragma region // vxuint4 Operators
inline vxuint4& vxuint4::operator += ( const vxuint4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}
inline vxuint4& vxuint4::operator *= ( float a )
{
	x = (uint)((float)x*a + 0.5f);
	y = (uint)((float)y*a + 0.5f);
	z = (uint)((float)z*a + 0.5f);
	w = (uint)((float)w*a + 0.5f);
	return *this;
}
inline vxuint4& vxuint4::operator /= ( float a )
{
	x = (uint)((float)x/a + 0.5f);
	y = (uint)((float)y/a + 0.5f);
	z = (uint)((float)z/a + 0.5f);
	w = (uint)((float)w/a + 0.5f);
	return *this;
}
inline vxuint4 vxuint4::operator + ( const vxuint4& a ) const
{
	return vxuint4(x + a.x, y + a.y, z + a.z, w + a.w);
}
inline vxuint4 vxuint4::operator * ( float a ) const
{
	return vxuint4((uint)((float)x * a + 0.5f), (uint)((float)y * a + 0.5f), (uint)((float)z * a + 0.5f), (uint)((float)w * a + 0.5f));
}
inline vxuint4 vxuint4::operator / ( float a ) const
{
	return vxuint4((uint)((float)x / a + 0.5f), (uint)((float)y / a + 0.5f), (uint)((float)z / a + 0.5f), (uint)((float)w / a + 0.5f));
}
inline vxuint4 operator * ( float f, const struct vxuint4& a)
{
	return vxuint4((uint)(f * (float)a.x + 0.5f), (uint)(f * (float)a.y + 0.5f), (uint)(f * (float)a.z + 0.5f), (uint)(f * (float)a.w + 0.5f));
}
inline bool	vxuint4::operator == ( const vxuint4& a ) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}
inline bool vxuint4::operator != ( const vxuint4& a ) const
{
	return x != a.x && y != a.y && z != a.z && w != a.w;
}
#pragma endregion

#pragma region // vxbyte2 Operators
inline vxbyte2& vxbyte2::operator += ( const vxbyte2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxbyte2& vxbyte2::operator *= ( float a )
{
	x = (byte)((float)x*a + 0.5f);
	y = (byte)((float)y*a + 0.5f);
	return *this;
}
inline vxbyte2& vxbyte2::operator /= ( float a )
{
	x = (byte)((float)x/a + 0.5f);
	y = (byte)((float)y/a + 0.5f);
	return *this;
}
inline vxbyte2 vxbyte2::operator + ( const vxbyte2& a ) const
{
	return vxbyte2(x + a.x, y + a.y);
}
inline vxbyte2 vxbyte2::operator * ( float a ) const
{
	return vxbyte2((byte)((float)x * a + 0.5f), (byte)((float)y * a + 0.5f));
}
inline vxbyte2 vxbyte2::operator / ( float a ) const
{
	return vxbyte2((byte)((float)x / a + 0.5f), (byte)((float)y / a + 0.5f));
}
inline vxbyte2 operator * ( float f, const struct vxbyte2& a)
{
	return vxbyte2((byte)(f * (float)a.x + 0.5f), (byte)(f * (float)a.y + 0.5f));
}
inline bool	vxbyte2::operator == ( const vxbyte2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxbyte2::operator != ( const vxbyte2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxbyte4 Operators
inline vxbyte4& vxbyte4::operator += ( const vxbyte4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}
inline vxbyte4& vxbyte4::operator *= ( float a )
{
	x = (byte)((float)x*a + 0.5f);
	y = (byte)((float)y*a + 0.5f);
	z = (byte)((float)z*a + 0.5f);
	w = (byte)((float)w*a + 0.5f);
	return *this;
}
inline vxbyte4& vxbyte4::operator /= ( float a )
{
	x = (byte)((float)x/a + 0.5f);
	y = (byte)((float)y/a + 0.5f);
	z = (byte)((float)z/a + 0.5f);
	w = (byte)((float)w/a + 0.5f);
	return *this;
}
inline vxbyte4 vxbyte4::operator + ( const vxbyte4& a ) const
{
	return vxbyte4(x + a.x, y + a.y, z + a.z, w + a.w);
}
inline vxbyte4 vxbyte4::operator * ( float a ) const
{
	return vxbyte4((byte)((float)x * a + 0.5f), (byte)((float)y * a + 0.5f), (byte)((float)z * a + 0.5f), (byte)((float)w * a + 0.5f));
}
inline vxbyte4 vxbyte4::operator / ( float a ) const
{
	return vxbyte4((byte)((float)x / a + 0.5f), (byte)((float)y / a + 0.5f), (byte)((float)z / a + 0.5f), (byte)((float)w / a + 0.5f));
}
inline vxbyte4 operator * ( float f, const struct vxbyte4& a)
{
	return vxbyte4((byte)(f * (float)a.x + 0.5f), (byte)(f * (float)a.y + 0.5f), (byte)(f * (float)a.z + 0.5f), (byte)(f * (float)a.w + 0.5f));
}
inline bool	vxbyte4::operator == ( const vxbyte4& a ) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}
inline bool vxbyte4::operator != ( const vxbyte4& a ) const
{
	return x != a.x && y != a.y && z != a.z && w != a.w;
}
#pragma endregion

#pragma region // vxushort2 Operators
inline vxushort2& vxushort2::operator += ( const vxushort2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxushort2& vxushort2::operator *= ( float a )
{
	x = (ushort)((float)x*a + 0.5f);
	y = (ushort)((float)y*a + 0.5f);
	return *this;
}
inline vxushort2& vxushort2::operator /= ( float a )
{
	x = (ushort)((float)x/a + 0.5f);
	y = (ushort)((float)y/a + 0.5f);
	return *this;
}
inline vxushort2 vxushort2::operator + ( const vxushort2& a ) const
{
	return vxushort2(x + a.x, y + a.y);
}
inline vxushort2 vxushort2::operator * ( float a ) const
{
	return vxushort2((ushort)((float)x * a + 0.5f), (ushort)((float)y * a + 0.5f));
}
inline vxushort2 vxushort2::operator / ( float a ) const
{
	return vxushort2((ushort)((float)x / a + 0.5f), (ushort)((float)y / a + 0.5f));
}
inline vxushort2 operator * ( float f, const struct vxushort2& a)
{
	return vxushort2((ushort)(f * (float)a.x + 0.5f), (ushort)(f * (float)a.y + 0.5f));
}
inline bool	vxushort2::operator == ( const vxushort2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxushort2::operator != ( const vxushort2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxshort2 Operators
inline vxshort2& vxshort2::operator += ( const vxshort2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxshort2& vxshort2::operator -= ( const vxshort2& a )
{
	x -= a.x;
	y -= a.y;
	return *this;
}
inline vxshort2& vxshort2::operator *= ( float a )
{
	x = (short)((float)x*a + 0.5f);
	y = (short)((float)y*a + 0.5f);
	return *this;
}
inline vxshort2& vxshort2::operator /= ( float a )
{
	x = (short)((float)x/a + 0.5f);
	y = (short)((float)y/a + 0.5f);
	return *this;
}
inline vxshort2 vxshort2::operator + () const
{
	return (*this);
}
inline vxshort2 vxshort2::operator - () const
{
	return vxshort2(-x, -y);
}
inline vxshort2 vxshort2::operator + ( const vxshort2& a ) const
{
	return vxshort2(x + a.x, y + a.y);
}
inline vxshort2 vxshort2::operator - ( const vxshort2& a ) const
{
	return vxshort2(x - a.x, y - a.y);
}
inline vxshort2 vxshort2::operator * ( float a ) const
{
	return vxshort2((short)((float)x * a + 0.5f), (short)((float)y * a + 0.5f));
}
inline vxshort2 vxshort2::operator / ( float a ) const
{
	return vxshort2((short)((float)x / a + 0.5f), (short)((float)y / a + 0.5f));
}
inline vxshort2 operator * ( float f, const struct vxshort2& a)
{
	return vxshort2((short)(f * (float)a.x + 0.5f), (short)(f * (float)a.y + 0.5f));
}
inline bool	vxshort2::operator == ( const vxshort2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxshort2::operator != ( const vxshort2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxushort3 Operators
inline vxushort3& vxushort3::operator += ( const vxushort3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}
inline vxushort3& vxushort3::operator -= ( const vxushort3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}
inline vxushort3& vxushort3::operator *= ( float a )
{
	x = (ushort)((float)x*a + 0.5f);
	y = (ushort)((float)y*a + 0.5f);
	z = (ushort)((float)z*a + 0.5f);
	return *this;
}
inline vxushort3& vxushort3::operator /= ( float a )
{
	x = (ushort)((float)x/a + 0.5f);
	y = (ushort)((float)y/a + 0.5f);
	z = (ushort)((float)z/a + 0.5f);
	return *this;
}
inline vxushort3 vxushort3::operator + () const
{
	return (*this);
}
inline vxushort3 vxushort3::operator - () const
{
	return vxushort3(-x, -y, -z);
}
inline vxushort3 vxushort3::operator + ( const vxushort3& a ) const
{
	return vxushort3(x + a.x, y + a.y, z + a.z);
}
inline vxushort3 vxushort3::operator - ( const vxushort3& a ) const
{
	return vxushort3(x - a.x, y - a.y, z - a.z);
}
inline vxushort3 vxushort3::operator * ( float a ) const
{
	return vxushort3((ushort)((float)x * a + 0.5f), (ushort)((float)y * a + 0.5f), (ushort)((float)z * a + 0.5f));
}
inline vxushort3 vxushort3::operator / ( float a ) const
{
	return vxushort3((ushort)((float)x / a + 0.5f), (ushort)((float)y / a + 0.5f), (ushort)((float)z / a + 0.5f));
}
inline vxushort3 operator * ( float f, const struct vxushort3& a)
{
	return vxushort3((ushort)(f * (float)a.x + 0.5f), (ushort)(f * (float)a.y + 0.5f), (ushort)(f * (float)a.z + 0.5f));
}
inline bool	vxushort3::operator == ( const vxushort3& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}
inline bool vxushort3::operator != ( const vxushort3& a ) const
{
	return x != a.x && y != a.y && z != a.z;
}
#pragma endregion

#pragma region // vxchar2 Operators
inline vxchar2& vxchar2::operator += ( const vxchar2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxchar2& vxchar2::operator -= ( const vxchar2& a )
{
	x -= a.x;
	y -= a.y;
	return *this;
}
inline vxchar2& vxchar2::operator *= ( float a )
{
	x = (char)((float)x*a + 0.5f);
	y = (char)((float)y*a + 0.5f);
	return *this;
}
inline vxchar2& vxchar2::operator /= ( float a )
{
	x = (char)((float)x/a + 0.5f);
	y = (char)((float)y/a + 0.5f);
	return *this;
}
inline vxchar2 vxchar2::operator + () const
{
	return (*this);
}
inline vxchar2 vxchar2::operator - () const
{
	return vxchar2(-x, -y);
}
inline vxchar2 vxchar2::operator + ( const vxchar2& a ) const
{
	return vxchar2(x + a.x, y + a.y);
}
inline vxchar2 vxchar2::operator - ( const vxchar2& a ) const
{
	return vxchar2(x - a.x, y - a.y);
}
inline vxchar2 vxchar2::operator * ( float a ) const
{
	return vxchar2((char)((float)x * a + 0.5f), (char)((float)y * a + 0.5f));
}
inline vxchar2 vxchar2::operator / ( float a ) const
{
	return vxchar2((char)((float)x / a + 0.5f), (char)((float)y / a + 0.5f));
}
inline vxchar2 operator * ( float f, const struct vxchar2& a)
{
	return vxchar2((char)(f * (float)a.x + 0.5f), (char)(f * (float)a.y + 0.5f));
}
inline bool	vxchar2::operator == ( const vxchar2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxchar2::operator != ( const vxchar2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxint2 Operators
inline vxint2& vxint2::operator += ( const vxint2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxint2& vxint2::operator -= ( const vxint2& a )
{
	x -= a.x;
	y -= a.y;
	return *this;
}
inline vxint2& vxint2::operator *= ( float a )
{
	x = (int)((float)x*a + 0.5f);
	y = (int)((float)y*a + 0.5f);
	return *this;
}
inline vxint2& vxint2::operator /= ( float a )
{
	x = (int)((float)x/a + 0.5f);
	y = (int)((float)y/a + 0.5f);
	return *this;
}
inline vxint2 vxint2::operator + () const
{
	return (*this);
}
inline vxint2 vxint2::operator - () const
{
	return vxint2(-x, -y);
}
inline vxint2 vxint2::operator + ( const vxint2& a ) const
{
	return vxint2(x + a.x, y + a.y);
}
inline vxint2 vxint2::operator - ( const vxint2& a ) const
{
	return vxint2(x - a.x, y - a.y);
}
inline vxint2 vxint2::operator * ( float a ) const
{
	return vxint2((int)((float)x * a + 0.5f), (int)((float)y * a + 0.5f));
}
inline vxint2 vxint2::operator / ( float a ) const
{
	return vxint2((int)((float)x / a + 0.5f), (int)((float)y / a + 0.5f));
}
inline vxint2 operator * ( float f, const struct vxint2& a)
{
	return vxint2((int)(f * (float)a.x + 0.5f), (int)(f * (float)a.y + 0.5f));
}
inline bool	vxint2::operator == ( const vxint2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxint2::operator != ( const vxint2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxint4 Operators
inline vxint4& vxint4::operator += ( const vxint4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}
inline vxint4& vxint4::operator -= ( const vxint4& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}
inline vxint4& vxint4::operator *= ( float a )
{
	x = (int)((float)x*a + 0.5f);
	y = (int)((float)y*a + 0.5f);
	z = (int)((float)z*a + 0.5f);
	w = (int)((float)w*a + 0.5f);
	return *this;
}
inline vxint4& vxint4::operator /= ( float a )
{
	x = (int)((float)x/a + 0.5f);
	y = (int)((float)y/a + 0.5f);
	z = (int)((float)z/a + 0.5f);
	w = (int)((float)w/a + 0.5f);
	return *this;
}
inline vxint4 vxint4::operator + () const
{
	return (*this);
}
inline vxint4 vxint4::operator - () const
{
	return vxint4(-x, -y, -z, -w);
}
inline vxint4 vxint4::operator + ( const vxint4& a ) const
{
	return vxint4(x + a.x, y + a.y, z + a.z, w + a.w);
}
inline vxint4 vxint4::operator - ( const vxint4& a ) const
{
	return vxint4(x - a.x, y - a.y, z - a.z, w - a.w);
}
inline vxint4 vxint4::operator * ( float a ) const
{
	return vxint4((int)((float)x * a + 0.5f), (int)((float)y * a + 0.5f), (int)((float)z * a + 0.5f), (int)((float)w * a + 0.5f));
}
inline vxint4 vxint4::operator / ( float a ) const
{
	return vxint4((int)((float)x / a + 0.5f), (int)((float)y / a + 0.5f), (int)((float)z / a + 0.5f), (int)((float)w / a + 0.5f));
}
inline vxint4 operator * ( float f, const struct vxint4& a)
{
	return vxint4((int)(f * (float)a.x + 0.5f), (int)(f * (float)a.y + 0.5f), (int)(f * (float)a.z + 0.5f), (int)(f * (float)a.w + 0.5f));
}
inline bool	vxint4::operator == ( const vxint4& a ) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}
inline bool vxint4::operator != ( const vxint4& a ) const
{
	return x != a.x && y != a.y && z != a.z && w != a.w;
}
#pragma endregion

#pragma region // vxint3 Operators
inline vxint3& vxint3::operator += ( const vxint3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}
inline vxint3& vxint3::operator -= ( const vxint3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}
inline vxint3& vxint3::operator *= ( float a )
{
	x = (int)((float)x*a + 0.5f);
	y = (int)((float)y*a + 0.5f);
	z = (int)((float)z*a + 0.5f);
	return *this;
}
inline vxint3& vxint3::operator /= ( float a )
{
	x = (int)((float)x/a + 0.5f);
	y = (int)((float)y/a + 0.5f);
	z = (int)((float)z/a + 0.5f);
	return *this;
}
inline vxint3 vxint3::operator + () const
{
	return (*this);
}
inline vxint3 vxint3::operator - () const
{
	return vxint3(-x, -y, -z);
}
inline vxint3 vxint3::operator + ( const vxint3& a ) const
{
	return vxint3(x + a.x, y + a.y, z + a.z);
}
inline vxint3 vxint3::operator - ( const vxint3& a ) const
{
	return vxint3(x - a.x, y - a.y, z - a.z);
}
inline vxint3 vxint3::operator * ( float a ) const
{
	return vxint3((int)((float)x * a + 0.5f), (int)((float)y * a + 0.5f), (int)((float)z * a + 0.5f));
}
inline vxint3 vxint3::operator / ( float a ) const
{
	return vxint3((int)((float)x / a + 0.5f), (int)((float)y / a + 0.5f), (int)((float)z / a + 0.5f));
}
inline vxint3 operator * ( float f, const struct vxint3& a)
{
	return vxint3((int)(f * (float)a.x + 0.5f), (int)(f * (float)a.y + 0.5f), (int)(f * (float)a.z + 0.5f));
}
inline bool	vxint3::operator == ( const vxint3& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}
inline bool vxint3::operator != ( const vxint3& a ) const
{
	return x != a.x && y != a.y && z != a.z;
}
#pragma endregion

#pragma region // vxdouble3 Operators
inline vxdouble3& vxdouble3::operator += ( const vxdouble3& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}
inline vxdouble3& vxdouble3::operator -= ( const vxdouble3& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}
inline vxdouble3& vxdouble3::operator *= ( double a )
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}
inline vxdouble3& vxdouble3::operator /= ( double a )
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}
inline vxdouble3 vxdouble3::operator + () const
{
	return (*this);
}
inline vxdouble3 vxdouble3::operator - () const
{
	return vxdouble3(-x, -y, -z);
}
inline vxdouble3 vxdouble3::operator + ( const vxdouble3& a ) const
{
	return vxdouble3(x + a.x, y + a.y, z + a.z);
}
inline vxdouble3 vxdouble3::operator - ( const vxdouble3& a ) const
{
	return vxdouble3(x - a.x, y - a.y, z - a.z);
}
inline vxdouble3 vxdouble3::operator * ( double a ) const
{
	return vxdouble3(x * a, y * a, z * a);
}
inline vxdouble3 vxdouble3::operator / ( double a ) const
{
	return vxdouble3(x / a, y / a, z / a);
}
inline vxdouble3 operator * ( double f, const struct vxdouble3& a)
{
	return vxdouble3(f * a.x, f * a.y, f * a.z);
}
inline bool	vxdouble3::operator == ( const vxdouble3& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}
inline bool vxdouble3::operator != ( const vxdouble3& a ) const
{
	return x != a.x && y != a.y && z != a.z;
}
#pragma endregion

#pragma region // vxdouble2 Operators
inline vxdouble2& vxdouble2::operator += ( const vxdouble2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxdouble2& vxdouble2::operator -= ( const vxdouble2& a )
{
	x -= a.x;
	y -= a.y;
	return *this;
}
inline vxdouble2& vxdouble2::operator *= ( double a )
{
	x *= a;
	y *= a;
	return *this;
}
inline vxdouble2& vxdouble2::operator /= ( double a )
{
	x /= a;
	y /= a;
	return *this;
}
inline vxdouble2 vxdouble2::operator + () const
{
	return (*this);
}
inline vxdouble2 vxdouble2::operator - () const
{
	return vxdouble2(-x, -y);
}
inline vxdouble2 vxdouble2::operator + ( const vxdouble2& a ) const
{
	return vxdouble2(x + a.x, y + a.y);
}
inline vxdouble2 vxdouble2::operator - ( const vxdouble2& a ) const
{
	return vxdouble2(x - a.x, y - a.y);
}
inline vxdouble2 vxdouble2::operator * ( double a ) const
{
	return vxdouble2(x * a, y * a);
}
inline vxdouble2 vxdouble2::operator / ( double a ) const
{
	return vxdouble2(x / a, y / a);
}
inline vxdouble2 operator * ( double f, const struct vxdouble2& a)
{
	return vxdouble2(f * a.x, f * a.y);
}
inline bool	vxdouble2::operator == ( const vxdouble2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxdouble2::operator != ( const vxdouble2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxdouble4 Operators
inline vxdouble4& vxdouble4::operator += ( const vxdouble4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}
inline vxdouble4& vxdouble4::operator -= ( const vxdouble4& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}
inline vxdouble4& vxdouble4::operator *= ( double a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}
inline vxdouble4& vxdouble4::operator /= ( double a )
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}
inline vxdouble4 vxdouble4::operator + () const
{
	return (*this);
}
inline vxdouble4 vxdouble4::operator - () const
{
	return vxdouble4(-x, -y, -z, -w);
}
inline vxdouble4 vxdouble4::operator + ( const vxdouble4& a ) const
{
	return vxdouble4(x + a.x, y + a.y, z + a.z, w + a.w);
}
inline vxdouble4 vxdouble4::operator - ( const vxdouble4& a ) const
{
	return vxdouble4(x - a.x, y - a.y, z - a.z, w - a.w);
}
inline vxdouble4 vxdouble4::operator * ( double a ) const
{
	return vxdouble4(x * a, y * a, z * a, w * a);
}
inline vxdouble4 vxdouble4::operator / ( double a ) const
{
	return vxdouble4(x / a, y / a, z / a, w / a);
}
inline vxdouble4 operator * ( double f, const struct vxdouble4& a)
{
	return vxdouble4(f * a.x, f * a.y, f * a.z, f * a.w);
}
inline bool	vxdouble4::operator == ( const vxdouble4& a ) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}
inline bool vxdouble4::operator != ( const vxdouble4& a ) const
{
	return x != a.x && y != a.y && z != a.z && w != a.w;
}
#pragma endregion

#pragma region // vxfloat2 Operators
inline vxfloat2& vxfloat2::operator += ( const vxfloat2& a )
{
	x += a.x;
	y += a.y;
	return *this;
}
inline vxfloat2& vxfloat2::operator -= ( const vxfloat2& a )
{
	x -= a.x;
	y -= a.y;
	return *this;
}
inline vxfloat2& vxfloat2::operator *= ( float a )
{
	x *= a;
	y *= a;
	return *this;
}
inline vxfloat2& vxfloat2::operator /= ( float a )
{
	x /= a;
	y /= a;
	return *this;
}
inline vxfloat2 vxfloat2::operator + () const
{
	return (*this);
}
inline vxfloat2 vxfloat2::operator - () const
{
	return vxfloat2(-x, -y);
}
inline vxfloat2 vxfloat2::operator + ( const vxfloat2& a ) const
{
	return vxfloat2(x + a.x, y + a.y);
}
inline vxfloat2 vxfloat2::operator - ( const vxfloat2& a ) const
{
	return vxfloat2(x - a.x, y - a.y);
}
inline vxfloat2 vxfloat2::operator * ( float a ) const
{
	return vxfloat2(x * a, y * a);
}
inline vxfloat2 vxfloat2::operator / ( float a ) const
{
	return vxfloat2(x / a, y / a);
}
inline vxfloat2 operator * ( float f, const struct vxfloat2& a)
{
	return vxfloat2(f * a.x, f * a.y);
}
inline bool	vxfloat2::operator == ( const vxfloat2& a ) const
{
	return x == a.x && y == a.y;
}
inline bool vxfloat2::operator != ( const vxfloat2& a ) const
{
	return x != a.x && y != a.y;
}
#pragma endregion

#pragma region // vxfloat4 Operators
inline vxfloat4& vxfloat4::operator += ( const vxfloat4& a )
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}
inline vxfloat4& vxfloat4::operator -= ( const vxfloat4& a )
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}
inline vxfloat4& vxfloat4::operator *= ( float a )
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}
inline vxfloat4& vxfloat4::operator /= ( float a )
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}
inline vxfloat4 vxfloat4::operator + () const
{
	return (*this);
}
inline vxfloat4 vxfloat4::operator - () const
{
	return vxfloat4(-x, -y, -z, -w);
}
inline vxfloat4 vxfloat4::operator + ( const vxfloat4& a ) const
{
	return vxfloat4(x + a.x, y + a.y, z + a.z, w + a.w);
}
inline vxfloat4 vxfloat4::operator - ( const vxfloat4& a ) const
{
	return vxfloat4(x - a.x, y - a.y, z - a.z, w - a.w);
}
inline vxfloat4 vxfloat4::operator * ( float a ) const
{
	return vxfloat4(x * a, y * a, z * a, w * a);
}
inline vxfloat4 vxfloat4::operator / ( float a ) const
{
	return vxfloat4(x / a, y / a, z / a, w / a);
}
inline vxfloat4 operator * ( float f, const struct vxfloat4& a)
{
	return vxfloat4(f * a.x, f * a.y, f * a.z, f * a.w);
}
inline bool	vxfloat4::operator == ( const vxfloat4& a ) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}
inline bool vxfloat4::operator != ( const vxfloat4& a ) const
{
	return x != a.x && y != a.y && z != a.z && w != a.w;
}
#pragma endregion

#pragma region // vxmatrix Operators
inline
	vxmatrix::vxmatrix( float f11, float f12, float f13, float f14,
	float f21, float f22, float f23, float f24,
	float f31, float f32, float f33, float f34,
	float f41, float f42, float f43, float f44 )
{
	_m11 = f11; _m12 = f12; _m13 = f13; _m14 = f14;
	_m21 = f21; _m22 = f22; _m23 = f23; _m24 = f24;
	_m31 = f31; _m32 = f32; _m33 = f33; _m34 = f34;
	_m41 = f41; _m42 = f42; _m43 = f43; _m44 = f44;
}
inline float&
	vxmatrix::operator () ( uint iRow, uint iCol )
{
	return m[iRow][iCol];
}
inline float
	vxmatrix::operator () ( uint iRow, uint iCol ) const
{
	return m[iRow][iCol];
}
inline
	vxmatrix::operator float* ()
{
	return (float *) &_m11;
}
inline
	vxmatrix::operator const float* () const
{
	return (const float *) &_m11;
}
inline vxmatrix&
	vxmatrix::operator *= ( const vxmatrix& mat )
{
	VXHMMatrixMultiply(this, this, &mat);
	return *this;
}
inline vxmatrix&
	vxmatrix::operator += ( const vxmatrix& mat )
{
	_m11 += mat._m11; _m12 += mat._m12; _m13 += mat._m13; _m14 += mat._m14;
	_m21 += mat._m21; _m22 += mat._m22; _m23 += mat._m23; _m24 += mat._m24;
	_m31 += mat._m31; _m32 += mat._m32; _m33 += mat._m33; _m34 += mat._m34;
	_m41 += mat._m41; _m42 += mat._m42; _m43 += mat._m43; _m44 += mat._m44;
	return *this;
}
inline vxmatrix&
	vxmatrix::operator -= ( const vxmatrix& mat )
{
	_m11 -= mat._m11; _m12 -= mat._m12; _m13 -= mat._m13; _m14 -= mat._m14;
	_m21 -= mat._m21; _m22 -= mat._m22; _m23 -= mat._m23; _m24 -= mat._m24;
	_m31 -= mat._m31; _m32 -= mat._m32; _m33 -= mat._m33; _m34 -= mat._m34;
	_m41 -= mat._m41; _m42 -= mat._m42; _m43 -= mat._m43; _m44 -= mat._m44;
	return *this;
}
inline vxmatrix&
	vxmatrix::operator *= ( float f )
{
	_m11 *= f; _m12 *= f; _m13 *= f; _m14 *= f;
	_m21 *= f; _m22 *= f; _m23 *= f; _m24 *= f;
	_m31 *= f; _m32 *= f; _m33 *= f; _m34 *= f;
	_m41 *= f; _m42 *= f; _m43 *= f; _m44 *= f;
	return *this;
}
inline vxmatrix&
	vxmatrix::operator /= ( float f )
{
	float fInv = 1.0f / f;
	_m11 *= fInv; _m12 *= fInv; _m13 *= fInv; _m14 *= fInv;
	_m21 *= fInv; _m22 *= fInv; _m23 *= fInv; _m24 *= fInv;
	_m31 *= fInv; _m32 *= fInv; _m33 *= fInv; _m34 *= fInv;
	_m41 *= fInv; _m42 *= fInv; _m43 *= fInv; _m44 *= fInv;
	return *this;
}
inline vxmatrix
	vxmatrix::operator + () const
{
	return *this;
}
inline vxmatrix
	vxmatrix::operator - () const
{
	return vxmatrix(-_m11, -_m12, -_m13, -_m14,
		-_m21, -_m22, -_m23, -_m24,
		-_m31, -_m32, -_m33, -_m34,
		-_m41, -_m42, -_m43, -_m44);
}
inline vxmatrix
	vxmatrix::operator * ( const vxmatrix& mat ) const
{
	vxmatrix matT;
	VXHMMatrixMultiply(&matT, this, &mat);
	return matT;
}

inline vxmatrix
	vxmatrix::operator + ( const vxmatrix& mat ) const
{
	return vxmatrix(_m11 + mat._m11, _m12 + mat._m12, _m13 + mat._m13, _m14 + mat._m14,
		_m21 + mat._m21, _m22 + mat._m22, _m23 + mat._m23, _m24 + mat._m24,
		_m31 + mat._m31, _m32 + mat._m32, _m33 + mat._m33, _m34 + mat._m34,
		_m41 + mat._m41, _m42 + mat._m42, _m43 + mat._m43, _m44 + mat._m44);
}
inline vxmatrix
	vxmatrix::operator - ( const vxmatrix& mat ) const
{
	return vxmatrix(_m11 - mat._m11, _m12 - mat._m12, _m13 - mat._m13, _m14 - mat._m14,
		_m21 - mat._m21, _m22 - mat._m22, _m23 - mat._m23, _m24 - mat._m24,
		_m31 - mat._m31, _m32 - mat._m32, _m33 - mat._m33, _m34 - mat._m34,
		_m41 - mat._m41, _m42 - mat._m42, _m43 - mat._m43, _m44 - mat._m44);
}
inline vxmatrix
	vxmatrix::operator * ( float f ) const
{
	return vxmatrix(_m11 * f, _m12 * f, _m13 * f, _m14 * f,
		_m21 * f, _m22 * f, _m23 * f, _m24 * f,
		_m31 * f, _m32 * f, _m33 * f, _m34 * f,
		_m41 * f, _m42 * f, _m43 * f, _m44 * f);
}

inline vxmatrix
	vxmatrix::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return vxmatrix(_m11 * fInv, _m12 * fInv, _m13 * fInv, _m14 * fInv,
		_m21 * fInv, _m22 * fInv, _m23 * fInv, _m24 * fInv,
		_m31 * fInv, _m32 * fInv, _m33 * fInv, _m34 * fInv,
		_m41 * fInv, _m42 * fInv, _m43 * fInv, _m44 * fInv);
}
inline vxmatrix
	operator * ( float f, const vxmatrix& mat )
{
	return vxmatrix(f * mat._m11, f * mat._m12, f * mat._m13, f * mat._m14,
		f * mat._m21, f * mat._m22, f * mat._m23, f * mat._m24,
		f * mat._m31, f * mat._m32, f * mat._m33, f * mat._m34,
		f * mat._m41, f * mat._m42, f * mat._m43, f * mat._m44);
}
inline bool
	vxmatrix::operator == ( const vxmatrix& mat ) const
{
	return 0 == memcmp(this, &mat, sizeof(vxmatrix));
}

inline bool
	vxmatrix::operator != ( const vxmatrix& mat ) const
{
	return 0 != memcmp(this, &mat, sizeof(vxmatrix));
}
#pragma endregion

#pragma region // vxmatrixdouble Operators
inline
	vxmatrixdouble::vxmatrixdouble( double f11, double f12, double f13, double f14,
	double f21, double f22, double f23, double f24,
	double f31, double f32, double f33, double f34,
	double f41, double f42, double f43, double f44 )
{
	_m11 = f11; _m12 = f12; _m13 = f13; _m14 = f14;
	_m21 = f21; _m22 = f22; _m23 = f23; _m24 = f24;
	_m31 = f31; _m32 = f32; _m33 = f33; _m34 = f34;
	_m41 = f41; _m42 = f42; _m43 = f43; _m44 = f44;
}
inline double&
	vxmatrixdouble::operator () ( uint iRow, uint iCol )
{
	return m[iRow][iCol];
}
inline double
	vxmatrixdouble::operator () ( uint iRow, uint iCol ) const
{
	return m[iRow][iCol];
}
inline
	vxmatrixdouble::operator double* ()
{
	return (double *) &_m11;
}
inline
	vxmatrixdouble::operator const double* () const
{
	return (const double *) &_m11;
}
inline vxmatrixdouble&
	vxmatrixdouble::operator *= ( const vxmatrixdouble& mat )
{
	VXHMDoubleMatrixMultiply(this, this, &mat);
	return *this;
}
inline vxmatrixdouble&
	vxmatrixdouble::operator += ( const vxmatrixdouble& mat )
{
	_m11 += mat._m11; _m12 += mat._m12; _m13 += mat._m13; _m14 += mat._m14;
	_m21 += mat._m21; _m22 += mat._m22; _m23 += mat._m23; _m24 += mat._m24;
	_m31 += mat._m31; _m32 += mat._m32; _m33 += mat._m33; _m34 += mat._m34;
	_m41 += mat._m41; _m42 += mat._m42; _m43 += mat._m43; _m44 += mat._m44;
	return *this;
}
inline vxmatrixdouble&
	vxmatrixdouble::operator -= ( const vxmatrixdouble& mat )
{
	_m11 -= mat._m11; _m12 -= mat._m12; _m13 -= mat._m13; _m14 -= mat._m14;
	_m21 -= mat._m21; _m22 -= mat._m22; _m23 -= mat._m23; _m24 -= mat._m24;
	_m31 -= mat._m31; _m32 -= mat._m32; _m33 -= mat._m33; _m34 -= mat._m34;
	_m41 -= mat._m41; _m42 -= mat._m42; _m43 -= mat._m43; _m44 -= mat._m44;
	return *this;
}
inline vxmatrixdouble&
	vxmatrixdouble::operator *= ( double f )
{
	_m11 *= f; _m12 *= f; _m13 *= f; _m14 *= f;
	_m21 *= f; _m22 *= f; _m23 *= f; _m24 *= f;
	_m31 *= f; _m32 *= f; _m33 *= f; _m34 *= f;
	_m41 *= f; _m42 *= f; _m43 *= f; _m44 *= f;
	return *this;
}
inline vxmatrixdouble&
	vxmatrixdouble::operator /= ( double f )
{
	double fInv = 1.0f / f;
	_m11 *= fInv; _m12 *= fInv; _m13 *= fInv; _m14 *= fInv;
	_m21 *= fInv; _m22 *= fInv; _m23 *= fInv; _m24 *= fInv;
	_m31 *= fInv; _m32 *= fInv; _m33 *= fInv; _m34 *= fInv;
	_m41 *= fInv; _m42 *= fInv; _m43 *= fInv; _m44 *= fInv;
	return *this;
}
inline vxmatrixdouble
	vxmatrixdouble::operator + () const
{
	return *this;
}
inline vxmatrixdouble
	vxmatrixdouble::operator - () const
{
	return vxmatrixdouble(-_m11, -_m12, -_m13, -_m14,
		-_m21, -_m22, -_m23, -_m24,
		-_m31, -_m32, -_m33, -_m34,
		-_m41, -_m42, -_m43, -_m44);
}
inline vxmatrixdouble
	vxmatrixdouble::operator * ( const vxmatrixdouble& mat ) const
{
	vxmatrixdouble matT;
	VXHMDoubleMatrixMultiply(&matT, this, &mat);
	return matT;
}

inline vxmatrixdouble
	vxmatrixdouble::operator + ( const vxmatrixdouble& mat ) const
{
	return vxmatrixdouble(_m11 + mat._m11, _m12 + mat._m12, _m13 + mat._m13, _m14 + mat._m14,
		_m21 + mat._m21, _m22 + mat._m22, _m23 + mat._m23, _m24 + mat._m24,
		_m31 + mat._m31, _m32 + mat._m32, _m33 + mat._m33, _m34 + mat._m34,
		_m41 + mat._m41, _m42 + mat._m42, _m43 + mat._m43, _m44 + mat._m44);
}
inline vxmatrixdouble
	vxmatrixdouble::operator - ( const vxmatrixdouble& mat ) const
{
	return vxmatrixdouble(_m11 - mat._m11, _m12 - mat._m12, _m13 - mat._m13, _m14 - mat._m14,
		_m21 - mat._m21, _m22 - mat._m22, _m23 - mat._m23, _m24 - mat._m24,
		_m31 - mat._m31, _m32 - mat._m32, _m33 - mat._m33, _m34 - mat._m34,
		_m41 - mat._m41, _m42 - mat._m42, _m43 - mat._m43, _m44 - mat._m44);
}
inline vxmatrixdouble
	vxmatrixdouble::operator * ( double f ) const
{
	return vxmatrixdouble(_m11 * f, _m12 * f, _m13 * f, _m14 * f,
		_m21 * f, _m22 * f, _m23 * f, _m24 * f,
		_m31 * f, _m32 * f, _m33 * f, _m34 * f,
		_m41 * f, _m42 * f, _m43 * f, _m44 * f);
}

inline vxmatrixdouble
	vxmatrixdouble::operator / ( double f ) const
{
	double fInv = 1.0f / f;
	return vxmatrixdouble(_m11 * fInv, _m12 * fInv, _m13 * fInv, _m14 * fInv,
		_m21 * fInv, _m22 * fInv, _m23 * fInv, _m24 * fInv,
		_m31 * fInv, _m32 * fInv, _m33 * fInv, _m34 * fInv,
		_m41 * fInv, _m42 * fInv, _m43 * fInv, _m44 * fInv);
}
inline vxmatrixdouble
	operator * ( double f, const vxmatrixdouble& mat )
{
	return vxmatrixdouble(f * mat._m11, f * mat._m12, f * mat._m13, f * mat._m14,
		f * mat._m21, f * mat._m22, f * mat._m23, f * mat._m24,
		f * mat._m31, f * mat._m32, f * mat._m33, f * mat._m34,
		f * mat._m41, f * mat._m42, f * mat._m43, f * mat._m44);
}
inline bool
	vxmatrixdouble::operator == ( const vxmatrixdouble& mat ) const
{
	return 0 == memcmp(this, &mat, sizeof(vxmatrixdouble));
}

inline bool
	vxmatrixdouble::operator != ( const vxmatrixdouble& mat ) const
{
	return 0 != memcmp(this, &mat, sizeof(vxmatrixdouble));
}
#pragma endregion
}
using namespace vxmathproj;

/**
 * @package vxobjects
 * @brief VXFramework의 Global Data Structures 및 VXObject Classes를 모은 namespace
 */
namespace vxobjects
{
//=========================
// Object Structures
//=========================
/**
 * @class SVXPlane
 * @brief Plane을 정의하는 자료구조
 */
struct SVXPlane{
	/// Plane 상의 한 점의 위치
	vxfloat3 f3PosPointsOnPlane;
	/// Plane 의 Normal Vector
	vxfloat3 f3VecNormal;
	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXPlane(){ VXSTRUCTURENULLSET(f3PosPointsOnPlane); VXSTRUCTURENULLSET(f3VecNormal); }
};
/**
 * @class SVXOrthoBox
 * @brief 현재 좌표계의 Axis에 평행한 Box를 정의하는 자료구조
 */
struct SVXOrthoBox{
	/// 현재 좌표계의 Axis에 평행한 Box에 대한 최대점 위치
	vxfloat3 f3PosMaxBox;
	/// 현재 좌표계의 Axis에 평행한 Box에 대한 최소점 위치
	vxfloat3 f3PosMinBox;
	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXOrthoBox(){ VXSTRUCTURENULLSET(f3PosMaxBox); VXSTRUCTURENULLSET(f3PosMinBox); }
	SVXOrthoBox(vxint3 i3SizeVolume)	{
		f3PosMinBox = vxfloat3(-0.5f, -0.5f, -0.5f);
		vxint3 i3IndexMaxRange = i3SizeVolume - vxint3(1, 1, 1);
		f3PosMaxBox = vxfloat3((float)i3IndexMaxRange.x, (float)i3IndexMaxRange.y, (float)i3IndexMaxRange.z) + vxfloat3(0.5f, 0.5f, 0.5f);

		if(!IsAvailableBox())
			VXERRORMESSAGE("ERROR in SVXOrthoBox");
	}
	/// 현재 좌표계의 SVXOrthoBox 가 유효하게 정의되었는가 확인
	bool IsAvailableBox(){
		if(f3PosMaxBox.x <= f3PosMinBox.x || f3PosMaxBox.y <= f3PosMinBox.y || f3PosMaxBox.z <= f3PosMinBox.z)
			return false;
		return true;
	}
};
/**
 * @class SVXOrthoRect
 * @brief 현재 좌표계(2D)의 Axis에 평행한 Rectangle를 정의하는 자료구조
 */
struct SVXOrthoRect{
	/// 현재 좌표계의 Axis에 평행한 Rectangle에 대한 최대점 위치
	vxfloat2 f2PosMaxRect;
	/// 현재 좌표계의 Axis에 평행한 Rectangle에 대한 최대점 위치
	vxfloat2 f2PosMinRect;
	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXOrthoRect(){ VXSTRUCTURENULLSET(f2PosMaxRect); VXSTRUCTURENULLSET(f2PosMinRect); }
};
/**
 * @class SVXSimpleVolumeDefinition
 * @brief Volume을 저장하는 array 및 이를 위한 최소의 정보를 정의하는 자료구조
 * @sa vxobjects::SVXVolumeDataArchive
 */
struct SVXSimpleVolumeDefinition{
	/// 현재 저장된 Volume의 data type
	EnumVXRDataType eVolumeDataType;
	/// Volume을 저장하고 있는 2D pointer
	void** ppvVolumeSlices;
	/**
	 * @brief Volume 의 크기 i3VolumeSize = (width, height, depth or slices) \n
	 * i3SizeExtraBoundary 가 포함되지 않음
	 */
	vxint3 i3VolumeSize;
	/**
	 * @brief CPU Memory Access Violation을 피하기 위해 System Memory 상의 Extra Boundary 영역의 한쪽면 크기
	 * @details i3SizeExtraBoundary = (한쪽 x축 방향 크기, 한쪽 y축 방향 크기, 한쪽 z축 방향 크기)
	 */
	int iSizeExtraBoundary;
	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXSimpleVolumeDefinition(){ 
		VXSTRUCTURENULLSET(i3VolumeSize); iSizeExtraBoundary = 0;
		ppvVolumeSlices = NULL; eVolumeDataType = vxrDataTypeUNDEFINED;
	}
	/// 포인터 ppvVolumeSlices 에 할당된 메모리 해제
	void Delete(){
		VXSAFE_DELETE2DARRAY(ppvVolumeSlices, i3VolumeSize.z + iSizeExtraBoundary*2);
	}
};

/**
 * @class SVXAlignAxisOS2WS
 * @brief Object Space 정의된 x축(1,0,0), y축(0,1,0), z축(0,0,z)이 처음 World Space 에 배치될 때의 방향을 정의 \n
 * pitch 는 고려되지 않으며 방향만 정의, (즉 vector 에 대해서만 유효함)
 * @sa
 * @ref vxobjects::SVXVolumeDataArchive
 */
struct SVXAlignAxisOS2WS{
	/**
	 * @brief Object Space 정의된 x축(1,0,0)에 대응되는 World Space 상에 배치된 Object의 x축을 정의, unit vector
	 */
	vxfloat3 f3VecAxisOSX2WSX;
	/**
	 * @brief Object Space 정의된 y축(0,1,0)에 대응되는 World Space 상에 배치된 Object의 y축을 정의, unit vector
	 */
	vxfloat3 f3VecAxisOSY2WSY;
	/**
	 * @brief Object Space 정의된 z축(0,0,1)에 대응되는 World Space 상에 배치된 Object의 z축을 정의하기 위한 XY RHS cross vecor 방향의 reverse 여부\n
	 * true 면 RHS 로 배치되며 Affine Space 에서 변환 성립, false 면 LHS 로 z툭이 배치
	 */
	bool bIsAxisZ_RHS;
	/**
	 * @brief f3VecAxisOSX2WSX, f3VecAxisOSY2WSY, bIsAxisZ_RHS 로부터 정의되는 초기 OS2WS 변환 행렬
	 */
	vxmatrix matAlignOS2WS;
	/**
	 * @brief constructor, 초기화 작업 수행
	 * @details 
	 * >> f3VecAxisOSX2WSX = vxfloat3(1, 0, 0);
	 * >> f3VecAxisOSX2WSY = vxfloat3(0, 1, 0);
	 * >> bIsAxisZ_RHS = true;
	 * >> matInitOS2WS is identity matrix
	 */
	SVXAlignAxisOS2WS()
	{
		f3VecAxisOSX2WSX = vxfloat3(1, 0, 0);
		f3VecAxisOSY2WSY = vxfloat3(0, 1, 0);
		bIsAxisZ_RHS = true;
		ComputeInitalMatrix();
	}
	SVXAlignAxisOS2WS(vxfloat3 f3VecAxisOSX2WSX_, vxfloat3 f3VecAxisOSY2WSY_, bool bIsAxisZ_RHS_)
	{
		f3VecAxisOSX2WSX = f3VecAxisOSX2WSX_;
		f3VecAxisOSY2WSY = f3VecAxisOSY2WSY_;
		bIsAxisZ_RHS = bIsAxisZ_RHS_;
		ComputeInitalMatrix();
	}
	/// 정의된 f3VecAxisOSX2WSX와 f3VecAxisOSX2WSY로부터 matInitOS2WS 계산하여 등록
	void ComputeInitalMatrix(){
		vxfloat3 f3VecZRHS;
		VXHMCrossDotVector(&f3VecZRHS, &f3VecAxisOSY2WSY, &f3VecAxisOSX2WSX);	// this is for -z direction
		vxmatrix matT;
		VXHMMatrixWS2CS(&matT, &vxfloat3(0, 0, 0), &f3VecAxisOSY2WSY, &f3VecZRHS);
		VXHMMatrixInverse(&matAlignOS2WS, &matT);
		if(!bIsAxisZ_RHS)
		{
			vxmatrix matInverseZ;
			VXHMMatrixScaling(&matInverseZ, vxfloat3(1.f, 1.f, -1.f));
			matAlignOS2WS = matAlignOS2WS * matInverseZ;
		}
	}
};

/**
 * @class SVXVolumeDataArchive
 * @brief VXFramework에서 정의하는 Volume의 상세 정보를 위한 자료구조
 * @sa
 * @ref vxobjects::CVXVObjectVolume \n
 * vxhelpers::VXHVolumeFillHistogramBasedOnVolumeValues
 */
struct SVXVolumeDataArchive{
	/**
	 * @brief Volume array 의 data type
	 */
	EnumVXRDataType eVolumeDataType;
	/**
	 * @brief 메모리에 저장되기 전 Volume Original data type
	 * @par ex. 
	 * file format float 저장된 볼륨을 ushort 으로 저장할 경우
	 * @par
	 * >> eDataType = vxrDataTypeUNSIGNEDSHORT, eDataTypeOriginal = vxrDataTypeFLOAT;
	 */
	EnumVXRDataType eVolumeDataTypeOriginal;
	/**
	 * @brief Volume을 저장한 2D array
	 * @details 
	 * 실제 할당된 x 축 방향 크기 = i3VolumeSize.x + i3SizeExtraBoundary.x*2 \n
	 * 실제 할당된 y 축 방향 크기 = i3VolumeSize.y + i3SizeExtraBoundary.y*2 \n
	 * 실제 할당된 z 축 방향 크기 = i3VolumeSize.z + i3SizeExtraBoundary.z*2 \n
	 * @par ex. 
	 * ushort 512x512x512 Volume에서 (100, 120, 150) index 값 sample \n
	 * @par
	 * >> int iSamplePosX = 100 + i3SizeExtraBoundary.x; \n
	 * >> int iSamplePosY = 120 + i3SizeExtraBoundary.y; \n
	 * >> int iSamplePosZ = 150 + i3SizeExtraBoundary.z; \n
	 * >> ushort usValue = ((ushort**)ppvVolumeSlices)[iSamplePosZ][iSamplePosX + iSamplePosY*(i3VolumeSize.x + i3SizeExtraBoundary.x*2)];
	 */
	void** ppvVolumeSlices;
	/**
	 * @brief CPU Memory Access Violation을 피하기 위해 System Memory 상의 Extra Boundary 영역의 한쪽면 크기
	 * @details i3SizeExtraBoundary = (한쪽 x축 방향 크기, 한쪽 y축 방향 크기, 한쪽 z축 방향 크기)
	 */
	vxint3 i3SizeExtraBoundary;
	/**
	 * @brief Volume 의 크기 i3VolumeSize = (width, height, depth or slices)
	 * @details i3SizeExtraBoundary 가 포함되지 않음
	 */
	vxint3 i3VolumeSize;
	/**
	 * @brief 단위 Voxel에 대해 OS 상 cell 의 WS 상의 크기
	 * @details d3VoxelPitch = (OS 상의 x 방향 voxel 크기, OS 상의 y 방향 voxel 크기, OS 상의 z 방향 voxel 크기)
	 */
	vxfloat3 f3VoxelPitch;
	/**
	 * @brief Volume으로 저장된(ppvVolumeSlices)의 최소값 d2MinMaxValue.x, 최대값 d2MinMaxValue.y
	 */
	vxdouble2 d2MinMaxValue;
	/**
	 * @brief Volume으로 저장되기 전에 정의된 최소값 d2ActualMinMaxValue.x, 최대값 d2ActualMinMaxValue.y
	 * @par ex. 
	 * file format float으로 -1.5 ~ 2.5 저장된 볼륨을 ushort 으로 저장할 경우 
	 * @par 
	 * >> d2MinMaxValue = vxdouble(0, 65535), d2ActualMinMaxValue = vxdouble(-1.5, 2.5);
	 */
	vxdouble2 d2ActualMinMaxValue;
	/**
	 * @brief Volume에 대한 Histogram 을 정의하는 array
	 * @details 
	 * array 크기는 uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5) \n
	 * pullHistogram[volume value] = # of voxels
	 */
	ullong* pullHistogram;	
	/**
	 * @brief memory 에 저장된 volume space (샘플 좌표)와 초기 world space 에 배치되는 변환 matrix 정의
	 */
	SVXAlignAxisOS2WS svxAlignAxisOS2WS;
	/**
	 * @brief constructor, 초기화 작업 수행
	 * @details 
	 * >> eVolumeDims = {vxrMetricTypePOSITIONX, vxrMetricTypePOSITIONY, vxrMetricTypePOSITIONZ, vxrMetricTypeDENSITY};\n
	 * >> d2ActualMinMaxValue = d2MinMaxValue = vxdouble(DBL_MAX, -DBL_MAX);\n
	 * 그 외는 모두 NULL (or 0, false) 로 초기화
	 */
	SVXVolumeDataArchive(){ VXSTRUCTURENULLSET(i3VolumeSize); VXSTRUCTURENULLSET(f3VoxelPitch); 
		VXSTRUCTURENULLSET(i3SizeExtraBoundary);
		eVolumeDataType = vxrDataTypeUNDEFINED;
		eVolumeDataTypeOriginal = vxrDataTypeUNDEFINED;

		d2MinMaxValue = d2ActualMinMaxValue = vxdouble2(DBL_MAX, -DBL_MAX);
		ppvVolumeSlices = NULL;
		pullHistogram = NULL;
	}
	
	/**
	 * @brief Histogram array 의 크기를 얻음, uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5)
	 */
	uint GetHistogramSize(){return (uint)(max(d2MinMaxValue.y - d2MinMaxValue.x + 1.5, 1.0));}
	/**
	 * @brief ppvVolumeSlices array 크기를 얻음, Extra Boundary가 적용된 크기
	 */
	vxint3 GetSampleSize(){return vxint3(i3VolumeSize.x + i3SizeExtraBoundary.x*2, i3VolumeSize.y + i3SizeExtraBoundary.y*2, 
		i3VolumeSize.z + i3SizeExtraBoundary.z*2);}

	/// 포인터 ppvVolumeSlices 및 pullHistogram 에 할당된 메모리 해제
	void Delete(){
		VXSAFE_DELETE2DARRAY(ppvVolumeSlices, i3VolumeSize.z + i3SizeExtraBoundary.z*2);
		VXSAFE_DELETEARRAY(pullHistogram);
	}
};

/**
 * @class SVXPrimitiveDataArchive
 * @brief VXFramework에서 정의하는 Primitive 로 이루어진 객체의 상세 정보를 위한 자료구조
 * @sa vxobjects::CVXVObjectPrimitive
 */

struct SVXPrimitiveDataArchive{
public:
	/**
	 * @brief vertex array를 저장하는 container map
	 * memory 할당된 pointer를 value로 갖고, @ref SVXPrimitiveDataArchive::Delete 에서 해제됨.
	 */
	map<EnumVXRVertexType, vxfloat3*> mapVerticeDefinitions;
public:
	/**
	 * @brief Primitive로 구성 된 객체의 Polygon 의 normal vector 기준의 vertex 배열 방향
	 */
	bool bIsPrimitiveFrontCCW;
	/**
	 * @brief Primitive Type
	 */
	EnumVXRPrimitiveType ePrimitiveType;
	/**
	 * @brief Primitive를 정의하는 vertex의 배열 기준, true : stripe, false : list
	 */
	bool bIsPrimitiveStripe;
	/**
	 * @brief Primitive로 구성 된 객체의 Polygon 개수
	 */
	uint uiNumPolygons;
	/**
	 * @brief 하나의 Primitive(Polygon)을 정의하는 index 개수
	 */
	uint uiStrideIndexListPerPolygon;
	/**
	 * @brief Vertex의 Index기반으로 Polygon을 정의하기 위한 Index Buffer (puiIndexList) 의 크기
	 * @details
	 * >> if(bIsPrimitiveStripe)\n
	 * >>    uiNumIndice = uiNumPolygons + (uiStrideIndexListPerPolygon - 1);\n
	 * >> else\n
	 * >>    uiNumIndice = uiNumPolygons*uiStrideIndexListPerPolygon;
	 */
	uint uiNumIndice;
	/**
	 * @brief Vertex의 Index기반으로 Polygon를 위한 Index Buffer가 정의된 array
	 */
	uint* puiIndexList;
	/**
	 * @brief Primitive로 구성 된 객체의 Vertex 개수
	 */
	uint uiNumVertice;
	/**
	 * @brief SVXPrimitiveDataArchive 단위의 OS 상에서 정의되는 bounding box
	 */
	SVXOrthoBox svxVertexBoundingOrthoBox;
	/**
	 * @brief Texture Resource 에 대한 Pointer \n
	 */
	void* pvTextureResource;

	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXPrimitiveDataArchive(){ 
		bIsPrimitiveFrontCCW = false; uiNumPolygons = 0; uiNumVertice = 0;
		uiStrideIndexListPerPolygon = uiNumIndice = 0;
		ePrimitiveType = vxrPrimitiveTypeUNDEFINED;
		bIsPrimitiveStripe = false;
		puiIndexList = NULL;
		pvTextureResource = NULL;
	}
	/*!
	 * @fn void vxobjects::SVXPrimitiveDataArchive::Delete()
	 * @brief 포인터 puiIndexList 및 mapVerticeDefinitions 의 value로 정의된 pointer 에 할당된 메모리 해제
	*/
	void Delete(){
		VXSAFE_DELETEARRAY(puiIndexList);
		VXSAFE_DELETEARRAY(pvTextureResource);
		//for(auto itrVertex3D = mapVerticeDefinitions.begin(); itrVertex3D != mapVerticeDefinitions.end(); itrVertex3D++)
		for(map<EnumVXRVertexType, vxfloat3*>::iterator itrVertex3D = mapVerticeDefinitions.begin(); itrVertex3D != mapVerticeDefinitions.end(); itrVertex3D++)
		{
			VXSAFE_DELETEARRAY(itrVertex3D->second);
		}
		mapVerticeDefinitions.clear();
	}
	/*!
	 * @fn vxfloat3* vxobjects::SVXPrimitiveDataArchive::GetVerticeDefinition(EnumVXRVertexType eVertexType)
	 * @brief mapVerticeDefinitions 의 value로 정의된 pointer 반환하는 method, 
	 * @param eVertexType [in] \n EnumVXRVertexType \n vertex buffer의 이름, (mapVerticeDefinitions의 key)
	 * @return vxfloat3 \n vertex buffer를 정의하는 vxfloat3의 포인터. 없으면 NULL 반환
	 */
	vxfloat3* GetVerticeDefinition(EnumVXRVertexType eVertexType){
		map<EnumVXRVertexType, vxfloat3*>::iterator itrVtxDef = mapVerticeDefinitions.find(eVertexType);
		if(itrVtxDef == mapVerticeDefinitions.end())
			return NULL;
		return itrVtxDef->second;
	}
	/*!
	 * @fn void vxobjects::SVXPrimitiveDataArchive::ReplaceOrAddVerticeDefinition(EnumVXRVertexType eVertexType, vxfloat3* pf3Vertex)
	 * @brief mapVerticeDefinitions 의 value로 정의된 pointer 반환하는 method, 
	 * @param eVertexType [in] \n EnumVXRVertexType \n vertex buffer의 이름, (mapVerticeDefinitions의 key)
	 * @param pf3Vertex [in] \n vxfloat3* \n vertex buffer를 정의하는 vxfloat3의 포인터.
	 * @remarks EnumVXRVertexType 를 키로 갖기 때문에 이미 등록된 vertex definition 이 있을 경우, \n
	 * 해당 definition 의 vertex pointer 를 메모리에서 해제하고 새로운 vertex pointer 를 등록
	 */
	void ReplaceOrAddVerticeDefinition(EnumVXRVertexType eVertexType, vxfloat3* pf3Vertex){
		vxfloat3* pfVertex = GetVerticeDefinition(eVertexType);
		if(pfVertex != NULL)
		{
			VXSAFE_DELETEARRAY(pfVertex);
			mapVerticeDefinitions.erase(eVertexType);
		}
		mapVerticeDefinitions.insert(pair<EnumVXRVertexType, vxfloat3*>(eVertexType, pf3Vertex));
	}
	/*!
	 * @fn int vxobjects::SVXPrimitiveDataArchive::GetNumVertexDefinitions()
	 * @brief 등록된 vertex definition 의 개수를 얻음
	 * @return int \n 등록된 vertex definition 의 개수
	 */
	int GetNumVertexDefinitions()
	{
		return (int)mapVerticeDefinitions.size();
	}
	/*!
	 * @fn void vxobjects::SVXPrimitiveDataArchive::ClearVertexDefinitionContainer()
	 * @brief mapVerticeDefinitions 를 clear 함. 
	 * @remarks 등록된 vertex pointer 에 대한 메모리 해제는 하지 않고 container 만 clear 함.
	 */
	void ClearVertexDefinitionContainer()
	{
		mapVerticeDefinitions.clear();
	}
};

/**
 * @class SVXTransferFunctionArchive
 * @brief VXFramework에서 정의하는 OTF에 대한 상세 정보를 위한 자료구조
 * @sa vxobjects::CVXTObject
 */
struct SVXTransferFunctionArchive{
	/**
	 * @brief OTF array 의 pointer
	 * @details 
	 * 1D : [0][0 to (i3DimSize.x - 1)] - default, [1][0 to (i3DimSize.x - 1)] - summed preintegration
	 * 2D : [0][0 to (i3DimSize.x*i3DimSize.y - 1)] - default, [...][0 to (i3DimSize.x*i3DimSize.y - 1)] - customized
	 * 3D : [0 to (i3DimSize.z - 1)][0 to (i3DimSize.x*i3DimSize.y - 1)]
	 */
	void** ppvArchiveTF;
	/**
	 * @brief OTF array 의 pointer dimension
	 * @details iNumDims = 1 or 2 or 3
	 */
	int iNumDims;	
	/**
	 * @brief OTF array 에 할당되어 있는 각각의 dimension에 대한 OTF array 값 중 최소값
	 * @details
	 * i3ValidMinIndex.x : 1st dimension 의 array 의 최소값 \n
	 * i3ValidMinIndex.y : 2nd dimension 의 array 의 최소값 \n
	 * i3ValidMinIndex.z : 3rd dimension 의 array 의 최소값
	 */
	vxint3 i3ValidMinIndex;
	/**
	 * @brief OTF array 에 할당되어 있는 각각의 dimension에 대한 OTF array 값 중 최대값
	 * @details
	 * i3ValidMaxIndex.x : 1st dimension 의 array 의 최대값 \n
	 * i3ValidMaxIndex.y : 2nd dimension 의 array 의 최대값 \n
	 * i3ValidMaxIndex.z : 3rd dimension 의 array 의 최대값
	 */
	vxint3 i3ValidMaxIndex;
	/**
	 * @brief OTF array 의 각 dimension 에 대한 크기
	 * @details 
	 * i3DimSizes.x : 1st dimension 의 array 크기 \n
	 * i3DimSizes.y : 2nd dimension 의 array 크기 \n
	 * i3DimSizes.z : 3rd dimension 의 array 크기 \n
	 * Valid dimension에 대하여 i3DimSizes.xyz > 0, Invalid demension에 대하여 i3DimSizes.xyz <= 0
	 */
	vxint3 i3DimSize;
	/**
	 * @brief OTF metric의 기준이 되는 Volume 값의 범위에 대한 bin 크기
	 * @par ex.
	 * 16 bit Volume Data에 대한 2D OTF (density, gradient magnitude), 각각의 범위를 0~65535 라고 가정할 때 \n
	 * 512x1024 크기의 2D OTF 를 정의하면, bin 의 XY 크기는 (65536/512, 65536/1024) 가 됨.
	 */
	vxdouble3 d3BinSize;
	/**
	 * @brief OTF array 값에 대한 data type
	 */
	EnumVXRDataType eValueDataType;
	/**
	 * @brief constructor, 모두 0 (NULL or false)으로 초기화
	 */
	SVXTransferFunctionArchive(){
		ppvArchiveTF = NULL;
		iNumDims = 0;
		VXSTRUCTURENULLSET(i3ValidMinIndex);
		VXSTRUCTURENULLSET(i3ValidMaxIndex);
		VXSTRUCTURENULLSET(i3DimSize);
		VXSTRUCTURENULLSET(d3BinSize);
		eValueDataType = vxrDataTypeUNDEFINED;
	}

	/*!
	 * @fn void vxobjects::SVXTransferFunctionArchive::Delete()
	 * @brief OTF array 에 대한 포인터 ppvArchiveTF 에 할당된 memory 해제
	*/
	void Delete(){
		switch(iNumDims)
		{
		case 1:
		case 2:
			VXSAFE_DELETE2DARRAY(ppvArchiveTF, i3DimSize.y);
			break;
		case 3:
			VXSAFE_DELETE2DARRAY(ppvArchiveTF, i3DimSize.z);
			break;
		default:
			break;
		}
	}
};

/**
 * @class SVXVolumeBlock
 * @brief block 기반의 volume에 대한 자료구조
 * @sa vxobjects::CVXVObjectVolume, vxobjects::SVXVolumeDataArchive
 */
struct SVXVolumeBlock{
	/**
	 * @brief block 의 최소 최대 값에 대한 단일 channel data type
	 * @details 일반적으로 vxobjects::SVXVolumeDataArchive::eVolumeDataType 와 같음
	 */
	EnumVXRDataType eVolumeMinMaxDataType;
	/**
	 * @brief 단위 block의 크기
	 * @details extra boundary가 포함되지 않은 크기 \n
	 * i3BlockSize = vxint3(size of x, size of y, size of z)
	 */
	vxint3 i3BlockSize;
	/**
	 * @brief 하나의 SVXVolumeBlock 에 각 축 방향으로 정의된 block들의 개수
	 * @details 모든 block의 개수 = i3BlocksNumber.x * i3BlocksNumber.y * i3BlocksNumber.z;
	 */
	vxint3 i3BlocksNumber;
	/**
	 * @brief block 기반의 volume을 저장하는 ppvVolumeBricks 에 할당된 extra boundary 의 한쪽면 크기
	 * @details 
	 * xyz 방향으로 모두 동일 
	 */
	int iBrickExtraSize;
	/**
	 * @brief block기반의 volume을 저장한 2D array
	 * @details 
	 * ppvVolumeBricks[block id][xyz sample position in a block] \n
	 * 단위 block에 실제 할당된 x 축 방향 크기 = i3BlockSize.x + iBrickExtraSize*2 \n
	 * 단위 block에 실제 할당된 y 축 방향 크기 = i3BlockSize.y + iBrickExtraSize*2 \n
	 * 단위 block에 실제 할당된 z 축 방향 크기 = i3BlockSize.z + iBrickExtraSize*2 
	 * @par ex. 
	 * 8x8x8 block을 단위로 정의된 512x512x512 volume (ushort) 에서 OS의 (100, 100, 100) 좌표를 sampling \n 
	 * 이 경우 i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
	 * 
	 * @par
	 * >> vxint3 i3BlockId = vxint3(floor(100/8), floor(100/8), floor(100/8)); \n
	 * >> int iBlockIdIndex = i3BlockId.x + i3BlockId.y*(i3BlocksNumber.x) + i3BlockId.y*(i3BlocksNumber.x*i3BlocksNumber.y); \n
	 * >> vxint3 i3PosSampleInBlock = vxint3(100%8, 100%8, 100%8); \n
	 * >> int iBlockSampleSizeX = i3BlockSize.x + iBrickExtraSize*2; \n
	 * >> int iBlockSampleSizeXY = iBlockSampleSizeX*(i3BlockSize.y + iBrickExtraSize*2); \n
	 * >> int iBlockSampleIndex = (i3PosSampleInBlock.x + iBrickExtraSize) + (i3SampleBlockPos.y + iBrickExtraSize)*iBlockSampleSizeX + (i3SampleBlockPos.z + iBrickExtraSize)*iBlockSampleSizeXY; \n
	 * >> ushort usValue = ((ushort**)ppvVolumeBricks)[iBlockIdIndex][iBlockSampleIndex];
	 */
	void** ppvVolumeBricks;
	/**
	 * @brief block 단위의 최소 최대값을 저장하는 1D array
	 * @details 
	 * array의 크기는 block의 전체 개수와 동일, extra boundary을 고려하지 않음 \n
	 * data type은 2 channel을 갖고 volume type과 같음. x : 최소값, y : 최대값
	 * @par ex. 
	 * 8x8x8 block을 단위로 정의된 512x512x512 volume (ushort) 에서 OS의 (100, 100, 100) 좌표에 해당하는 block의 최소 최대값 \n 
	 * 이 경우 i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
	 *
	 * @par
	 * >> vxint3 i3BlockId = vxint3(floor(100/8), floor(100/8), floor(100/8)); \n
	 * >> int iBlockIdIndex = i3BlockId.x + i3BlockId.y*(i3BlocksNumber.x) + i3BlockId.y*(i3BlocksNumber.x*i3BlocksNumber.y); \n
	 * >> vxushort2 d2minmax = ((vxushort2*)pvMinMaxBlocks)[iBlockIdIndex];
	 */
	void* pvMinMaxBlocks;			
	/**
	 * @brief block 단위로 binary tag가 지정된 1D array
	 * @details 
	 * array의 크기는 block의 전체 개수와 동일, extra boundary을 고려하지 않음 \n
	 * @par ex. 
	 * 8x8x8 block을 단위로 정의된 512x512x512 volume (ushort) 에서 OS의 (100, 100, 100) 좌표에 해당하는 block의 tag \n 
	 * 이 경우 i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
	 *
	 * @par
	 * >> vxint3 i3BlockId = vxint3(floor(100/8), floor(100/8), floor(100/8)); \n
	 * >> int iBlockIdIndex = i3BlockId.x + i3BlockId.y*(i3BlocksNumber.x) + i3BlockId.y*(i3BlocksNumber.x*i3BlocksNumber.y); \n
	 * >> bool bIsTagged = pbTaggedActivatedBlocks[iBlockIdIndex];
	 */
	bool* pbTaggedActivatedBlocks;	
	
	/**
	 * @brief block 기반 volume의 data type
	 * @details 일반적으로 vxobjects::SVXVolumeDataArchive::eVolumeDataType 와 같음
	 */
	EnumVXRDataType eVolumeBrickDataType;
	/**
	 * @brief @ref vxobjects::SVXVolumeDataArchive::i3SizeExtraBoundary 가 반영된 좌표에 대한 xyz의 block id table
	 * @details 
	 * block x id = piBlockIndexMap[0][0 to (SVXVolumeDataArchive::i3VolumeSize.x + SVXVolumeDataArchive::i3SizeExtraBoundary.x*2)] \n
	 * block y id = piBlockIndexMap[1][0 to (SVXVolumeDataArchive::i3VolumeSize.y + SVXVolumeDataArchive::i3SizeExtraBoundary.y*2)] \n
	 * block z id = piBlockIndexMap[2][0 to (SVXVolumeDataArchive::i3VolumeSize.z + SVXVolumeDataArchive::i3SizeExtraBoundary.z*2)]
	 */
	int* piBlockIndexMap[3];

	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXVolumeBlock(){ 
		VXSTRUCTURENULLSET(i3BlockSize); VXSTRUCTURENULLSET(i3BlocksNumber);
		pvMinMaxBlocks = NULL; ppvVolumeBricks = NULL; pbTaggedActivatedBlocks = NULL; 
		piBlockIndexMap[0] = NULL; piBlockIndexMap[1] = NULL; piBlockIndexMap[2] = NULL;
		iBrickExtraSize = 0;
		eVolumeBrickDataType = eVolumeMinMaxDataType = vxrDataTypeUNDEFINED;
	}

	/*!
	 * @fn void vxobjects::SVXVolumeBlock::Delete()
	 * @brief 할당된 memory 모두 해제
	*/
	void Delete(){ 
		VXSAFE_DELETE2DARRAY(ppvVolumeBricks, i3BlocksNumber.z*i3BlocksNumber.y*i3BlocksNumber.x);
		VXSAFE_DELETEARRAY(pvMinMaxBlocks);
		VXSAFE_DELETEARRAY(pbTaggedActivatedBlocks);
		VXSAFE_DELETEARRAY(piBlockIndexMap[0]);
		VXSAFE_DELETEARRAY(piBlockIndexMap[1]);
		VXSAFE_DELETEARRAY(piBlockIndexMap[2]);
		iBrickExtraSize = 0;
		eVolumeBrickDataType = eVolumeMinMaxDataType = vxrDataTypeUNDEFINED;
	}
};

/**
 * @class SVXFrameBuffer
 * @brief VXFramework에서 정의하는 frame buffer에 대한 상세 정보를 위한 자료구조
 * @sa vxobjects::CVXIObject
 */
struct SVXFrameBuffer{
	/**
	 * @brief frame buffer 의 width
	 */
	int iWidth;
	/**
	 * @brief frame buffer 의 height
	 */
	int iHeight;
	/**
	 * @brief array로 정의된 frame buffer
	 */
	void* pvBuffer;
	/**
	 * @brief frame buffer의 data type
	 */
	EnumVXRDataType ePixelDataType;
	/**
	 * @brief frame buffer의 한 pixel에 대한 (data type) 크기 (bytes)
	 */
	uint uiSizeBytesPerPixel;
	/**
	 * @brief frame buffer의 사용 용도
	 * @details eFrameBufferUsage == vxrFrameBufferUsageRENDEROUT 일 경우, 반드시 vxrDataTypeBYTE4CHANNEL 로 설정됨.
	 */
	EnumVXRFrameBufferUsage eFrameBufferUsage;
	/**
	 * @brief frame buffer에 대한 mention
	 */
	wstring strHelpMent;

#ifdef __WINDOWS
	/**
	 * @brief win32에서 file memory를 통한 buffer interoperation을 위한 handle
	 */
	HANDLE hFileMap;
#endif

	/// constructor, 모두 0 (NULL or false)으로 초기화
	SVXFrameBuffer(){ iWidth = iHeight = 0;
		pvBuffer = NULL;
		ePixelDataType = vxrDataTypeUNDEFINED;
		eFrameBufferUsage = vxrFrameBufferUsageCUSTOM;
		strHelpMent.clear();
#ifdef __WINDOWS
		hFileMap = NULL;
#endif
	}

	/*!
	 * @fn void vxobjects::SVXFrameBuffer::Delete()
	 * @brief 할당된 memory 모두 해제
	 */
	void Delete(){
		iWidth = iHeight = 0; 
		if(eFrameBufferUsage == vxrFrameBufferUsageRENDEROUT)
		{
#ifdef __WINDOWS
			UnmapViewOfFile(pvBuffer);
			CloseHandle(hFileMap);
			hFileMap = NULL;
#endif
#ifdef __MACOS
			delete[] pvBuffer;
#endif
			pvBuffer = NULL;
		}
		else
		{
			VXSAFE_DELETEARRAY(pvBuffer);
		}
	}
};

/**
 * @class SVXLocalProgress
 * @brief VXFramework에서 정의하는 progress 에 대한 자료구조
 */
struct SVXLocalProgress{
	/**
	 * @brief progress의 시작, 0.0 에서 100.0 사이
	 */
	double dStartProgress;
	/**
	 * @brief progress의 범위, 0.0 에서 100.0 사이
	 */
	double dRangeProgress;
	/**
	 * @brief progress의 현재 진행 정도가 기록되는 module/function 내의 static parameter에 대한 포인터
	 */
	double* pdProgressOfCurWork; /*out*/
	/// constructor, 초기화
	SVXLocalProgress()
	{
		dStartProgress = 0;
		dRangeProgress = 100;
		pdProgressOfCurWork = NULL;
	}

	/*!
	 * @fn void vxobjects::SVXLocalProgress::Init()
	 * @brief >> *pdProgressOfCurWork = dStartProgress;
	 */
	void Init()
	{
		*pdProgressOfCurWork = dStartProgress;
	}

	/*!
	 * @fn void vxobjects::SVXLocalProgress::SetProgress(double dProgress, double dTotal)
	 * @brief >> *pdProgressOfCurWork = dStartProgress + dRangeProgress*dProgress/dTotal;
	 */
	void SetProgress(double dProgress, double dTotal)
	{
		*pdProgressOfCurWork = 
			dStartProgress + dRangeProgress*dProgress/dTotal;
	}

	/*!
	 * @fn void vxobjects::SVXLocalProgress::Deinit()
	 * @brief >> dStartProgress = *pdProgressOfCurWork;
	 */
	void Deinit()
	{
		dStartProgress = *pdProgressOfCurWork;
	}
};

//=========================
// Global Objects
//=========================
struct SVXVirtualArchiveObject;
/**
 * @class CVXObject
 * @brief VXObject의 최상위 class 로 VXObject family의 공통 parameter를 갖음
 */
__vxstaticclass CVXObject
{
private:
protected:
	SVXVirtualArchiveObject* m_pVAO;

public:
	CVXObject();
	~CVXObject();
	
	/*!
	 * @brief VXObject의 content가 정의되어 있는가를 확인
	 * @remarks content는 VXObject를 상송 받는 최하위 VXObject에서 정의됨
	 * @li @ref vxobjects::CVXIObject
	 * @li @ref vxobjects::CVXLObject
	 * @li @ref vxobjects::CVXTObject
	 * @li @ref vxobjects::CVXVObjectVolume
	 * @li @ref vxobjects::CVXVObjectPrimitive
	 */
	bool IsDefinedResource();
	/*!
	 * @brief VXObject의 visibility를 설정
	 * @param bVisibility [in] \n bool \n true 면 visible, false 면 hidden
	 * @remarks customized purpose
	 */
	void SetVisibility(bool bVisibility);
	/*!
	 * @brief VXObject의 visibility를 설정
	 * @param bVisibility [in] \n bool \n boolean visibility
	 * @return bool \n visible 이면 true, hidden 이면 false 반환
	 * @remarks customized purpose
	 */
	bool GetVisibility() const;
	/*!
	 * @brief VXObject의 Object ID 설정
	 * @param iObjectID [in] \n int \n 32 bit ID
	 * @remarks 
	 * 일반적으로 Resource Manager (@ref CVXResourceManager) 에서 지정 \n
	 * [8bit : Object Type][8bit : Magic Bits][16 bit : Count-based ID]
	 */
	void SetObjectID(int iObjectID);
	/*!
	 * @brief VXObject의 Object ID를 얻음
	 * @return int \n Object ID를 반환
	 */
	int GetObjectID() const;
	/*!
	 * @brief VXObject를 정의하는데 사용된 가장 연관성 높은 VXObject의 ID를 설정
	 * @param iReferenceObjectID [in] \n int \n VXObject를 정의하는데 사용된 가장 연관성 높은 VXObject의 ID
	 * @remarks default ID는 0으로 설정되어 있음
	 */
	void SetReferenceObjectID(int iReferenceObjectID);
	/*!
	 * @brief VXObject를 정의하는데 사용된 가장 연관성 높은 VXObject의 ID를 얻음
	 * @return int \n VXObject를 정의하는데 사용된 가장 연관성 높은 VXObject의 ID
	 * @remarks 연관된 VXObject의 ID가 설정되어 있지 않은 경우 0을 반환
	 */
	int GetReferenceObjectID() const;
	/*!
	 * @brief VXObject에 대한 사용자 mention을 설정
	 * @param strMent [in] \n wstring \n 저장할 VXObject에 대한 사용자 mention
	 */
	void SetHelptMent(wstring strMent);
	/*!
	 * @brief VXObject에 대한 사용자 mention을 얻음
	 * @return wstring \n VXObject에 대한 사용자 mention 반환
	 */
	wstring GetHelptMent() const;
	/*!
	 * @brief VXObject를 정의한 module대한 정보를 기록함
	 * @param strDefinedByModuleSpecifier [in] \n wstring \n VXObject를 정의한 module대한 mention
	 */
	void SetDefineModuleSpecifier(wstring strDefinedByModuleSpecifier);
	/*!
	 * @brief VXObject를 정의한 module대한 정보를 기록한 mention을 얻음
	 * @return wstring \n VXObject를 정의한 module대한 정보를 기록한 mention 반환
	 */
	wstring GetDefineModuleSpecifier();
	
	/*!
	 * @brief 정의된 VXObject의 type을 얻음 
	 * @return EnumVXRObjectType \n 정의된 VXObject의 type 반환
	 * @remarks VXObject가 정의되어 있는 상태면 이것에 따라 VXObject의 instance를 생성함
	 */
	EnumVXRObjectType GetObjectType();

	// Custom Parameter
	/*!
	 * @brief VXObject에 임의의 문자열을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param strData [in] \n wstring \n 저장할 문자열
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, wstring strData);
	/*!
	 * @brief VXObject에 임의의 double형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param dData [in] \n double \n 저장할 double형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, double dData);
	/*!
	 * @brief VXObject에 임의의 vxdouble2형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param d2Data [in] \n vxdouble2 \n 저장할 vxdouble2형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble2 d2Data);
	/*!
	 * @brief VXObject에 임의의 vxdouble3형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param d3Data [in] \n vxdouble3 \n 저장할 vxdouble3형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble3 d3Data);
	/*!
	 * @brief VXObject에 임의의 vxdouble4형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param d4Data [in] \n vxdouble4 \n 저장할 vxdouble4형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble4 d4Data);
	/*!
	 * @brief VXObject에 임의의 int형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param iData [in] \n int \n 저장할 int형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, int iData);
	/*!
	 * @brief VXObject에 임의의 vxint2형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param i2Data [in] \n vxint2 \n 저장할 vxint2형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint2 i2Data);
	/*!
	 * @brief VXObject에 임의의 vxint3형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param i3Data [in] \n vxint3 \n 저장할 vxint3형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint3 i3Data);
	/*!
	 * @brief VXObject에 임의의 vxint4형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param i4Data [in] \n vxint4 \n 저장할 vxint4형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint4 i4Data);
	/*!
	 * @brief VXObject에 임의의 bool형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param bData [in] \n bool \n 저장할 bool형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, bool bData);
	/*!
	 * @brief VXObject에 임의의 vxmatrix형 값을 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param matData [in] \n vxmatrix \n 저장할 vxmatrix형 값
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 그외의 특정 형식으로 저장해도 무관.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxmatrix matData);
	/*!
	 * @brief VXObject에 임의의 포인터를 등록함
	 * @param strStandardParameterName [in] \n wstring \n 임의로 저장할 data에 대한 key name
	 * @param pData [in] \n void \n 저장할 임의의 void형의 포인터
	 * @return bool \n 등록이 성공하면 true, 그렇지 않으면 false
	 * @remarks 
	 * 일반적으로 key 문자열은 @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 에서 지원하는 형식으로 씀\n
	 * 포인터가 가리키는 주소만 저장하며 해당 포인터에 할당된 memory가 있을 경우 이를 관리하지는 않음.
	 */
	bool GetCustomParameter(wstring strStandardParameterName, void* pData/*out*/) const; 

	// Static Helper Functions //
	/*!
	 * @brief VXObject ID 값으로부터 object type을 반환하는 static helper 함수
	 * @param iObjectID [in] \n int \n VXObject의 ID
	 * @return EnumVXRObjectType \n VXObject의 ID에 인코딩되어 있는 object type
	 * @remarks 
	 * VXObject ID 형식에 대해서 지원.\n
	 * VXObject Magic Key를 통해 지원 예정 ::: 차후 패치
	 */
	static EnumVXRObjectType GetObjectTypeFromID(int iObjectID);
	/*!
	 * @brief VXObject ID 값으로부터 object type이 VXVObject인가를 확인하는 static helper 함수
	 * @param iObjectID [in] \n int \n VXObject의 ID
	 * @return bool \n VXVObject이면 true, 그렇지 않으면 false 반환
	 * @remarks 
	 * VXObject ID 형식에 대해서 지원.\n
	 * VXObject Magic Key를 통해 지원 예정 ::: 차후 패치
	 */
	static bool IsVObjectType(int iObjectID);
};

struct SVXVirtualArchiveVObject;
/**
 * @class CVXVObject
 * @brief VXObject를 상속 받으며 VXVObjectVolume과 VXVObjectPrimitive의 공간 정보를 갖는 상위 class
 * @sa vxobjects::CVXVObjectVolume, vxobjects::CVXVObjectPrimitive
 */
__vxstaticclass CVXVObject : public CVXObject
{
private:
protected:
	// Defined In Object Space!! //
	SVXVirtualArchiveVObject* m_pVAVO;

public:
	CVXVObject();
	~CVXVObject();

	/*!
	 * @brief content를 포함하는 axis-aligned bounding box in OS 를 얻음
	 * @param psvxOrthoBox [out] \n SVXOrthoBox \n object space (OS) 에서의 axis-aligned bounding box를 정의될 SVXOrthoBox의 포인터
	 */
	void GetOrthoBoundingBox(SVXOrthoBox* psvxOrthoBox/*out*/);
	
	/*!
	 * @brief OS가 WS로 배치되는 것이 정의되었는지를 확인하는 함수
	 * @return bool \n 정의되어 있으면 true, 그렇지 않으면 false
	 */
	bool IsGeometryDefined();
	
	/*!
	 * @brief VXVObject의 customized 색상을 설정
	 * @param f4Color [in] \n vxfloat4 \n normalized color RGBA (xyzw)
	 * @remarks 반드시 이 색상으로 보이는 것은 아니며, VXVObject를 가시화하는 module에서 어떻게 처리하는가에 따름.
	 */
	void SetVObjectColor(vxfloat4 f4Color);
	/*!
	 * @brief VXVObject의 customized 색상을 얻음
	 * @return vxfloat4 \n VXVObject의 customized 색상을 반환
	 */
	vxfloat4 GetVObjectColor() const;

	// Object Origin Resource //
	/*!
	 * @brief VXVObject의 content가 복사가 아닌 다른 VXObject의 source를 reference로 사용될 때 해당 VXObject의 ID를 얻음
	 * @return int \n source VXObject의 ID를 반환
	 */
	int GetSourceVObjectID() const;
	
	// Transform //
	/*!
	 * @brief VXVObject를 OS나 WS에서 회전 변환함
	 * @param pf3VecRotationAxis [in] \n vxfloat3 \n 해당 space에서의 회전축을 정의하고 있는 normalized vector가 저장된 vxfloat3의 포인터
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n 현재 변환이 이루어질 space
	 * @param fAngleDegree [in] \n float \n 회전각, radian
	 * @remarks VXFramework은 RHS를 기본으로 설정하고 있으며, 일반적으로 회전 방향은 오른 나사를 따름. (엄밀히는 설정된 좌표계가 RHS나 LHS인가에 따름)
	 */
	void TransformRotation(vxfloat3* pf3VecRotationAxis, EnumVXVCoordSpace eCoordSpace, float fAngleDegree);
	/*!
	 * @brief VXVObject를 OS나 WS에서 확대/축소 변환함
	 * @param pf3ScaleFactor [in] \n vxfloat3 \n 해당 space에서의 축방향을 기준으로 한 scaling factor가 저장된 vxfloat3의 포인터
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n 현재 변환이 이루어질 space
	 */
	void TransformScale(vxfloat3* pf3ScaleFactor, EnumVXVCoordSpace eCoordSpace);
	/*!
	 * @brief VXVObject를 OS나 WS에서 평행이동 변환함
	 * @param pf3VecTranslationAxis [in] \n vxfloat3 \n 해당 space에서의 이동 방향을 정의하는 normalized vector가 정의된 vxfloat3의 포인터
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n 현재 변환이 이루어질 space
	 * @param fTranslationLength [in] \n float \n 현재 변환이 이루어질 space에서의 이동 길이
	 */
	void TransformTranslation(vxfloat3* pf3VecTranslationAxis, EnumVXVCoordSpace eCoordSpace, float fTranslationLength);
	/*!
	 * @brief VXVObject의 OS와 WS의 변환을 정의하는 matrix를 설정
	 * @param pmatOS2WS [in] \n vxmatrix \n OS와 WS의 변환을 정의하는 matrix를 저장하고 있는 vxmatrix의 포인터
	 * @remarks 내부적으로 OS와 WS 변환과 관련된 좌표계가 재설정되며 관련 matrix가 재설정됨
	 */
	void SetTransformMatrixOS2WS(const vxmatrix* pmatOS2WS);
	/*!
	 * @brief VXVObject에서 정의되어 있는 OS의 WS의 변환을 정의하는 matrix를 얻음
	 * @return vxmatrix \n OS의 WS의 변환을 정의하는 matrix가 정의된 vxmatrix의 포인터
	 */
	vxmatrix* GetMatrixOS2WS();
	/*!
	 * @brief VXVObject에서 정의되어 있는 WS의 OS의 변환을 정의하는 matrix를 얻음
	 * @return vxmatrix \n WS의 OS의 변환을 정의하는 matrix가 정의된 vxmatrix의 포인터
	 */
	vxmatrix* GetMatrixWS2OS();
};

struct SVXVirtualArchiveVObjectVolume;
/**
 * @class CVXVObjectVolume
 * @brief VXVObject를 통해 OS와 WS의 배치 관계가 설정된 Volume의 정보를 갖는 class
 * @sa vxobjects::CVXVObject
 */
__vxstaticclass CVXVObjectVolume : public CVXVObject	// CT Volume or Processing Result Volume or Histogram (2D : Size(x, y, 1))
{
private:
protected:
	SVXVirtualArchiveVObjectVolume* m_pVAVOVolume;

	void Destroy();

public:
	CVXVObjectVolume();
 	~CVXVObjectVolume();

	// Basic Functions //
	// Block & Brick for Interactive Rendering //
	// Not Hierarchical blocking
	// Octree : level 0, Large Block,  level 1, Small Block
	/*!
	 * @brief volume 정보를 갖고 있는 @ref vxobjects::SVXVolumeDataArchive 자료 구조를 VXVObjectVolume에 등록함
	 * @param psvxVolumeDataArchive [in] \n SVXVolumeDataArchive \n Volume 정보가 정의된 SVXVolumeDataArchive 의 포인터
	 * @param i3BlockSizes[2] [in] \n vxint3 \n 
	 * volume을 정의하는 단위 block 자료구조에서 block 크기를 저장하고 있는 크기 2의 static array에 대한 vxint3의 포인터 \n
	 * block 크기가 지정되면 내부적으로 block 단위의 최소, 최대값 자료구조가 생성되나, block 단위로 volume 재구성되지는 않음 \n
	 * i3BlockSizes[0] : 큰 block, i3BlockSizes[1] : 작은 block \n
	 * NULL 이면 block 생성을 안 함
	 * @param iSourceVolumeID [in] \n int \n 
	 * content의 pointer reference를 다른 VXObjectVolume와 공유할 때 해당 VXObjectVolume의 ID \n
	 * 일반적으로 copy method가 사용되며, pointer 공유를 안 하며, 이 경우 0을 씀. 기본값은 0
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 * 함수가 진행되는 progress 정보를 포함하는 SVXLocalProgress의 포인터 \n
	 * 기본값은 NULL이며, NULL이면 사용 안 함.
	 * @remarks i3BlockSizes이 주어지면 내부적으로 @ref CVXVObjectVolume::GenerateVolumeMinMaxBlocks 이 호출됨, CVXVObjectVolume::GenerateBricks 은 불리지 않음
	 */
	bool RegisterVolumeArchive(SVXVolumeDataArchive* psvxVolumeDataArchive, vxint3 i3BlockSizes[2]/* 0 : Large, 1: Small */, int iSourceVolumeID = 0, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief VXVObjectVolume에 정의되어 있는 volume 정보를 얻음.
	 * @return SVXVolumeDataArchive \n volume 정보가 저장되어 있는 SVXVolumeDataArchive 의 포인터
	 */
	SVXVolumeDataArchive* GetVolumeArchive();

	// Optional //
	/*!
	 * @brief volume 에 대해 block 단위의 최소 최대값을 내부적으로 생성하여 @ref SVXVolumeBlock 자료구조 내에 할당하는 함수
	 * @param iLevel [in] \n int \n block 의 level, 0 or 1
	 * @param i3BlockSize [in] \n vxint3 \n 단위 block 의 크기
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 함수가 진행되는 progress 정보를 포함하는 SVXLocalProgress 의 포인터 \n
	 기본값은 NULL이며, NULL이면 사용 안 함.
	 * @remarks 
	 * 기존에 생성되어 있는 최소, 최대값 block이 class에 등록되어 있으면 해제 후 다시 등록 \n
	 * @ref SVXVolumeBlock 에 최소 최대값에 대한 array만 설정됨
	 * CVXVObjectVolume::GenerateBricks 함수 호출 전에 불려야 하는 함수
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void GenerateVolumeMinMaxBlocks(int iLevel, vxint3 i3BlockSize, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief volume 을 block 단위로 재구성하여 @ref SVXVolumeBlock 자료구조 내에 할당하는 함수
	 * @param iLevel [in] \n int \n block 의 level, 0 or 1
	 * @param iBrickExtraSize [in] \n vxint3 \n 
	 * block 단위로 volume을 재구성할 때 사용할 한쪽면에 대한 extra boundary의 크기, xyz축에 동일하게 사용 \n
	 * 실제 단위 block의 크기는 SVXVolumeBlock::i3BlockSize + vxint3(iBrickExtraSize, iBrickExtraSize, iBrickExtraSize) * 2 로 정의
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 함수가 진행되는 progress 정보를 포함하는 SVXLocalProgress의 포인터 \n
	 기본값은 NULL이며, NULL이면 사용 안 함.
	 * @remarks 
	 * 기존에 생성되어 있는 최소, 최대값 block이 class에 등록되어 있어야 함 \n
	 * 즉, CVXVObjectVolume::GenerateVolumeMinMaxBlocks 이 먼저 불려야 함. \n
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void GenerateBricks(int iLevel, int iBrickExtraSize, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief volume 의 block 자료구조를 얻는 함수
	 * @param iLevel [in] \n int \n block 의 level, 0 or 1
	 * @return  SVXVolumeBlock \n 
	 * volume 의 block 자료구조가 저장된 SVXVolumeBlock의 포인터 \n
	 * volume 및 block 자료구조가 정의되어 있지 않거나 level 값이 잘못 들어가면 NULL 반환.
	 */
	SVXVolumeBlock* GetVolumeBlock(int iLevel);	// 0 or 1
	/*!
	 * @brief volume 의 block 자료구조에서 설정하는 최소, 최대값 사이의 값을 포함하고 있는 block에 대한 tag를 update하는 함수
	 * @param iLevel [in] \n int \n block 의 level, 0 or 1
	 * @param d2ActiveValueRangeMinMax [in] \n vxdouble2 \n 사용할 최소(x), 최대값(y) \n
	 * @param psvxLocalProgress [out] \n SVXLocalProgress \n 
	 함수가 진행되는 progress 정보를 포함하는 SVXLocalProgress의 포인터 \n
	 기본값은 NULL이며, NULL이면 사용 안 함.
	 * @remarks 
	 * 기존에 생성되어 있는 최소, 최대값 block이 class에 등록되어 있어야 함 \n
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void UpdateTaggedActivatedBlocks(int iLevel, vxdouble2 d2ActiveValueRangeMinMax, SVXLocalProgress* psvxLocalProgress = NULL);

	// Static Helper Functions for Tagged Binary Access //
	/*!
	 * @brief tagged binary volume으로 정의된 array에서 tagged binary 값을 얻는 static helper 함수
	 * @param ppiTaggedBinaryVolume [in] \n int \n tagged binary volume을 저장하고 있는 2D array
	 * @param i3SamplePos [in] \n vxint3 \n volume의 OS 에서의 position
	 * @param iSampleXOffet [in] \n iSampleXOffet \n 
	 * x축 방향에 대한 extra boundary가 포함된 read offset 크기 \n
	 * i3VolumeSize.x + i3SizeExtraBoundary.x * 2
	 * @remarks tagged binary volume은 int type으로 bit encoding되어 있음
	 */
	inline static bool GetBitBinaryTag(int** ppiTaggedBinaryVolume, vxint3 i3SamplePos/*include extra boundary*/, int iSampleXOffet/*include extra boundary*/);
	/*!
	 * @brief tagged binary volume에서 하나의 z-slice에 대해 OS의 xy position을 encoding된 array의 iQuet와 iRem을 구하는 static helper 함수
	 * @param i2SamplePosXY [in] \n vxint2 \n extra boundary가 포함된 volume의 sample index 좌표
	 * @param iSampleXOffet [in] \n iSampleXOffet \n 
	 * x축 방향에 대한 extra boundary가 포함된 read offset 크기 \n
	 * i3VolumeSize.x + i3SizeExtraBoundary.x * 2
	 * @remarks tagged binary volume은 int type으로 bit encoding되어 있음
	 */
	inline static vxint2 GetBitBinaryIndex(vxint2 i2SamplePosXY/*include extra boundary*/, int iSampleXOffet/*include extra boundary*/);
};

struct SVXVirtualArchiveTObject;
/**
 * @class CVXTObject
 * @brief VXObject를 상속 받으며 volume의 visibility 속성을 결정하는 OTF 관련 정보에 대한 class
 * @sa vxobjects::CVXObject, vxobjects::CVXVObjectVolume
 */
__vxstaticclass CVXTObject : public CVXObject
{
private:
protected:
	SVXVirtualArchiveTObject* m_pVATO;

public:
	CVXTObject();
	~CVXTObject();
	
	/*!
	 * @brief OTF 정보를 갖고 있는 @ref vxobjects::SVXTransferFunctionArchive 자료 구조를 VXTObject에 등록함
	 * @param psvxTransferFunctionArchive [in] \n SVXTransferFunctionArchive \n OTF 정보가 정의된 SVXTransferFunctionArchive 의 포인터
	 * @param iReferenceObjectID [in] \n int \n 
	 * 일반적으로 하나의 VXTObject는 하나의 VXVObjectVolume에 특화되어 생성되며, VXVObjectVolume의 ID \n
	 * VXVObjectVolume와 data type 및 저장된 volume 의 값의 범위가 같을 경우 혼용해도 문제가 없음. \n
	 * VXVObjectVolume에 의존하지 않고 독자적으로 구성될 경우 0을 넣음.
	 */
	bool RegisterTransferFunctionArchive(SVXTransferFunctionArchive* psvxTransferFunctionArchive, int iReferenceObjectID);
	/*!
	 * @brief VXTObject에 정의되어 있는 OTF 정보를 얻음.
	 * @return SVXVolumeDataArchive \n OTF 정보가 저장되어 있는 SVXTransferFunctionArchive의 포인터
	 */
	SVXTransferFunctionArchive* GetTransferFunctionArchive();
	
	// Static Helper Functions //
	/*!
	 * @brief SVXVolumeDataArchive 에 저장되는 OTF array를 할당하는 static helper 함수
	 * @param iNumDims [in] \n int \n OTF dimension
	 * @param i3DimSize [in] \n vxint3 \n OTF dimension 의 크기
	 * @param eValueDataType [in] \n EnumVXRDataType \n OTF array의 data type
	 * @param pppvArchiveTF [in] \n void \n 2D OTF array에 대한 void**의 포인터 (3D 포인터)
	 * @remarks OTF array 는 항상 2D OTF로 저장됨
	 * @sa vxobjects::SVXTransferFunctionArchive
	 */
	static void CreateArchive(int iNumDims, vxint3 i3DimSize, EnumVXRDataType eValueDataType, void*** pppvArchiveTF/*out*/);
};

/**
 * @class CVXCObject
 * @brief 카메라 관련 정보를 다루는 VXIObject에 하나의 instance로 포함된 class
 * @remarks 
 * 본 class에서 사용되는 space는 다음과 같다.
 * @li WS (World Space) : 카메라 및 객체가 배치되는 real world.
 * @li CS (Camera Space or Viewing Space) : 카메라 기준의 space, WS와 unit이 같음 \n
 * 원점 : 카메라 위치, y축 : up vector, -z축 : viewing direction
 * @li PS (Projection Space) : CS에서 정의된 view frustum의 내부를 기준으로 normalized cube형태의 frustum으로 정의된 space \n
 * 원점 : near plane과 viewing direction이 만나는 점.\n
 * y 및 z 축은 CS의 방향과 같으나, 길이는 view frustum으로 정의되는 길이가 1로 normalized 되게끔 scaling 됨.
 * @li SS (Screen Space or Window Space) : buffer pixel에 대응하는 space \n
 * 원점 : PS 상의 normalized view frustum 기준 z = 0인 plane의 좌측상단 \n
 * x축 : PS 상의 x축 방향과 동일, y축 : PS 상의 -y축 방향, z축 : PS 상의 -z축 방향 \n
 * xy scaling : screen or window를 정의하는 buffer의 해상도 크기 \n
 * z scaling : 1 (즉 PS의 z값에 부호만 반대)
 * @remarks image plane은 near plane 상에 정의됨.
 * @sa vxobjects::CVXIObject
 */
struct SVXVirtualArchiveCObject;
__vxstaticclass CVXCObject
{
private:
protected:
	// Default Interest Coordinate System is World Space
	// Camera Position : f3PosOriginInGlobalSpace
	// Viewing Direction : -f3VecAxisZInGlobalSpace
	// Up Vector : f3VecAxisYInGlobalSpace
	// Left Vector : f3VecAxisXInGlobalSpace
	SVXVirtualArchiveCObject* m_pVACO;

	void ComputeMatrix();

public:
	/*!
	 * @brief constructor, 카메라를 초기화하기 위한 parameter 필요
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS에 axis-aligned 된 cube인 scene stage가 정의된 SVXOrthoBox 의 포인터
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n 주어진 scene stage 상의 어느 위치를 기준으로 camera를 설정하는가를 결정
	 * @param pi2SizeScreen [in](optional) \n vxint2 \n 
	 * SS를 정의하기 위한 screen 의 해상도가 정의된 vxint2 의 포인터 \n
	 * 기본값은 NULL이며, 이 경우 해상도는 width = 1, height = 1로 설정됨
	 */
	CVXCObject(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType, const vxint2* pi2SizeScreen = NULL);
	~CVXCObject();
	
	/*!
	 * @brief 카메라를 scene stage (or view stage) 기준으로 설정하는 함수
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS에 axis-aligned 된 cube인 scene stage가 정의된 SVXOrthoBox 의 포인터
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n 주어진 scene stage 상의 어느 위치를 기준으로 camera를 설정하는가를 결정
	 * @remarks WS, CS, PS 간 변환 matrix가 다시 계산됨.
	 */
 	void SetViewStage(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType);
 
	/*!
	 * @brief CS 에서 PS 로 변환 (projection) 되는 상태를 설정
	 * @param bPerspective [in] \n bool \n true 면 perspective projection이 되며, false 면 orthogonal projection이 됨.
	 * @param pfFov [in] \n float \n 
	 * perspective projection 시 up vector 방향 기준 field of view의 angle (radian) 이 정의된 float 포인터 \n
	 * NULL이면 기존에 설정된 field of view 값을 사용.
	 * @remarks field of view 의 초기값은 PI/4 로 설정되어 있음.
	 */
 	void SetPerspectiveViewingState(bool bPerspective, float* pfFov = NULL); 
	/*!
	 * @brief perspective projection 의 상태를 얻는 함수
	 * @param pfFov [out](optional) \n float \n 현재 설정된 field of view (radian) 값이 저장될 float의 포인터
	 * @return bool \n perspective projection 이면 true 반환, orthogonal projection 이면 false 반환
	 * perspective projection 시 up vector 방향 기준 field of view의 angle (radian) 이 정의된 float 포인터 \n
	 * @remarks field of view 의 초기값은 PI/4 로 설정되어 있음.
	 */
 	bool GetPerspectiveViewingState(float* pfFov/*out*/);
 
 	// Image Plane is defined as Near Plane from Camera
	/*!
	 * @brief viewing state를 통한 CS, PS, SS 의 변환 matrix를 재설정하는 함수
	 * @param pf2ImagePlaneSize [in] \n vxfloat2 \n WS(or CS) 상의 near plane의 width(x), height(y)가 저장된 vxfloat2 의 포인터
	 * @param pfAspectWdivByH [in] \n float \n WS(or CS) 상의 up vector 방향 field of view가 정의된 float 의 포인터
	 * @param pfNearPlaneDistFromCamera [in] \n float \n WS(or CS) 상의 camera에서 near plane 사이의 거리가 정의된 float 의 포인터
	 * @param pfFarPlaneDistFromCamera [in] \n float \n WS(or CS) 상의 camera에서 far plane 사이의 거리가 정의된 float 의 포인터
	 * @param pi2ScreenSize [in] \n vxint2 \n SS 상의 screen 크기(width(x), height(y))가 정의된 vxint2 의 포인터. 
	 * @param pf2FittingSize [in] \n vxfloat2 \n Orthogonal Projection에서 Camera 가로 세로 ratio 를 유지시키기 위한 Fitting 정보가 저장된 vxfloat2의 포인터
	 * @param pfFittingFovY [in] \n float \n Perspective Projection에서 Camera 가로 세로 ratio 를 유지시키기 위한 Fitting 정보가 저장된 vxfloat2의 포인터
	 * @remarks WS to CS, CS to PS, PS to SS 에 대한 변환 matrix 및 각각에 대한 inverse matrix가 다시 계산됨.
	 */
 	void SetDefaultViewingState(vxfloat2* pf2ImagePlaneSize, float* pfAspectWdivByH, float* pfNearPlaneDistFromCamera, float* pfFarPlaneDistFromCamera, vxint2* pi2ScreenSize, vxfloat2* pf2FittingSize = NULL, float* pfFittingFovY = NULL);
	/*!
	 * @brief 현재 CS, PS, SS 의 변환 matrix를 정의하는 viewing state를 얻는 함수
	 * @param pf2ImagePlaneSize [out] \n vxfloat2 \n WS(or CS) 상의 near plane의 width(x), height(y)가 저장될 vxfloat2 의 포인터
	 * @param pfAspectWdivByH [out] \n float \n WS(or CS) 상의 up vector 방향 field of view가 저장될 float 의 포인터
	 * @param pfNearPlaneDistFromCamera [out] \n float \n WS(or CS) 상의 camera에서 near plane 사이의 거리가 저장될 float 의 포인터
	 * @param pfFarPlaneDistFromCamera [out] \n float \n WS(or CS) 상의 camera에서 far plane 사이의 거리가 저장될 float 의 포인터
	 * @param pi2ScreenSize [out] \n vxint2 \n SS 상의 screen 크기(width(x), height(y))가 저장될 vxint2 의 포인터
	 * @param pf2FittingSize [out] \n vxfloat2 \n Orthogonal Projection에서 Camera 가로 세로 ratio 를 유지시키기 위한 Fitting 정보가 저장될 vxfloat2의 포인터
	 * @param pfFittingFovY [out] \n float \n Perspective Projection에서 Camera 가로 세로 ratio 를 유지시키기 위한 Fitting 정보가 저장될 vxfloat2의 포인터
	 * @remarks 얻고 싶지 않은 parameter에 대해 NULL을 넣으면 해당 parameter에 값을 저장 안 함.
	 */
 	void GetDefaultViewingState(vxfloat2* pf2ImagePlaneSize/*out*/, float* pfNearPlaneDistFromCamera/*out*/, float* pfFarPlaneDistFromCamera/*out*/, vxint2* pi2ScreenSize/*out*/, vxfloat2* pf2FittingSize = NULL/*out*/, float* pfFittingFovY = NULL/*out*/);
	/*!
	 * @brief WS 상에서 정의된 카메라 state를 설정하는 함수
	 * @param pf3PosCamera [in] \n vxfloat3 \n WS 상의 카메라 위치가 정의된 vxfloat3 의 포인터
	 * @param pf3VecView [in] \n vxfloat3 \n WS 상의 viewing direction이 정의된 vxfloat3 의 포인터
	 * @param pf3VecUp [in] \n vxfloat3 \n WS 상의 up vector가 정의된 float의 포인터
	 * @remarks WS 와 CS 의 변환을 정의하는 matrix가 다시 계산됨.
	 */
 	void SetCameraState(vxfloat3* pf3PosCamera, vxfloat3* pf3VecView, vxfloat3* pf3VecUp);
	/*!
	 * @brief WS 상의 카메라 정보를 얻는 함수
	 * @param pf3PosCamera [in] \n vxfloat3 \n WS 상의 카메라 위치가 자장될 vxfloat3 의 포인터
	 * @param pf3VecView [in] \n vxfloat3 \n WS 상의 viewing direction이 자장될 vxfloat3 의 포인터
	 * @param pf3VecUp [in] \n vxfloat3 \n WS 상의 up vector가 저장될 float의 포인터
	 */
 	void GetCameraState(vxfloat3* pf3PosCamera/*out*/, vxfloat3* pf3VecView/*out*/, vxfloat3* pf3VecUp/*out*/);
	/*!
	 * @brief WS 상에서 정의된 카메라 zoom 상태를 1.0 (100% 상태) 으로 정의하는 up vector 방향의 field of view 를 설정하는 함수
	 * @param fFittingFovY [in] \n float \n WS 에서 up vector 방향의 field of view
	 */
	void SetFittingFovY(float fFittingFovY);
	/*!
	 * @brief WS 상에서 정의된 카메라 zoom 상태를 1.0 (100% 상태) 으로 정의하는 up vector 방향의 field of view 를 얻는 함수
	 * @param pfFittingFovY [out] \n float \n WS 에서 up vector 방향의 field of view이 저장될 float 의 포인터
	 */
	void GetFittingFovY(float* pfFittingFovY/*out*/);
	/*!
	 * @brief perspective projection 의 여부를 얻는 함수
	 * @return bool \n perspective projection 이면 true 반환, orthogonal projection 이면 false 반환
	 */
 	bool IsPerspective();
 
	/*!
	 * @brief WS 에서 SS 로의 변환을 정의하는 matrix를 얻음
	 * @param psvxMatrixWS2CS [out] \n vxmatrix \n WS 에서 CS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @param psvxMatrixCS2PS [out] \n vxmatrix \n CS 에서 PS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @param psvxMatrixPS2SS [out] \n vxmatrix \n PS 에서 SS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @remarks 얻고 싶지 않은 parameter에 대해 NULL을 넣으면 해당 parameter에 값을 저장 안 함.
	 */
 	void GetMatricesWStoSS(vxmatrix* psvxMatrixWS2CS/*out*/, vxmatrix* psvxMatrixCS2PS/*out*/, vxmatrix* psvxMatrixPS2SS/*out*/);
	/*!
	 * @brief SS 에서 WS 로의 변환을 정의하는 matrix를 얻음
	 * @param psvxMatrixSS2PS [out] \n vxmatrix \n SS 에서 PS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @param psvxMatrixPS2CS [out] \n vxmatrix \n PS 에서 CS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @param psvxMatrixCS2WS [out] \n vxmatrix \n CS 에서 WS로 변환하는 matrix가 저장될 vxmatrix 의 포인터
	 * @remarks 얻고 싶지 않은 parameter에 대해 NULL을 넣으면 해당 parameter에 값을 저장 안 함.
	 */
	void GetMatricesSStoWS(vxmatrix* psvxMatrixSS2PS/*out*/, vxmatrix* psvxMatrixPS2CS/*out*/, vxmatrix* psvxMatrixCS2WS/*out*/);
	
	/*!
	 * @brief WS or CS or PS or SS 에서 정의되는 image plane의 rectangle을 정의하는 네 점을 얻음
	 * @param eCoordSapce [in] \n EnumVXVCoordSpace \n image plane을 얻고자 하는 space
	 * @param pf3PosTopLeft [out] \n vxfloat3 \n eCoordSapce 에서 정의된 image plane의 rectangle의 좌측상단 점이 저장될 vxfloat3 의 포인터
	 * @param pf3PosTopRight [out] \n vxfloat3 \n eCoordSapce 에서 정의된 image plane의 rectangle의 우측상단 점이 저장될 vxfloat3 의 포인터
	 * @param pf3PosBottomLeft [out] \n vxfloat3 \n eCoordSapce 에서 정의된 image plane의 rectangle의 좌측하단 점이 저장될 vxfloat3 의 포인터
	 * @param pf3PosBottomRight [out] \n vxfloat3 \n eCoordSapce 에서 정의된 image plane의 rectangle의 우측하단 점이 저장될 vxfloat3 의 포인터
	 * @remarks 
	 * 얻고 싶지 않은 parameter에 대해 NULL을 넣으면 해당 parameter에 값을 저장 안 함. \n
	 * OS 에 대해선 동작하지 않음.
	 */
	void GetImagePlaneRectPoints(EnumVXVCoordSpace eCoordSapce, vxfloat3* pf3PosTopLeft/*out*/, vxfloat3* pf3PosTopRight/*out*/, vxfloat3* pf3PosBottomLeft/*out*/, vxfloat3* pf3PosBottomRight/*out*/);
};

struct SVXVirtualArchiveIObject;
/**
 * @class CVXIObject
 * @brief image plane의 buffer 를 다루고 하나의 VXCObject를 갖는 VXObject를 상속 받는 class
 * @remarks 
 * 하나의 해상도 (width, height)에 대해 다양한 용도에 사용될 여러개의 image buffer (frame buffer)가 정의됨. \n
 * image plane과 연결되는 하나의 카메라 정보를 갖는 CVXCObject 의 instance를 갖음.
 * @sa vxobjects::CVXObject, vxobjects::CVXCObject
 */
__vxstaticclass CVXIObject : public CVXObject
{
private:
protected:
	SVXVirtualArchiveIObject* m_pVAIO;

public:
	/*!
	 * @brief constructor, image plane 에 mapping되는 frame buffer를 정의하는 해상도 필요
	 * @param iWidthPix [in](optional) \n int \n 해상도 width (pixel), 기본값은 0
	 * @param iHeightPix [in](optional) \n int \n 해상도 height (pixel), 기본값은 0
	 * @remarks width 또는 height 가 0 이하면 frame buffer를 생성하는 함수 (@ref CVXIObject::ResizeFrameBuffer, @ref  CVXIObject::InsertFrameBuffer)가 실패
	 */
	CVXIObject(int iWidthPix = 0, int iHeightPix = 0);
	~CVXIObject();
	
	/*!
	 * @brief 정의되어 있는 frame buffer의 크기를 재설정하는 함수
	 * @param iWidthPix [in] \n int \n 해상도 width (pixel), 1 이상
	 * @param iHeightPix [in] \n int \n 해상도 height (pixel), 1 이상
	 * @remarks 
	 * 기존에 정의된 frame buffer를 memory에서 해제  후 주어진 크기로 다시 할당\n
	 * frame buffer에 저장되었던 내용물도 모두 삭제됨 (다시 채우는 module or function이 호출되야 함)\n
	 * image plane의 pixel에 대한 x pitch와 y pitch가 같다고 가정하므로 width와 height에 대한 ratio가 바뀜\n
	 * 따라서, CVXCObject 에서 정의하는 WS 상의 image plane의 정보가 재설정되고, 관련된 변환 matrix가 재설정됨.
	 */
	void ResizeFrameBuffer(int iWidthPix, int iHeightPix);
	/*!
	 * @brief 정의되어 있는 frame buffer의 정보를 얻는 함수
	 * @param pi2SizeScreen [out] \n vxint2 \n frame buffer의 해상도가 저장될 vxint2 의 포인터. width(x), height(y)
	 * @param piNumFrameBuffers [out](optional) \n int \n 현재 정의된 frame buffer의 개수가 저장될 int 포인터
	 * @param piBytesSizePerPixel [out](optional) \n int \n 현재 정의된 모든 frame buffer에 대하여 하나의 pixel을 정의하는 type들의 크기합이 저장될 int의 포인터
	 * @remarks 얻고 싶지 않은 parameter에 대해 NULL을 넣으면 해당 parameter에 값을 저장 안 함.
	 */
	void GetFrameBufferInfo(vxint2* pi2SizeScreen/*out*/, int* piNumFrameBuffers = NULL/*out*/, int* piBytesSizePerPixel = NULL/*out*/);
	/*!
	 * @brief 정의되어 있는 frame buffer의 정보가 저장된(array 포함) @ref vxobjects::SVXFrameBuffer 를 얻는 함수
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n 정의된 frame buffer들 중 해당 usage의 buffer를 얻음
	 * @param uiBufferIndex [in] \n uint \n 해당 usage의 frame buffer 중 index 번째로 정의되어 있는 buffer를 얻음
	 * @return SVXFrameBuffer \n frame buffer의 정보가 저장된(array 포함) @ref vxobjects::SVXFrameBuffer 의 포인터
	 */
	SVXFrameBuffer* GetFrameBuffer(EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex);
	
	/*!
	 * @brief frame buffer를 하나 추가하는 함수
	 * @param ePixelDataType [in] \n EnumVXRDataType \n 추가할 frame buffer의 data type
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n 추가할 frame buffer의 usage
	 * @param strMent [in] \n wstring \n 추가할 frame buffer에 대한 mention
	 * @param uiSizeBytesPerPixel [in](optional) \n uint \n ePixelDataType == enums::vxrDataTypeSTRUCTURED 일 경우에만 쓰이며, 이 때의 단위 pixel의 크기(bytes)
	 * @remarks eFrameBufferUsage == vxenums::vxrFrameBufferUsageRENDEROUT 일 경우 data type은 vxenums::vxrDataTypeBYTE4CHANNEL 이어야 함.
	 */
	void InsertFrameBuffer(EnumVXRDataType ePixelDataType, EnumVXRFrameBufferUsage eFrameBufferUsage, wstring strMent, uint uiSizeBytesPerPixel = 0/*Only Used when vxrDataTypeSTRUCTURED as EnumVXRDataType*/);

	/*!
	 * @brief frame buffer를 변경하는 함수
	 * @param ePixelDataType [in] \n EnumVXRDataType \n 변경할 frame buffer의 data type
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n 변경할 frame buffer의 usage
	 * @param uiBufferIndex [in] \n uint \n 변경할 frame buffer의 index
	 * @param strMent [in] \n wstring \n 추가할 frame buffer에 대한 mention
	 * @param uiSizeBytesPerPixel [in](optional) \n uint \n ePixelDataType == enums::vxrDataTypeSTRUCTURED 일 경우에만 쓰이며, 이 때의 단위 pixel의 크기(bytes)
	 * @return bool \n 해당 uiBufferIndex 의 버퍼가 있으면 true, 없으면 false 반환
	 * @remarks 해당 index 의 버퍼가 ePixelDataType 로 이미 선언되어 있으면 아무 작업 없이 true 반환
	 */
	bool ReplaceFrameBuffer(EnumVXRDataType ePixelDataType, EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex, wstring strMent, uint uiSizeBytesPerPixel = 0/*Only Used when vxrDataTypeSTRUCTURED as EnumVXRDataType*/);

	/*!
	 * @brief frame buffer를 삭제하는 함수
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n 삭제할 frame buffer의 usage
	 * @param uiBufferIndex [in] \n uint \n 해당 usage (eFrameBufferUsage)의 frame buffer 중 index 번째의 buffer를 삭제
	 * @return 해당 frame buffer가 존재하며 삭제에 성공하면 true, 그렇지 않으면 false 반환
	 * @remarks 해당 frame buffer(eFrameBufferUsage && uiBufferIndex)는 memory에서 해제됨.
	 */
	bool DeleteFrameBuffer(EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex);

	/*!
	 * @brief CVXCObject의 instance를 생성.
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS에 axis-aligned 된 cube인 scene stage가 정의된 SVXOrthoBox 의 포인터
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n 주어진 scene stage 상의 어느 위치를 기준으로 camera를 설정하는가를 결정
	 * @remarks 
	 * 이미 생성되어 있으면 동작하지 않음. \n
	 * instance 생성 전 CVXIObject 의 content 가 정의되어 있어야(@ref vxobjects::CVXObject::IsDefinedResource) 함. (즉 buffer가 생성되어 있어야 함)
	 * @sa vxobjects::CVXObject, vxobjects::CVXCObject
	 */
	void AttachCamera(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType);
	/*!
	 * @brief CVXCObject의 instance를 얻는 함수
	 * @return CVXCObject \n instance로 정의되어 있는 CVXCObject 의 포인터
	 */
	CVXCObject* GetCameraObject();

	/*!
	 * @brief vxenums::vxrFrameBufferUsageRENDEROUT 로 정의된 SVXFrameBuffer 들이 저장된 vector container를 얻음
	 * @return 정의된 SVXFrameBuffer 들이 저장된 vector container에 대한 vector<SVXFrameBuffer> 의 포인터
	 * @remarks 정의된 frame buffer 의 data type 은 vxenums::vxrDataTypeBYTE4CHANNEL 임
	 */
	vector<SVXFrameBuffer>* GetRenderBufferPointerList();
	/*!
	 * @brief vxenums::vxrFrameBufferUsageDEPTH 로 정의된 SVXFrameBuffer 들이 저장된 vector container를 얻음
	 * @return 정의된 SVXFrameBuffer 들이 저장된 vector container에 대한 vector<SVXFrameBuffer> 의 포인터
	 * @remarks 정의된 frame buffer 의 data type 은 vxenums::vxrDataTypeFLOAT 임
	 */
	vector<SVXFrameBuffer>* GetDepthBufferPointerList();
	/*!
	 * @brief vxenums::vxrFrameBufferUsageCUSTOM 로 정의된 SVXFrameBuffer 들이 저장된 vector container를 얻음
	 * @return 정의된 SVXFrameBuffer 들이 저장된 vector container에 대한 vector<SVXFrameBuffer> 의 포인터
	 * @remarks 정의된 frame buffer 의 data type 에 대한 특별한 제한은 없음
	 */
	vector<SVXFrameBuffer>* GetCustomBufferPointerList();
};

struct SVXVirtualArchiveVObjectPrimitive;
/**
 * @class CVXVObjectPrimitive
 * @brief VXVObject를 통해 OS와 WS의 배치 관계가 설정된 Volume의 정보를 갖는 class.
 * @remarks 
 * 기존의 OS를 OS 와 VOS 의 두개로 나누어 처리. \n
 * @li OS : SVXPrimitiveDataArchive 저장되는 primitive 좌표계
 * @li VOS : SVXPrimitiveDataArchive 로 정의되는 개별 객체가 WS 좌표계 이전에 배치되는 좌표계 \n
 * CVXVObject 의 OS 가 VOS 가 되며, OS/VOS/WS 간 변환은 사용자 정의에 따름 \n
 * @remarks OS 가 VOS 로의 객체 변환을 통해 SVXPrimitiveDataArchive 로 정의되는 개별 객체를 WS 상에 여러 형태(affine transform)로 배치 및 변형 할 수 있음.
 * @sa vxobjects::CVXVObject, vxobjects::CVXCObject
 */
__vxstaticclass CVXVObjectPrimitive : public CVXVObject
{
private:
protected:
	SVXVirtualArchiveVObjectPrimitive* m_pVAVOPrim;

public:
	CVXVObjectPrimitive();
	~CVXVObjectPrimitive();
	
	/*!
	 * @brief primitive로 정의된 객체 정보를 갖고 있는 @ref vxobjects::SVXPrimitiveDataArchive 자료 구조를 VXVObjectPrimitive에 등록함
	 * @param psvxPrimitiveDataArchive [in] \n SVXPrimitiveDataArchive \n primitive로 정의된 객체 정보가 저장된 SVXVolumeDataArchive 의 포인터
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 * 함수가 진행되는 progress 정보를 포함하는 SVXLocalProgress의 포인터 \n
	 * 기본값은 NULL이며, NULL이면 사용 안 함.
	 */
	bool RegisterPrimitiveArchive(SVXPrimitiveDataArchive* psvxPrimitiveDataArchive, SVXLocalProgress* psvxProgress = NULL);
	/*!
	 * @brief VXVObjectPrimitive에 정의되어 있는 primitive로 정의된 객체 정보를 얻음.
	 * @return SVXPrimitiveDataArchive \n primitive로 정의된 객체 정보가 저장되어 있는 SVXPrimitiveDataArchive 의 포인터
	 */
	SVXPrimitiveDataArchive* GetPrimitiveArchive();
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치되는 개수를 얻음
	 * @return int \n SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치된 개수
	 */
	int GetNumArrangementComponents();
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치되는 상태를 얻음.
	 * @param iIndex [in] \n int \n 개별 객체가 VOS 상에 배치되는 변환들에 대한 index
	 * @param psvxMatrix [out] \n vxmatrix \n 개별 객체가 VOS 상에 배치되는 변환을 저장할 vxmatrix의 포인터, NULL 이면 사용 안 함
	 * @param pf4Color [out] \n vxfloat4 \n 개별 객체가 VOS 상에 배치될 때 부여될 normalized color RGBA 색상을 저장할 vxfloat4의 포인터, NULL 이면 사용 안 함
	 * @param pbIsVisible [out] \n bool \n 개별 객체가 VOS 상에 배치될 때 부여될 visibility를 저장할 bool 포인터, true면 visible, false면 hidden, NULL 이면 사용 안 함
	 * @param pbIsWireFame [out] \n bool \n 개별 객체가 wireframe 으로 그려질지에 대한 bool 포인터, true면 wire, false면 solid, NULL 이면 사용 안 함
	 * @return bool \n 해당 index의 변환이 존재하면 true, 그렇지 않으면 false 반환
	 * @remarks ,
	 * 변환은 RHS, row major matrix로 정의됨.\n
	 * 개별 객체가 VOS 상에 배치하는 정보는 내부에 list로 정의되며 zero count-based index로 구분됨.\n
	 * ex.[0][1][2] -> 하나 추가 -> [0][1][2][3] -> [1] 삭제 -> [0][2->1][3->2] => [0][1][2]
	 */
	bool GetPrimitiveArrangementComponent(int iIndex, vxmatrix* psvxMatrix/*out*/, vxfloat4* pf4Color/*out*/, bool* pbIsVisible/*out*/, bool* pbIsWireFame/*out*/);
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치하는 정보(변환 matrix 및 색상)를 추가
	 * @param psvxMatrix [in] \n vxmatrix \n SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치하는 변환이 저장된 vxmatrix의 포인터
	 * @param pf4Color [in] \n vxfloat4 \n SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치될 때 부여할 normalized color RGBA 색상을 저장할 vxfloat4의 포인터
	 * @return bool \n 정상적으로 추가되면 true, 그렇지 않으면 false 반환
	 * @remarks 
	 * 변환은 RHS, row major matrix로 정의됨.\n
	 * 개별 객체가 VOS 상에 배치하는 정보는 내부에 list로 정의되며 zero count-based index로 구분됨.\n
	 * ex.[0][1][2] -> 하나 추가 -> [0][1][2][3] -> [1] 삭제 -> [0][2->1][3->2] => [0][1][2]
	 */
	bool InsertPrimitiveArrangement(const vxmatrix* psvxMatrix, const vxfloat4* pf4Color);
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치되는 상태를 수정.
	 * @param iIndex [in] \n int \n 개별 객체가 VOS 상에 배치되는 변환들에 대한 index
	 * @param psvxMatrix [in] \n vxmatrix \n 개별 객체가 VOS 상에 배치되는 변환을 저장한 vxmatrix의 포인터, NULL 이면 기존 것이 유지
	 * @param pf4Color [in] \n vxfloat4 \n 개별 객체가 VOS 상에 배치될 때 부여될 normalized color RGBA 색상을 저장한 vxfloat4의 포인터, NULL 이면 기존 것이 유지
	 * @param bIsVisible [in] \n bool \n 개별 객체가 VOS 상에 배치될 때 부여될 visibility를 결정, true면 visible, false면 hidden
	 * @param bIsWireFrame [in] \n bool \n 개별 객체가 wireframe 으로 그려질지를 결정, true면 wire, false면 solid
	 * @return bool \n 해당 index의 변환이 존재하면 true, 그렇지 않으면 false 반환
	 * @remarks 
	 * 변환은 RHS, row major matrix로 정의됨.\n
	 */
	bool ModifyPrimitiveArrangement(int iIndex, const vxmatrix* psvxMatrix, const vxfloat4* pf4Color, bool bIsVisible, bool bIsWireFrame);
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 VOS 상에 배치되는 상태를 삭제.
	 * @param iIndex [in] \n int \n 개별 객체가 VOS 상에 배치되는 변환들에 대한 index
	 * @return bool \n 해당 index의 변환을 삭제 성공하면 true, 그렇지 않으면 false 반환
	 * @remarks 
	 * 변환은 RHS, row major matrix로 정의됨.\n
	 * 개별 객체가 VOS 상에 배치하는 정보는 내부에 list로 정의되며 zero count-based index로 구분됨.\n
	 * ex.[0][1][2] -> 하나 추가 -> [0][1][2][3] -> [1] 삭제 -> [0][2->1][3->2] => [0][1][2]
	 */
	bool RemovePrimitiveArrangement(int iIndex);
	/*!
	 * @brief SVXPrimitiveDataArchive 로 정의된 개별 객체가 WS에 배치(OS->VOS->WS)될 때의 pitch 정보를 얻음
	 * @param pf3PitchOS2WS [out] \n vxfloat3 \n 개별 객체가 WS에 배치(OS->VOS->WS)될 때의 pitch 정보가 저장될 vxfloat3 의 포인터
	 * @param iIndex [in] \n int \n 개별 객체가 VOS 상에 배치되는 변환들에 대한 index
	 * @return bool \n pitch 정보를 성공적으로 얻으면 true, 그렇지 않으면 false 반환
	 */
	bool GetPrimitiveArrangementComponentPitch(vxfloat3* pf3PitchOS2WS/*out*/, int iIndex);

	// Helper
	/*!
	 * @brief scale 후 translation을 수행하는 변환 matrix를 생성하는 static helper 함수
	 * @param psvxMatrix [out] \n vxmatrix \n 계산된 변환 matrix를 저장할 vxmatrix의 포인터
	 * @param f3Scale1st [in] \n vxfloat3 \n scale factor
	 * @param f3Translation2nd [in] \n vxfloat3 \n translation factor
	 * @remarks OS 기준으로 대칭인 개별객체를 VOS 로 배치하는 matrix를 얻을 때 사용
	 */
	static void ComputeSymmetricPrimitiveArrangementMatrix(vxmatrix* psvxMatrix/*out*/, vxfloat3 f3Scale1st, vxfloat3 f3Translation2nd);
	/*!
	 * @brief source line 을 destination line 으로 변환하는 matrix를 생성하는 static helper 함수
	 * @param psvxMatrix [out] \n vxmatrix \n 계산된 변환 matrix를 저장할 vxmatrix의 포인터
	 * @param f3PosSrc[2] [in] \n vxfloat3 \n source line을 정의하는 크기 2인 static array, f3PosSrc[0] : 시작점, f3PosSrc[1] : 끝
	 * @param f3PosDest[2] [in] \n vxfloat3 \n destination line을 정의하는 크기 2인 static array, f3PosDest[0] : 시작점, f3PosDest[1] : 끝
	 * @par ex. (0, 0, 0), (0, 0, 1) 로 정의하고 있는 line 을 (10, 10, 10), (20, 20, 20) 으로 정의하는 line으로 변환하는 matrix
	 * >> vxfloat3 f3PosSrc[2] = { vxfloat3(0, 0, 0), vxfloat3(0, 0, 1) }; \n
	 * >> vxfloat3 f3PosDest[2] = { vxfloat3(10, 10, 10),  vxfloat3(20, 20, 20) }; \n
	 * >> vxmatrix mat; \n
	 * >> CVXVObjectPrimitive::ComputeLinePrimitiveArrangementMatrix(&mat, f3PosSrc, f3PosDest); \n
	 * >> // use mat
	 */
	static void ComputeLinePrimitiveArrangementMatrix(vxmatrix* psvxMatrix/*out*/, vxfloat3 f3PosSrc[2], vxfloat3 f3PosDest[2]);
};

struct SVXVirtualArchiveCustomList;
/**
 * @class CVXLObject
 * @brief CVXObject를 상속 받는 customized list 정보를 갖는 class.
 * @remarks 
 * VXFramework에서 정의하는 VXObject 자료 구조외의 값을 resource manager가 관리하는 자료구조로 사용하기 위해 쓰임 \n
 * 내부적으로 (key, value)의 map container로 구현되어 있으며 key의 형식은 "_vlist_[type string]_[Name]" 으로 정의 \n
 * value는 vector<data type>의 vector container로 구현되어 있음.
 * @sa vxobjects::CVXObject, vxhelpers::VXHGetDataTypeFromString
 */
__vxstaticclass CVXLObject: public CVXObject
{
private:
protected:
	SVXVirtualArchiveCustomList* m_pVALOArchive;

public:
	CVXLObject();
	~CVXLObject();
	
	/*!
	 * @brief 등록되어 있는 모든 list를 모두 해제
	 * @remarks 
	 */
	void ClearAll();
	/*!
	 * @brief 해당 문자열의 key name을 갖는 list만 해제
	 * @param strListName [in] \n wstring \n key name의 문자열
	 * @return bool \n 해당 문자열의 key name을 갖는 list를 해제 성공하면 true, 그렇지 않으면 false 반환.
	 */
	bool ClearList(wstring strListName);

	/*!
	 * @brief 해당 문자열의 key name을 갖는 list를 얻음
	 * @param strListName [in] \n wstring \n 
	 * key name의 문자열, key의 형식은 "_vlist_[type string or VOIDPTR]_[Name]" \n
	 * [type string]은 VXFramework에서 정의한 문자열(@ref vxhelpers::VXHGetDataTypeFromString)을 따름. \n
	 * ex. "_vlist_FLOAT3_customname", "_vlist_VOIDPTR_customname", ...
	 * @param ppvtrList [in] \n void* \n 해당 key에 대응하는 list를 저장하는 vector<data type>의 포인터 주소를 받을 void의 이중 포인터
	 * @return bool \n 해당 문자열의 key name을 갖는 list를 얻는데 성공하면 true, 그렇지 않으면 false 반환.
	 * @remarks 
	 * "_vlist_[type string]_[Name]" 형식의 지정한 type string 외의 user defined 문자열을 key 로 쓸 수 있으며 pointer 역시 user defined pointer를 사용할 수도 있음.\n
	 * 이 경우, user defined pointer 에 resource 가 할당되어 있을 때, 이를 관리하지는 않음.
	 * user defined pointer 를 resource 로 관리하고자 할 때 type string 를 VOIDPTR 로 해야 하며, 이 경우 내부적으로 resource 관리
	 * @sa vxhelpers::VXHGetDataTypeFromString
	 */
	bool GetList(wstring strListName, void** ppvtrList/*out*/);
	/*!
	 * @brief 해당 문자열의 key name을 갖는 list를 등록함
	 * @param strListName [in] \n wstring \n 
	 * key name의 문자열, key의 형식은 "_vlist_[type string or VOIDPTR]_[Name]" \n
	 * [type string]은 VXFramework에서 정의한 문자열(@ref vxhelpers::VXHGetDataTypeFromString)을 따름. \n
	 * ex. "_vlist_FLOAT3_customname", "_vlist_VOIDPTR_customname", ...
	 * @param pvtrList [in] \n void \n 
	 * 저장할 list에 대한 void 포인터 \n
	 * 일반적으로 vector<data type> 로 list가 저장되어 있고 이것의 포인터를 parameter로 사용. \n
	 * user defined pointer를 사용할 수도 있음.
	 * @return bool \n 해당 문자열의 key name을 갖는 list를 얻는데 성공하면 true, 그렇지 않으면 false 반환.
	 * @remarks 
	 * "_vlist_[type string]_[Name]" 형식의 지정한 type string 외의 user defined 문자열을 key 로 쓸 수 있으며 pointer 역시 user defined pointer를 사용할 수도 있음.\n
	 * 이 경우, user defined pointer 에 resource 가 할당되어 있을 때, 이를 관리하지는 않음.
	 * user defined pointer 를 resource 로 관리하고자 할 때 type string 를 VOIDPTR 로 해야 하며, 이 경우 내부적으로 resource 관리
	 * @sa vxhelpers::VXHGetDataTypeFromString
	 */
	bool RegisterList(wstring strListName, void* pvtrList);
	
	/*!
	 * @brief RegisterList 에서 VOIDPTR 타입으로 등록된 자료구조에 대해 특별한 resource 관리가 delete function 이 필요할 경우 \n
	 * 이와 관련된 delegation function 및 class 를 저장하며 지정된 key name 의 자료구조가 삭제될 때 호출됨
	 * @param strListName [in] \n wstring \n 
	 * key name의 문자열, key의 형식은 "_vlist_VOIDPTR_[Name]" \n
	 * @param pvDelegate [in] \n void \n delegation function pointer
	 * @param pvFunctionClass [in] \n void \n delegation function 을 저장하고 있는 class instance pointer \n
	 * 해당 class 는 function 외의 멤버 resource 를 갖고 있지 않도록 하여, void* delete 가 되도록 함
	 * @return bool \n 해당 문자열의 key name을 갖는 list를 얻는데 성공하면 true, 그렇지 않으면 false 반환.
	 * @remarks 
	 * 일반적으로 static function 을 사용하나, static memory 해제 타이밍이 CVXLObject 삭제 타이밍보다 빠를 경우 발생하는 문제를 해결하기 위함.
	 * @sa CVXLObject::RegisterList
	 */
	bool RegisterDeleteDelegater(wstring strCallbackListName, void*pvDelegate);
	  
	/*!
	 * @brief LObject 가 갖고 있는 모든 List element 의 합
	 * @return ullong \n LObject 가 갖고 있는 모든 List element 의 합을 byte 로 반환
	 */
	ullong GetSizeOfAllLists();
};
}
using namespace vxobjects;

/**
 * @package vxhelpers
 * @brief VXFramework에서 사용되는 helper functions을 모은 namespace
 */
namespace vxhelpers
{
//=========================
// Global Helper Functions
//=========================
/*!
 * @fn __vxstatic bool VXHAllocatorVoidPointer1D(void** ppVoidTarget, int iSize, EnumVXRDataType eValueDataType, uint iUnitSizeBytes)
 * @brief 임의의 Data Type에 대해 1D로 메모리를 할당
 * @param ppVoidTarget [out] \n void* @n 할당된 메모리에 대한 1D 포인터
 * @param iSize [in] \n int \n 할당할 메모리 크기. (bytes)
 * @param eValueDataType [in] \n EnumVXRDataType \n 할당할 메모리에 해단 Data Type
 * @param iUnitSizeBytes [in](optional) \n uint \n Default 값은 0 이며, eValueDataType == vxrDataTypeSTRUCTURED 일 경우만 임의의 Structure의 크기로 사용
 * @return bool \n 메모리 할당이 성공하면 true, 실패하면 false 반환
 * @remarks 
 * 할당된 메모리 포인터에 값을 쓰거나 읽기 위해선 Data Type 에 대한 Casting 이 필요. \n
 * vxrDataTypeSTRING 은 지원하지 않음. \n
 * eValueDataType != vxrDataTypeSTRUCTURED 일 경우 iUnitSizeBytes 파라미터는 사용되지 않음
*/
__vxstatic bool VXHAllocatorVoidPointer1D(void** ppVoidTarget, int iSize, EnumVXRDataType eValueDataType, uint iUnitSizeBytes = 0);

/*!
 * @fn __vxstatic bool vxhelpers::VXHAllocatorVoidPointer2D(void*** pppVoidTarget, int iSize1, int iSize2, EnumVXRDataType eValueDataType, uint iUnitSizeBytes)
 * @brief 임의의 Data Type에 대해 2D로 메모리를 할당
 * @param pppVoidTarget [out] \n void** @n 할당된 메모리에 대한 2D 포인터
 * @param iSize1 [in] \n int \n void** 로 할당할 메모리 크기. (bytes), y dimension
 * @param iSize2 [in] \n int \n void* 로 할당할 메모리 크기. (bytes), x dimension
 * @param eValueDataType [in] \n EnumVXRDataType \n 할당할 메모리에 해단 Data Type
 * @param iUnitSizeBytes [in](optional) \n uint \n Default 값은 0 이며, eValueDataType == vxrDataTypeSTRUCTURED 일 경우만 임의의 Structure의 크기로 사용
 * @return bool \n 메모리 할당이 성공하면 true, 실패하면 false 반환
 * @remarks 
 * 할당된 메모리 포인터에 값을 쓰거나 읽기 위해선 Data Type 에 대한 Casting 이 필요. \n
 * vxrDataTypeSTRING 은 지원하지 않음. \n
 * eValueDataType != vxrDataTypeSTRUCTURED 일 경우 iUnitSizeBytes 파라미터는 사용되지 않음 \n
 * 할당된 메모리 포인터에 대해 (x, y) 의 indexing 은 (*pppVoidTarget)[y][x]으로 으루어 짐
*/
__vxstatic bool VXHAllocatorVoidPointer2D(void*** pppVoidTarget, int iSize1, int iSize2, EnumVXRDataType eValueDataType, uint iUnitSizeBytes = 0);

/*!
 * @fn __vxstatic void vxhelpers::VXHGetSystemMemoryInfo(ullong* pullFreeMemoryBytes, ullong* pullAvailablePhysicalMemoryBytes)
 * @brief 현재 구동되고 있는 OS를 통한 System Memory 상태를 제공
 * @param pullFreeMemoryBytes [out] \n ullong \n 현재 사용 가능한 메모리 크기 (bytes)
 * @param pullAvailablePhysicalMemoryBytes [out] \n ullong \n 현재 System 에 인식되는 물리 메모리 크기 (bytes)
 * @remarks x86 또는 x64, 현재 구동 OS의 상태에 따라 실제 메모리와 다르게 잡힐 수 있음.
*/
__vxstatic void VXHGetSystemMemoryInfo(ullong* pullFreeMemoryBytes, ullong* pullAvailablePhysicalMemoryBytes);

/*!
 * @fn __vxstatic ullong vxhelpers::VXHGetCurrentTimePack()
 * @brief 현재 시간에 대한 정보를 64bit으로 인코딩하여 제공
 * @return ullong \n  0~9 bit : milli-seconds , 10~15 bit : second, 16~21 bit : minute, 22~26 bit : hour,  27~31 bit : day, 32~35 bit : month, 36~65 bit : year
 * @remarks 
*/
__vxstatic ullong VXHGetCurrentTimePack();

/*!
 * @fn __vxstatic int vxhelpers::VXHGetDataTypeSizeByte(EnumVXRDataType eValueDataType)
 * @brief EnumVXRDataType 으로 정의되는 Data Type에 대한 단위 bytes 크기를 제공
 * @param eValueDataType [in] \n EnumVXRDataType \n Target Data Type
 * @return int \n Data Type의 크기 (bytes)
 * @remarks vxrDataTypeUNDEFINED, vxrDataTypeSTRUCTURED, vxrDataTypeSTRING 의 경우 0을 반환
*/
__vxstatic int VXHGetDataTypeSizeByte(EnumVXRDataType eValueDataType);

/*!
 * @fn __vxstatic EnumVXRDataType vxhelpers::VXHGetDataTypeFromString(wstring strDataType)
 * @brief VXFramework에서 정의한 문자열을 Data Type으로 제공
 * @param strDataType [in] \n wstring \n Data Type을 정의하는 문자열
 * @return EnumVXRDataType \n VXFramework에서 정의한 문자열과 다른 문자열이 들어 왔을 때 vxrDataTypeUNDEFINED 반환
 * @remarks 
 * @par VXFramework에서 정의한 문자열 :
 * "CHAR", "BYTE", "SHORT", "USHORT", "INT", "INT3", "UINT", "FLOAT", "DOUBLE",  \n
 * "BYTE4", "FLOAT2", "FLOAT3", "FLOAT4", "DOUBLE2", "DOUBLE3", "DOUBLE4", "STRING"
*/
__vxstatic EnumVXRDataType VXHGetDataTypeFromString(wstring strDataType);

// VolumeArchive Helper //
/*!
 * @fn __vxstatic void vxhelpers::VXHVolumeFillBoundaryWithMinValue(SVXVolumeDataArchive* psvxVolumeArchive, bool bIsClampAxisZ, SVXLocalProgress* psvxProgress)
 * @brief SVXVolumeDataArchive 에서 정의되는 Extra Boundary 볼륨 영역에 Volume의 최소값을 채움
 * @param psvxVolumeArchive [in-out] \n 
 * SVXVolumeDataArchive \n Volume이 정의되어 있는 자료구조 SVXVolumeDataArchive에 대한 포인터 \n
 * psvxVolumeArchive->ppvVolumeSlices 의 Extra Boundary 영역에 값을 채움.
 * @param bIsClampAxisZ [in] \n bool \n Volume이 z 축 방향에 대한 Extra Boundary 에 값을 Clamp 형식(경계값으로 통일)으로 채우는 가의 여부
 * @param psvxProgress [in](optional)  
 * SVXLocalProgress \n 현재 진행 정도를 처리하는 자료구조 SVXLocalProgress에 대한 포인터 \n
 * Default는 NULL 이며, 이 경우 진행 정도를 처리하지 않고 함수 수행
 * @remarks 
 * psvxVolumeArchive 에서 Volume 은 정의 및 할당되어 있어야 함. \n
 * bIsClampAxisZ 가 false 이면 Extra Boundary 영역을 볼륨의 최소값으로 채움
*/
__vxstatic void VXHVolumeFillBoundaryWithMinValue(SVXVolumeDataArchive* psvxVolumeArchive, bool bIsClampAxisZ = false, SVXLocalProgress* psvxProgress = NULL);

/*!
 * @fn __vxstatic void vxhelpers::VXHVolumeFillHistogramBasedOnVolumeValues(SVXVolumeDataArchive* psvxVolumeArchive, SVXLocalProgress* psvxProgress)
 * @brief SVXVolumeDataArchive 에서 정의되는 볼륨에 대한 Histogram을 생성
 * @param psvxVolumeArchive [in-out] \n 
 * SVXVolumeDataArchive \n Volume이 정의되어 있는 자료구조 SVXVolumeDataArchive에 대한 포인터 \n
 * psvxVolumeArchive->pullHistogram 에 메모리 할당 및 Histogram 정의
 * @param psvxProgress [in](optional) \n 
 * SVXLocalProgress \n 현재 진행 정도를 처리하는 자료구조 SVXLocalProgress에 대한 포인터 \n
 * Default는 NULL 이며, 이 경우 진행 정도를 처리하지 않고 함수 수행
 * @remarks 
 * 기존에 생성 및 정의되어 있으면 기존 것을 삭제 후 다시 생성 및 정의 \n
 * Histogram의 Array(psvxVolumeArchive->pullHistogram) 크기는 uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5)으로 정함
*/ 
__vxstatic void VXHVolumeFillHistogramBasedOnVolumeValues(SVXVolumeDataArchive* psvxVolumeArchive, SVXLocalProgress* psvxProgress = NULL);

/*!
 * @fn __vxstatic void VXHVolumeGetMinMaxValue(vxdouble2* pd2MinMaxValue, void** ppvVolume, EnumVXRDataType eDataType, vxint3 i3SizeVolume, vxint3 i3SizeExtraBoundary, SVXLocalProgress* psvxProgress)
 * @brief 볼륨의 최소 최대값을 구함
 * @param pd2MinMaxValue [out] \n vxdouble2 \n pd2MinMaxValue->x : 최소값, pd2MinMaxValue->y : 최대값
 * @param ppvVolume [in] \n void** @n 볼륨이 할당되어 있는 2D Pointer
 * @param eDataType [in] \n EnumVXRDataType \n 볼륨의 Data Type
 * @param i3SizeVolume [in] \n vxint3 \n 볼륨의 크기 i3SizeVolume.xyz = (size X, size Y, size Z)
 * @param i3SizeExtraBoundary [in] \n vxint3 \n 볼륨의 Extra Boundary 크기 i3SizeExtraBoundary.xyz = (size X, size Y, size Z), 한쪽면
 * @param psvxProgress [in](optional) \n 
 * SVXLocalProgress \n 현재 진행 정도를 처리하는 자료구조 SVXLocalProgress에 대한 포인터 \n
 * Default는 NULL 이며, 이 경우 진행 정도를 처리하지 않고 함수 수행
 * @remarks 함수 수행이 실패하면 pd2MinMaxValue에 유효값이 저장되지 않음.
*/
__vxstatic void VXHVolumeGetMinMaxValue(vxdouble2* pd2MinMaxValue, void** ppvVolume, EnumVXRDataType eDataType, vxint3 i3SizeVolume, vxint3 i3SizeExtraBoundary, SVXLocalProgress* psvxProgress = NULL);

// PrimitiveArchive Helper //
/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Sphere(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCenter, double dRadius, int iNumIter)
 * @brief SVXPrimitiveDataArchive에 구를 생성
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive 자료구조가 정의되어 있는 SVXVolumeDataArchive에 대한 포인터
 * @param d3PosCenter [in] \n vxdouble3 \n 구의 중심점 위치
 * @param dRadius [in] \n double \n 구의 반지름
 * @param iNumIter [in] \n int \n 구를 정의하는 Polygon의 정밀도를 정의, 0보다 큰 값이어야 하며 보통 3 또는 4의 값을 사용
 * @remarks 
 * sphere-approximating polygons based on surface refinement method \n
 * described in http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/sphere_cylinder/ by Paul Bourke (1992). \n
 * Given iNumIter, # of vertices:(4 ^ (iNumIter+1)) + 2, # of edges:(4 ^ (iNumIter+1)) * 3, # of triangles:(4 ^ iNumIter) * 8 \n
 * 기존에 psvxPrimitiveArchive에 Primitive가 생성되어 있으면 안 됨.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Sphere(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCenter, double dRadius, int iNumIter);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cone(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations)
 * @brief SVXPrimitiveDataArchive에 모든 면이 막혀 있는 원뿔을 생성
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive 자료구조가 정의되어 있는 SVXVolumeDataArchive에 대한 포인터
 * @param d3PosStart [in] \n vxdouble3 \n 원뿔의 밑원의 중심점 위치
 * @param d3PosEnd [in] \n vxdouble3 \n 원뿔의 뿔의 꼭지점 위치
 * @param dRadius [in] \n double \n 원뿔의 밑원의 반지름
 * @param iNumInterpolations \n int \n 원뿔을 정의하는 Polygon의 정밀도를 정의, 3이상의 값이어야 함
 * @remarks 
 * 밑원은 (360 / iNumInterpolations) 개의 삼각형 Polygon으로 정의되며 이것에 대응하는 옆면의 삼각형으로 원뿔을 정의 \n
 * 기존에 psvxPrimitiveArchive에 Primitive가 생성되어 있으면 안 됨.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cone(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cylinder(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations)
 * @brief SVXPrimitiveDataArchive에 윗원와 밑원이 막혀 있는 원통을 생성
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive 자료구조가 정의되어 있는 SVXVolumeDataArchive에 대한 포인터
 * @param d3PosStart [in] \n vxdouble3 \n 원통의 밑원의 중심점 위치
 * @param d3PosEnd \n vxdouble3 \n 원통의 윗원의 중심점 위치
 * @param dRadius \n double \n 원통의 반지름
 * @param bIsOpenCylinder \n bool \n 원통의 윗면 아랫면이 뚫려 있는가를 결정
 * @param iNumInterpolations \n int \n 원통을 정의하는 Polygon의 정밀도를 정의, 3이상의 값이어야 함
 * @remarks 
 * 원통의 원은 (360 / iNumInterpolations) 개의 삼각형 Polygon으로 정의되며 이것에 대응하는 옆면의 삼각형으로 원통을 정의 \n
 * 기존에 psvxPrimitiveArchive에 Primitive가 생성되어 있으면 안 됨.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cylinder(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, bool bIsOpenCylinder, int iNumInterpolations);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cube(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCubeMin, vxdouble3 d3PosCubeMax, double dEdgeNormalWeight, bool bIsOnlyOutline)
 * @brief SVXPrimitiveDataArchive에 좌표축과 align된 육면체 생성
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive 자료구조가 정의되어 있는 SVXVolumeDataArchive에 대한 포인터
 * @param d3PosCubeMin [in] \n vxdouble3 \n aligned axis 좌표계에서 육면체의 최소 위치점
 * @param d3PosCubeMax [in] \n vxdouble3 \n aligned axis 좌표계에서 육면체의 최대 위치점
 * @param dEdgeNormalWeight [in] \n double \n 꼭지점의 normal vector가 육면체의 면을 구성하는 vertex에 영향을 주는 정도, 0.0 ~ 1.0 사이의 값이어야 함
 * @param bIsOnlyOutline [in] \n bool \n true일 경우 polygon이 아닌 line primitive 생성, false일 경우 polygon으로 면을 생성하게끔 primitive 생성
 * @remarks 
 * 육면체가 정의되는 OS(Object Space) 기준 각 축에 평행한 육면체는 두 개의 min / max 정점인 d3PosCubeMin 와 d3PosCubeMax 으로 정의 \n
 * d3PosCubeMin.xyz < d3PosCubeMax.xyz 이어야 함 \n
 * 기존에 psvxPrimitiveArchive에 Primitive가 생성되어 있으면 안 됨.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cube(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCubeMin, vxdouble3 d3PosCubeMax, double dEdgeNormalWeight/*0.0 to 1.0*/, bool bIsOnlyOutline);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Line(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd)
 * @brief SVXPrimitiveDataArchive에 line 생선
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive 자료구조가 정의되어 있는 SVXVolumeDataArchive에 대한 포인터
 * @param d3PosStart [in] \n vxdouble3 \n line의 시작점 위치
 * @param d3PosEnd [in] \n vxdouble3 \n line의 끝점 위치
 * @remarks 기존에 psvxPrimitiveArchive에 Primitive가 생성되어 있으면 안 됨.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Line(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd);

/*!
 * @fn __vxstatic void vxhelpers::VXHCheckValidGPUSDK()
 * @brief 현재 구동 환경에서의 유효한 GPU SDK를 vector list 로 제공함
 * @param strSDK [in] \n string \n "DX11", "CUDA", "OpenGL", "OpenCL" 중 하나 
 * @return bool \n valid 하면 true, 그렇지 않으면 false
 * @remarks 시간이 약간 걸리므로 실시간 연산에서 여러번 사용하면 안 됨.
*/
__vxstatic bool VXHCheckValidGPUSDK(string strSDK);

/*!
 * @fn __vxstatic void vxhelpers::VXHComputeIntialAlignmentMatrixOS2WS()
 * @brief OS 상의 축과 WS 상의 축을 Object Bound Box 의 중심을 WS 상의 원점으로 위치시키는 변환 행렬 계산
 * @param pvxAlignAxisOS2WS [in] \n SVXAlignAxisOS2WS \n OS 상의 xyz축이 WS 상의 xyz 축에 mapping 되는 관계를 정하는 SVXAlignAxisOS2WS 를 정의하고 있는 포인터
 * @param f3PitchInOS [in] \n vxfloat3 \n OS 상의 각축 방향에 대한 1 단위에 대응하는 WS 상의 수치
 * @param svxOrthoBoxInOS [in] \n SVXOrthoBox \n OS 상의 Object Bound Box
 * @param pmatInitOS2WS [out] \n vxmatrix \n 계산되는 변환 행렬을 저장할 포인터
 * @return bool \n valid 하면 true, 그렇지 않으면 false
 * @remarks 시간이 약간 걸리므로 실시간 연산에서 여러번 사용하면 안 됨.
*/
__vxstatic bool VXHComputeIntialAlignmentMatrixOS2WS(vxmatrix* pmatInitOS2WS/*out*/, SVXAlignAxisOS2WS* pvxAlignAxisOS2WS, vxfloat3 f3PitchInOS, SVXOrthoBox svxOrthoBoxInOS);
//==========================================
// Module Parameter Structures : 2010.10.28
//==========================================
/*!
 * @fn __vxstatic int vxhelpers::VXHStringSplit(vector<wstring>* pvtrTokens, const wstring* pstrSrc, const wstring* pstrDelimiter)
 * @brief 문자열을 delimiter 문자열 기준으로 나눔.
 * @param pvtrTokens [out] \n vector<wstring> \n 나눠진 문자열의 vector list 자료구조에 대한 포인터
 * @param pstrSrc [in] \n wstring \n source 문자열에 대한 포인터
 * @param pstrDelimiter [in] \n wstring \n delimiter 문자열에 대한 포인터
 * @return int \n 나눠진 문자열의 개수
 * @remarks 하나의 delimiter 에 대해 동작.
*/
__vxstatic int VXHStringSplit(vector<wstring>* pvtrTokens, const wstring* pstrSrc, const wstring* pstrDelimiter);

/*!
 * @fn __vxstatic bool vxhelpers::VXHStringGetParameterFromCustomStringMap(void* pvParameter, map<wstring, wstring>* pmapCustomParameter, wstring strParameterName)
 * @brief map<wstring, wstring>으로 정의되는 container에서 VXFramework format의 custom parameter를 얻는 함수
 * @param pvParameter [out] \n void \n VXFramework format으로 정의된 custom parameter 문자열에서 얻은 값에 대한 포인터
 * @param pmapCustomParameter [in] \n map<wstring, wstring> \n VXFramework format의 custom parameter형식으로 작성된 map<wstring, wstring>의 포인터
 * @param strParameterName [in] \n wstring \n VXFramework format의 custom parameter name의 문자열
 * @return bool \n strParameterName이 pmapCustomParameter에 존재하고 VXFramework format에 맞는 문자열로 값이 지정되어 있으면 true, 그렇지 않으면 false
 * @remarks 
 * map<wstring, wstring> container에서 key의 형식은 "_[data type]_[name]"으로 content의 형식은 "x,x,..."으로 지정됨 \n
 * [data type]의 문자열은 다음과 같으며 \n
 * @li bool, int, double, int2, int3, int4, double2, double3, double4, matrix44, string \n
 * content의 문자열은 각각에 대하여 \n
 * @li [bool] : "true" or "false", int/double : "number", >> bool type \n
 * @li [int2/int3/int4/double2/double3/double4] : "number, number, ..." channel 개수를 일치시켜야 함. >> vx[type] type \n
 * @li [matrix44] : "number, number, ..., number" 총 16개. 앞에서부터 vxmatrix의 _m11, _m12, ..., _m43, _m44 >> vxmatrix type \n
 * @li [string] : "string" >> wstring type \n
 * 으로 정해져 있으며, 위의 형식에 따라 사용해야 함 \n
 * @details
 * @par ex. 
 * key:"_double2_customname", content:"2.0, 1.5"가 저장된 container map에 대하여 : \n
 * >> ...\n
 * >> map<wstring, wstring> mapCustomParameter; \n
 * >> mapCustomParameter.insert(pair<wstring, wstring>("_double2_customname", "2.0, 1.5"); \n
 * >> ... \n
 * >> vxdouble2 d2Value; \n
 * >> vxhelpers::VXHStringGetParameterFromCustomStringMap(&d2Value, &mapCustomParameter, "_double2_customname"); \n
 * >> // use d2Value defined as (2.0, 1.5) \n
*/
__vxstatic bool VXHStringGetParameterFromCustomStringMap(void* pvParameter, map<wstring, wstring>* pmapCustomParameter, wstring strParameterName);

/*!
 * @fn __vxstatic int vxhelpers::VXHStringGetVXObjectListFromObjectStringMap(vector<CVXObject*>* pvtrVXObjects, map<wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput)
 * @brief map< wstring, vector<CVXObject*> >으로 정의되는 container에서 VXFramework의 VXObject List를 얻는 함수
 * @param pvtrVXObjects [out] \n vector<CVXObject*> \n 함수의 결과인 VXFramework의 VXObject에 대한 vector list에 대한 포인터
 * @param pmapVXObjects [in] \n map< wstring, vector<CVXObject*> > \n VXObject List가 VXFramework의 Module Parameter 형식으로 작성된 map<wstring, vector<CVXObject*>>의 포인터
 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject의 type
 * @param bIsInput [in] \n bool \n true이면 input으로 들어 오는 VXObject List, false이면 output으로 들어 오는 VXObject List
 * @return int \n 얻어지는 VXObject List에 포함된 VXObjects 개수
 * @remarks 해당하는 VXObject가 없으면 0을 반환.
 *		\n map<wstring, vector<CVXObject*>>의 key는 다음의 형식으로 저장되어 있음
 *		\n _[in/out]_[VOLUME/PRIMITIVE/CUSTOMLIST/TRANSFERFUNCTION/IMAGEPLANE]_vector
 *		\n bIsInput이 true일 경우 _in_[...]_vector를 false일 경우 _out_[...]_vector에 해당하는 VXObject List를 얻음.
*/
__vxstatic int VXHStringGetVXObjectListFromObjectStringMap(vector<CVXObject*>* pvtrVXObjects, map< wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput);

/*!
 * @fn __vxstatic CVXObject* vxhelpers::VXHStringGetVXObjectFromObjectStringMap(map<wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
 * @brief map< wstring, vector<CVXObject*> >으로 정의되는 container에서 VXFramework의 VXObject를 얻는 함수
 * @param pmapVXObjects [in] \n map< wstring, vector<CVXObject*> > \n VXObject List가 VXFramework의 Module Parameter 형식으로 작성된 map<wstring, vector<CVXObject*>>의 포인터
 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject의 type
 * @param bIsInput [in] \n bool \n true이면 input으로 들어 오는 VXObject List, false이면 output으로 들어 오는 VXObject List
 * @param iIndex [in] \n int \n VXObject List에서 얻고자 하는 VXObject의 index
 * @return CVXObject \n 얻어진 VXObject의 포인터
 * @remarks 
 * 반환되는 VXObject는 VXObject type에 관계 없이 VXFramework에서 정의하는 VXObjects 중 최상위 CVXObject로 반환.\n
 * VXObject type에 맞게 casting 필요.
 * @details
 * @par ex.
 * >> CVXIObject* pCIObject = (CVXIObject*)vxhelpers::VXHStringGetVXObjectFromObjectStringMap(&mapVXObjects, vxenums::vxrObjectTypeIMAGEPLANE, false, 0);
*/
__vxstatic CVXObject* VXHStringGetVXObjectFromObjectStringMap(map< wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput, int iIndex);

/*!
 * @fn __vxstatic void* vxhelpers::VXHStringGetCustomObjectFromPointerStringMap(map<wstring, void*>* pmapCustomObjects, wstring strParameterName)
 * @brief map<wstring, void*>으로 정의되는 container에서 VXFramework에서 사용될 Custom Pointer를 얻는 함수
 * @param pmapCustomObjects [in] \n map<wstring, void*> \n Custom Pointer가 content로 저장되어 있는 map<wstring, void*>의 포인터
 * @param strParameterName [in] \n wstring \n VXFramework format의 custom parameter name의 문자열
 * @return void \n 얻어진 Custom Pointer, strParameterName에 해당하는 포인터가 없는 경우 NULL 반환
 * @remarks 
 * map<wstring, void*> container에서 별도로 지정된 key 문자열의 형식은 없음. \n
 * custom pointer는 platform 및 module에서 별도로 정의된 객체의 instance pointer가 될 수도 있으며, \n
 * platform 및 module의 interoperation을 통한 function callback을 위한 function pointer로 활용될 수 있음. \n
*/
__vxstatic void* VXHStringGetCustomObjectFromPointerStringMap(map<wstring, void*>* pmapCustomObjects, wstring strParameterName);
}
using namespace vxhelpers;

namespace vxobjects
{
/*!
 * @class SVXModuleParameters
 * @brief Module에 공통으로 사용될 input, output을 정의하는 자료구조
 * @remarks 각각의 container에 Module 공통이 아닌 특별하게 사용할 container 형식으로 parameter를 사용할 수도 있음
 * @sa 
 * vxhelpers::VXHStringGetVXObjectFromObjectStringMap \n 
 * vxhelpers::VXHStringGetParameterFromCustomStringMap \n
 * vxhelpers::VXHStringGetCustomObjectFromPointerStringMap
*/
struct SVXModuleParameters{
	/*!
	 * @brief Module에 사용될 VXObject 들에 대한 ID 들이 등록된 container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetVXObjectFromObjectStringMap 을 통해 ID를 얻을 수 있도록 container를 구성 \n
	 * >> wstring key 의 형식 : _[in/out]_[VOLUME/PRIMITIVE/IMAGEPLANE/TRANSFERFUNCTION/CUSTOMLIST]_vector
	 */
	map< wstring, vector<CVXObject*> > mapVXObjects;
	/*!
	 * @brief Module에 사용될 VXFramework format으로 정의된 container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 을 통해 값을 얻을 수 있도록 container를 구성 \n
	 * >> wstring key 와 Value 의 형식 \n
	 * >> _[bool/int/double/int2/int3/int4/double2/double3/double4/matrix44/string]_[name] \n
	 * >> ex. Key : "_bool_IsTfChanged", Value : "true"
	 */
	map<wstring, wstring> mapCustomParamters;
	/*!
	 * @brief Module에 사용될 custom pointer로 정의된 container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetCustomObjectFromPointerStringMap 을 통해 포인터를 얻을 수 있도록 container를 구성 \n
	 * >> wstring key 의 형식 \n
	 * >> _[in/out]_[class/vector/custom...]_[name] \n
	 * >> ex. "_in_class_GPUMANAGER"
	 */
	map<wstring, void*> mapCustomObjects;

	/*!
	 * @fn bool vxobjects::SVXModuleParameters::GetCustomParameter(void* pvParameter, wstring strParameterName)
	 * @brief map<wstring, wstring>으로 정의되는 container에서 VXFramework format의 custom parameter를 얻는 함수
	 * @param pvParameter [out] \n void \n VXFramework format으로 정의된 custom parameter 문자열에서 얻은 값에 대한 포인터
	 * @param strParameterName [in] \n map<wstring, wstring> \n VXFramework format의 custom parameter형식으로 작성된 map<wstring, wstring>의 포인터
	 * @return bool \n strParameterName이 pmapCustomParameter에 존재하고 VXFramework format에 맞는 문자열로 값이 지정되어 있으면 true, 그렇지 않으면 false
	 * @remarks @ref vxhelpers::VXHStringGetParameterFromCustomStringMap 와 동일하게 동작
	*/
	bool GetCustomParameter(void* pvParameter, wstring strParameterName)
	{
		return VXHStringGetParameterFromCustomStringMap(pvParameter, &mapCustomParamters, strParameterName);
	}
	/*!
	 * @fn int vxobjects::SVXModuleParameters::GetVXObjectList(vector<CVXObject*>* pvtrVXObjects, EnumVXRObjectType eObjectType, bool bIsInput)
	 * @brief map<wstring, vector<CVXObject*>>으로 정의되는 container에서 VXFramework의 VXObject List를 얻는 함수
	 * @param pvtrVXObjects [out] \n vector<CVXObject*> \n 함수의 결과인 VXFramework의 VXObject에 대한 vector list에 대한 포인터
	 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject의 type
	 * @param bIsInput [in] \n bool \n true이면 input으로 들어 오는 VXObject List, false이면 output으로 들어 오는 VXObject List
	 * @return int \n 얻어지는 VXObject List에 포함된 VXObjects 개수
	 * @remarks @ref vxhelpers::VXHStringGetVXObjectListFromObjectStringMap 와 동일하게 동작
	*/
	int GetVXObjectList(vector<CVXObject*>* pvtrVXObjects, EnumVXRObjectType eObjectType, bool bIsInput)
	{
		return VXHStringGetVXObjectListFromObjectStringMap(pvtrVXObjects, &mapVXObjects, eObjectType, bIsInput);
	}
	/*!
	 * @fn CVXObject* vxobjects::SVXModuleParameters::GetVXObject(EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
	 * @brief map<wstring, vector<CVXObject*>>으로 정의되는 container에서 VXFramework의 VXObject를 얻는 함수
	 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject의 type
	 * @param bIsInput [in] \n bool \n true이면 input으로 들어 오는 VXObject List, false이면 output으로 들어 오는 VXObject List
	 * @param iIndex [in] \n int \n VXObject List에서 얻고자 하는 VXObject의 index
	 * @return CVXObject \n 얻어진 VXObject의 포인터
	 * @remarks @ref vxhelpers::VXHStringGetVXObjectFromObjectStringMap 와 동일하게 동작
	*/
	CVXObject* GetVXObject(EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
	{
		return VXHStringGetVXObjectFromObjectStringMap(&mapVXObjects, eObjectType, bIsInput, iIndex);
	}
	/*!
	 * @fn void* vxobjects::SVXModuleParameters::GetCustomObject(wstring strParameterName)
	 * @brief map<wstring, void*>으로 정의되는 container에서 VXFramework에서 사용될 Custom Pointer를 얻는 함수
	 * @param strParameterName [in] \n map<wstring, void*> \n Custom Pointer가 content로 저장되어 있는 map<wstring, void*>의 포인터
	 * @return void \n 얻어진 Custom Pointer, strParameterName에 해당하는 포인터가 없는 경우 NULL 반환
	 * @remarks @ref vxhelpers::VXHStringGetCustomObjectFromPointerStringMap 와 동일하게 동작
	*/
	void* GetCustomObject(wstring strParameterName)
	{
		return VXHStringGetCustomObjectFromPointerStringMap(&mapCustomObjects, strParameterName);
	}
};
}
