/**
 * @mainpage    Manager Frame of VXFramework
 *
 * @section intro �Ұ�
 *      - VXFramework�� Manager Frame�� �̷�� Global Data Structures, Helper Functions �׸��� VXEngine APIs�� �����Ѵ�.
 *
 * @section CREATEINFO �ۼ�����
 *      - �ۼ���      :   �赿��
 *      - �ۼ���      :   2012/8/6
 *      - Contact     :   korfriend@gmail.com
 *
 * @section MODIFYINFO ��������
 *      - 2012.7.27    :   �� ��¥ �ҽ��� �������� VXFramework Doxygen 0.0.1 ���� �ۼ�
 *      - 2012.7.30    :   namespace [vxenums, vxmathproj] ���� �߰� / �Ϸ�
 *      - 2012.7.31    :   namespace [vxhelpers] ���� �߰� / �Ϸ�
 *      - 2012.8.1     :   namespace [vxengineapi] ���� �߰�, vxenums, vxmathproj, vxhelpers ���� ����
 *      - 2012.8.2     :   namespace [vxengineapi] ���� �Ϸ�, [vxobjects] ���� �߰�
 *      - 2012.8.3     :   namespace [vxobjects] ���� �Ϸ�, vxengineapi ���� ����, class library refactoring
 *      - 2012.8.6     :   namespace [vxgpuinterfaces] ���� �Ϸ�, �� �� internal �ڷᱸ���� ���� ���� �۾� �Ϸ�
 *      - 2012.8.15    :   vxengineapi::VXEVolumeSetVolumeInfo �߰�, vxenums::EnumVXRMetricType ���� �� ���� �ڷ� ���� ����
 *      - 2012.11.26    :  Entire Framework is refactored
 */
 
/**
 * @file VXEngineGlobalUnit.h
 * @brief Global Data Structures �� Classes �׸��� Helper Functions ����� ����.
 * @section Include & Link ����
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
 * @brief VXFramework���� ����ϴ� STL(standard template library)�� ���� namespace
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
 * @brief Common Data Structure�� ���Ǵ� enumeration �� ���� namespace
 */
namespace vxenums{
/*! VXFramework���� �����ϴ� ��ȯ ��ǥ���� ����, 4x4 matrix�� �����ϴ� Projecting ��ȯ���� ���� ���� */
enum EnumVXVCoordSpace{	
	vxvCoordSpaceSCREEN = 0,/*!< Pixel�� ���ǵǴ� Screen Space */
	vxvCoordSpacePROJECTION,/*!< Normalized Frustrum���� ���ǵǴ� Projection Space */
	vxvCoordSpaceCAMERA,/*!< Viewing Frustrum���� ���ǵǴ� Camera Space */
	vxvCoordSpaceWORLD,/*!< ��ü�� ������ ��ġ�Ǵ� World Space */
	vxvCoordSpaceOBJECT/*!< ��ü�� ���ǵǴ� Object Space */
};

/*! Camera�� �ʱ� States(��ġ, View �� Up Vector)�� ���� ���� */
enum EnumVXVStageViewType{
	vxvStageViewORTHOBOXOVERVIEW = 0,/*!< 3D View���� OS�� Object Bounding Box�� �밢�� ���⿡ ���� Overview�� ���� */
	vxvStageViewCENTERFRONT,/*!< �ܸ� ���󿡼� OS�� Object Bounding Box�� ���� Front (or Coronal) View ���� */
	vxvStageViewCENTERRIGHT,/*!< �ܸ� ���󿡼� OS�� Object Bounding Box�� ���� Right (or Sagittal) View ���� */
	vxvStageViewCENTERHORIZON/*!< �ܸ� ���󿡼� OS�� Object Bounding Box�� ���� Top (or Axial) View ���� */
};

/*! Polygonal VObject�� �����ϴ� Primitive ���� */
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

/*! Module-Platform interoperation�� ���Ǵ� VXObject�� type ���� */
enum EnumVXRObjectType{	
	vxrObjectTypeNONE = 0,/*!< Undefined */
	vxrObjectTypeVOLUME,/*!< Volume VXObject */
	vxrObjectTypePRIMITIVE,/*!< Polygon VXObject */
	vxrObjectTypeCUSTOMLIST,/*!< ������ List �ڷᱸ�� ����� inter-operation�� ���� VXObject */
	vxrObjectTypeTRANSFERFUNCTION,/*!< OTF�� �����ϴ� VXObject */
	vxrObjectTypeIMAGEPLANE/*!< Image Plane�� �����ϴ� VXObject�̸� Camera Object�� ���� */
};

/*! VXFramework���� ���������� ���Ǵ� Data types, 2010.10.15~Current Version */
/*! VXFramework���� ���������� ���Ǵ� Data types, 2012.10.18 : vxrDataTypeINT3, vxrDataTypeSTRING �߰� */
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

/*! CVXIObject���� ����ϴ� Frame Buffer ���� */
enum EnumVXRFrameBufferUsage{
	vxrFrameBufferUsageNONE = 0,/*!< Undefined, There is no allocated frame buffer */
	// Render //
	vxrFrameBufferUsageRENDEROUT,/*!< Used for rendering out buffer, the buffer should have vxrDataTypeBYTE4CHANNEL as data type */
	// Depth //
	vxrFrameBufferUsageDEPTH,/*!< Used for depth buffer, the buffer should have vxrDataTypeFLOAT as data type */
	// Custom //
	vxrFrameBufferUsageCUSTOM,/*!< Used for customized purpose, the buffer may have any type */
};

/*! VXFramework���� ����ϴ� Module ����, CVXModuleArbiter���� Module�� �����ϴ� ������ ��� */
enum EnumVXMModuleType{	
	vxmModuleTypeNONE = 0,/*!< Undefined, There is no such a module */
	vxmModuleTypeRENDER,/*!< Rendering module */
	vxmModuleTypeTRANSFERFUNCTION,/*!< OTF setting ���� module */
	vxmModuleTypeVGENERATION,/*!< VObject ����/ó��/�м� ���� module */
	vxmModuleTypeRECONSTRUCTION,/*!< CT Reconstruction ���� Module */
	vxmModuleTypeHELPER/*!< Customized-Define Module */
};
}
using namespace vxenums;

/**
 * @package vxmathproj
 * @brief VXFramework���� ���Ǵ� projection space�� ���� math �ڷᱸ�� �� math helper functions�� ���� namespace
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
 *  \brief 3D position�� 4x4 matrix�� ��ȯ�ϴ� function. Homogeneous factor w = 1 �� �ΰ� projecting ó��.
 *  \param pf3PosOut [out] \n vxfloat3 \n operation�� ����� vxfloat3�� ������.
 *  \param pf3PosIn [in] \n vxfloat3 \n operation�� source �� ���� 3D position�� ���� ������.
 *  \param psvxMatrix [in] \n vxmatrix \n operation�� ��ȯ�� �����ϴ� 4x4 matrix�� ���� ������
 *  \remarks row major operation
 */
__vxstatic void VXHMTransformPoint(vxfloat3* pf3PosOut, const vxfloat3* pf3PosIn, const vxmatrix* psvxMatrix);

/*! \fn __vxstatic void vxmathproj::VXHMTransformVector(vxfloat3* pf3VecOut, const vxfloat3* pf3VecIn, const vxmatrix* psvxMatrix)
 *  \brief 3D vector�� 4x4 matrix�� ��ȯ�ϴ� function. Homogeneous factor w = 1 �� �ΰ� projecting ó��.
 *  \param pf3VecOut [out] \n vxfloat3 \n operation�� ����� vxfloat3�� ������.
 *  \param pf3VecIn [in] \n vxfloat3 \n operation�� source �� ���� 3D vector�� ���� ������.
 *  \param psvxMatrix [in] \n vxmatrix \n operation�� ��ȯ�� �����ϴ� 4x4 matrix�� ���� ������
 *  \remarks row major operation
 */
__vxstatic void VXHMTransformVector(vxfloat3* pf3VecOut, const vxfloat3* pf3VecIn, const vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixMultiply(vxmatrix* psvxMatrix , const vxmatrix* psvxMatrix1st, const vxmatrix* psvxMatrix2nd)
 * @brief �� ���� 4x4 matrix�� ����.
 * @param psvxMatrix [out] \n vxmatrix \n 4x4 matrix�� ������ ����� vxmatrix�� ������
 * @param psvxMatrix1st [in] \n vxmatrix \n operation�� source�� 4x4 matrix�� ���� ������ (����)
 * @param psvxMatrix2nd [in] \n vxmatrix \n operation�� source�� 4x4 matrix�� ���� ������ (������)
 * @remarks n row major operation�̸� ���� ������ ���ʿ��� ���������� �����.
 */
__vxstatic void VXHMMatrixMultiply(vxmatrix* psvxMatrix , const vxmatrix* psvxMatrix1st, const vxmatrix* psvxMatrix2nd);

/*!
 * @fn __vxstatic void vxmathproj::VXHMAddVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief �� ���� 3D vector�� ����.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector�� ������ ����� vxfloat3�� ������
 * @param pf3Vec1 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @param pf3Vec2 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @remarks 
*/
__vxstatic void VXHMAddVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMSubstractVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief �� ���� 3D vector�� ��.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector�� ���� ����� vxfloat3�� ������
 * @param pf3Vec1 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @param pf3Vec2 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @remarks 
*/
__vxstatic void VXHMSubstractVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic float vxmathproj::VXHMDotVector(const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief �� ���� 3D vector�� ���� dot product�� ����.
 * @param pf3Vec1 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @param pf3Vec2 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������
 * @return float \n dot product�� scalar ���
 * @remarks 
*/
__vxstatic float VXHMDotVector(const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMCrossDotVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2)
 * @brief �� ���� 3D vector�� ���� cross dot product�� ����.
 * @param pf3VecOut [out] \n vxfloat3 \n 3D vector�� cross dot product�� ����� vxfloat3�� ������
 * @param pf3Vec1 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������ (����)
 * @param pf3Vec2 [in] \n vxfloat3 \n operation�� source�� 3D vector�� ���� ������ (������)
 * @remarks pf3Vec1���� pf3Vec2�������� cross dot�� ����, (���ʿ��� ������)
*/
__vxstatic void VXHMCrossDotVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec1, const vxfloat3* pf3Vec2);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixIdentity(vxmatrix* psvxMatrix)
 * @brief identity matrix�� �����ϴ� �Լ�
 * @param psvxMatrix [out] \n vxmatrix \n identity matrix�� ����� vxmatrix�� ������
 * @remarks 
*/
__vxstatic void VXHMMatrixIdentity(vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixWS2CS(vxmatrix* psvxMatrix, const vxfloat3* pf3PosEye, const vxfloat3* pf3VecUp, const vxfloat3* pf3VecView)
 * @brief ������ �������� ���ǵ� Space�� �������� look-at Space�� ��ȯ�ϴ� matrix�� ����.
 * @param psvxMatrix [out] \n vxmatrix \n look-at Space(�Ǵ� Camera Space or Viewing Space)�� ��ȯ�ϴ� vxmatrix�� ���� ������
 * @param pf3PosEye [in] \n vxfloat3 \n Camera(�Ǵ� Eye)�� position�� �����ϴ� vxfloat3�� ������
 * @param pf3VecUp [in] \n vxfloat3 \n ���� Camera(�Ǵ� Eye)�� ���ǵ� Space������ ���� ������ �����ϴ� vector�� ���� vxfloat3�� ������
 * @param pf3VecView [in] \n vxfloat3 \n ���� Camera(�Ǵ� Eye)�� ���ǵ� Space������ �ü��� ������  �����ϴ� vector�� ���� vxfloat3�� ������
 * @remarks 
 * RHS �����̸� row major �������� matrix�� ���� \n
 * f3PosEye, f3VecUp, f3VecView�� Space�� ��� �����ؾ� �ϸ� �ϳ��� World Space�� ���� \n
 * Camera Space������ View ������ -z�� ����, Up ������ y�� ���� \n
*/
__vxstatic void VXHMMatrixWS2CS(vxmatrix* psvxMatrix, const vxfloat3* pf3PosEye, const vxfloat3* pf3VecUp, const vxfloat3* pf3VecView);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixOrthogonalCS2PS(vxmatrix* psvxMatrix, float fWidthFrustum, float fHeightFrustum, float fNearPlane, float fFarPlane)
 * @brief ������ �������� ���ǵ� Space�� �������� Orhogonal Projecting�� �����ϴ� matrix�� ����.
 * @param psvxMatrix [out] \n vxmatrix \n Projection Space(�Ǵ� Camera Space or Viewing Space)�� ��ȯ�ϴ� vxmatrix�� ���� ������
 * @param fWidthFrustum [in] \n float \n View Frustum �� width�� Camera Space���� ���ǵ� ���� ���
 * @param fHeightFrustum [in] \n float \n View Frustum �� height�� Camera Space���� ���ǵ� ���� ���
 * @param fNearPlane [in] \n float \n View Frustum �� minimum z ������ Camera Space���� ���ǵ� ���� ���
 * @param fFarPlane [in] \n float \n View Frustum �� maximum z ������ Camera Space���� ���ǵ� ���� ���
 * @remarks 
 * RHS �����̸� row major �������� matrix�� ���� \n
 * look-at Space(�Ǵ� Camera Space or Viewing Space)���� Projection Space�� ��ȯ \n
 * View Frustum ���� ������ ��� Cripping out �Ǿ� Projection ���� ���� \n
 * ��ǥ���� ������ Camera Space�� ����
*/
__vxstatic void VXHMMatrixOrthogonalCS2PS(vxmatrix* psvxMatrix, float fWidthFrustum, float fHeightFrustum, float fNearPlane, float fFarPlane);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixPerspectiveCS2PS(vxmatrix* psvxMatrix, float fFovY, float fAspectRatio, float fNearPlane, float fFarPlane)
 * @brief ������ �������� ���ǵ� Space�� �������� Perspective Projecting�� �����ϴ� matrix�� ����.
 * @param psvxMatrix [out] \n vxmatrix \n Projection Space(�Ǵ� Camera Space or Viewing Space)�� ��ȯ�ϴ� vxmatrix�� ���� ������
 * @param fFovY [in] \n float \n Perspective View Frustum �� y ���⿡ ���� field of view�� radian���� ������ ��
 * @param fAspectRatio [in] \n float \n Perspective View Frustum �� Aspect Ratio�� Viewing Space���� ���ǵǴ� View Plane�� (width / height)
 * @param fNearPlane [in] \n float \n View Frustum �� minimum z ������ Camera Space���� ���ǵ� ���� ���
 * @param fFarPlane [in] \n float \n View Frustum �� maximum z ������ Camera Space���� ���ǵ� ���� ���
 * @remarks 
 * RHS �����̸� row major �������� matrix�� ���� \n
 * look-at Space(�Ǵ� Camera Space or Viewing Space)���� Projection Space�� ��ȯ \n
 * View Frustum ���� ������ ��� Cripping out �Ǿ� Projection ���� ���� \n
 * ��ǥ���� ������ Camera Space�� ����
*/
__vxstatic void VXHMMatrixPerspectiveCS2PS(vxmatrix* psvxMatrix, float fFovY, float fAspectRatio, float fNearPlane, float fFarPlane);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixPS2SS(vxmatrix* psvxMatrix, float fWidthScreen, float fHeightScreen)
 * @brief ������ �������� ���ǵ� Space�� �������� Screen�� Pixel Plane���� ���ǵǴ� Screen Space�� ��ȯ�ϴ� matrix ����.
 * @param psvxMatrix [out] \n vxmatrix \n Screen Space�� ��ȯ�ϴ� vxmatrix�� ���� ������
 * @param fWidthScreen [in] \n float \n Screen�� width�� pixel ������ ����
 * @param fHeightScreen [in] \n float \n Screen�� height�� pixel ������ ����
 * @remarks 
 * RHS �����̸� row major �������� matrix�� ���� 
 * Projection Space���� Screen Space�� ��ȯ \n
 * Projection Space���� ���ǵǴ� View Frustum�� Near Plane�� Screen Plane�� Mapping��. \n
 * Screen Space���� Screen�� �������� x��, �Ʒ����� y��, Viewing Depth ������ z������ ����
*/
__vxstatic void VXHMMatrixPS2SS(vxmatrix* psvxMatrix, float fWidthScreen, float fHeightScreen);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixInverse(vxmatrix* psvxMatrixInv, const vxmatrix* psvxMatrix)
 * @brief Inverse matrix�� ����
 * @param psvxMatrixInv [out] \n vxmatrix \n inverse matrix�� ����� vxmatrix�� ������
 * @param psvxMatrix [in] \n vxmatrix \n operation�� source�� matrix�� ���� ������
 * @remarks Determinant�� 0�� ��� psvxMatrixInv�� ���� ��ȯ���� �ʰ� ��ȯ
*/
__vxstatic void VXHMMatrixInverse(vxmatrix* psvxMatrixInv, const vxmatrix* psvxMatrix);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixRotationAxis(vxmatrix* psvxMatrix, const vxfloat3* pf3VecRotationAxis, float fRotAngleRad)
 * @brief ������ �������� �־��� ���� �߽����� ȸ���ϴ� matrix ����
 * @param psvxMatrix [out] \n vxmatrix \n ȸ�� matrix�� ����� vxmatrix�� ������
 * @param pf3VecRotationAxis [in] \n vxfloat3 \n ȸ������ �����ϴ� 3D vector�� �����ϴ� ������
 * @param fRotAngleRad [in] \n float \n ȸ������ ����. Radian ����.
 * @remarks ��ǥ���� ���⿡ ���� ȸ�� ������ ������ (ex. RHS�� ��� �������� ����, LHS�� ��� �޳��� ����)
*/
__vxstatic void VXHMMatrixRotationAxis(vxmatrix* psvxMatrix, const vxfloat3* pf3VecRotationAxis, float fRotAngleRad);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixScaling(vxmatrix* psvxMatrix, vxfloat3 f3ScaleXYZ)
 * @brief ���� ��ǥ�谡 �����ϴ� ���� ������ ���� scale �ϴ� matrix ����
 * @param psvxMatrix [out] \n vxmatrix \n scaling matrix�� ����� vxmatrix�� ������
 * @param f3ScaleXYZ [in] \n vxfloat3 \n (x,y,z) �� �� ���⿡ ���� scaling factor�� �����ϴ� vxfloat3�� ������
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixScaling(vxmatrix* psvxMatrix, vxfloat3 f3ScaleXYZ);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixTranslation(vxmatrix* psvxMatrix, vxfloat3 f3TranslateXYZ)
 * @brief ���� ��ǥ�谡 �����ϴ� ���� ������ ���� �����̵��ϴ� matrix ����
 * @param psvxMatrix [out] \n vxmatrix \n �����̵� matrix�� ����� vxmatrix�� ������
 * @param f3TranslateXYZ [in] \n vxfloat3 \n (x,y,z) �� �� ���⿡ ���� translation factor�� �����ϴ� vxfloat3�� ������
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixTranslation(vxmatrix* psvxMatrix, vxfloat3 f3TranslateXYZ);

/*!
 * @fn __vxstatic void vxmathproj::VXHMMatrixTranspose(vxmatrix* psvxMatrixDst, const vxmatrix* psvxMatrixSrc)
 * @brief transpose ������ ����
 * @param psvxMatrixDst [out] \n vxmatrix \n transpose �� matrix�� ����� vxmatrix�� ������
 * @param psvxMatrixSrc [in] \n vxmatrix \n transpose �� matrix�� ����� vxmatrix�� ������
 * @remarks row major operation
*/
__vxstatic void VXHMMatrixTranspose(vxmatrix* psvxMatrixDst, const vxmatrix* psvxMatrixSrc);

/*!
 * @fn __vxstatic void vxmathproj::VXHMNormalizeVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec)
 * @brief 3D vector�� ���� ���̰� 1�� ���� vector�� ����
 * @param pf3VecOut [out] \n vxfloat3 \n �����̵� matrix�� ����� vxmatrix�� ������
 * @param pf3Vec [in] \n vxfloat3 \n �����̵� matrix�� ����� vxmatrix�� ������
 * @remarks ���̰� 0�� 3D vector�� �־����� ���ǹ��� 3D vector ����.
*/
__vxstatic void VXHMNormalizeVector(vxfloat3* pf3VecOut, const vxfloat3* pf3Vec);

/*!
 * @fn __vxstatic float vxmathproj::VXHMLengthVector(const vxfloat3* pf3Vec)
 * @brief 3D vector�� ���̸� ���
 * @param pf3Vec [in] \n vxfloat3 \n operation�� source 3D vector�� ������
 * @return \n 3D vector�� ����
*/
__vxstatic float VXHMLengthVector(const vxfloat3* pf3Vec);

/*!
 * @fn __vxstatic float vxmathproj::VXHMLengthVectorSq(const vxfloat3* pf3Vec)
 * @brief 3D vector�� ������ ������ ���
 * @param pf3Vec [in] \n vxfloat3 \n operation�� source 3D vector�� ������
 * @return \n 3D vector�� ������ ����
 * @remarks 3D vector�� ���� ����� ���� �Ŀ��� square root�� ������ ���� ���� ��ȯ
*/
__vxstatic float VXHMLengthVectorSq(const vxfloat3* pf3Vec);

// double precision
/*!
 * @fn __vxstatic void vxmathproj::VXHMDoubleMatrixMultiply(vxmatrixdouble* psvxMatrix , const vxmatrixdouble* psvxMatrix1st, const vxmatrixdouble* psvxMatrix2nd)
 * @brief �� ���� 4x4 matrix�� ����.
 * @param psvxMatrix [out] \n vxmatrix \n 4x4 matrix�� ������ ����� vxmatrix�� ������
 * @param psvxMatrix1st [in] \n vxmatrix \n operation�� source�� 4x4 matrix�� ���� ������ (����)
 * @param psvxMatrix2nd [in] \n vxmatrix \n operation�� source�� 4x4 matrix�� ���� ������ (������)
 * @remarks 
 * n row major operation�̸� ���� ������ ���ʿ��� ���������� �����.
 * VXHMMatrixMultiply�� ������ �����̸� float precision�� ��� double precision���� ó�� \n
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
 * @brief VXFramework�� Global Data Structures �� VXObject Classes�� ���� namespace
 */
namespace vxobjects
{
//=========================
// Object Structures
//=========================
/**
 * @class SVXPlane
 * @brief Plane�� �����ϴ� �ڷᱸ��
 */
struct SVXPlane{
	/// Plane ���� �� ���� ��ġ
	vxfloat3 f3PosPointsOnPlane;
	/// Plane �� Normal Vector
	vxfloat3 f3VecNormal;
	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
	SVXPlane(){ VXSTRUCTURENULLSET(f3PosPointsOnPlane); VXSTRUCTURENULLSET(f3VecNormal); }
};
/**
 * @class SVXOrthoBox
 * @brief ���� ��ǥ���� Axis�� ������ Box�� �����ϴ� �ڷᱸ��
 */
struct SVXOrthoBox{
	/// ���� ��ǥ���� Axis�� ������ Box�� ���� �ִ��� ��ġ
	vxfloat3 f3PosMaxBox;
	/// ���� ��ǥ���� Axis�� ������ Box�� ���� �ּ��� ��ġ
	vxfloat3 f3PosMinBox;
	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
	SVXOrthoBox(){ VXSTRUCTURENULLSET(f3PosMaxBox); VXSTRUCTURENULLSET(f3PosMinBox); }
	SVXOrthoBox(vxint3 i3SizeVolume)	{
		f3PosMinBox = vxfloat3(-0.5f, -0.5f, -0.5f);
		vxint3 i3IndexMaxRange = i3SizeVolume - vxint3(1, 1, 1);
		f3PosMaxBox = vxfloat3((float)i3IndexMaxRange.x, (float)i3IndexMaxRange.y, (float)i3IndexMaxRange.z) + vxfloat3(0.5f, 0.5f, 0.5f);

		if(!IsAvailableBox())
			VXERRORMESSAGE("ERROR in SVXOrthoBox");
	}
	/// ���� ��ǥ���� SVXOrthoBox �� ��ȿ�ϰ� ���ǵǾ��°� Ȯ��
	bool IsAvailableBox(){
		if(f3PosMaxBox.x <= f3PosMinBox.x || f3PosMaxBox.y <= f3PosMinBox.y || f3PosMaxBox.z <= f3PosMinBox.z)
			return false;
		return true;
	}
};
/**
 * @class SVXOrthoRect
 * @brief ���� ��ǥ��(2D)�� Axis�� ������ Rectangle�� �����ϴ� �ڷᱸ��
 */
struct SVXOrthoRect{
	/// ���� ��ǥ���� Axis�� ������ Rectangle�� ���� �ִ��� ��ġ
	vxfloat2 f2PosMaxRect;
	/// ���� ��ǥ���� Axis�� ������ Rectangle�� ���� �ִ��� ��ġ
	vxfloat2 f2PosMinRect;
	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
	SVXOrthoRect(){ VXSTRUCTURENULLSET(f2PosMaxRect); VXSTRUCTURENULLSET(f2PosMinRect); }
};
/**
 * @class SVXSimpleVolumeDefinition
 * @brief Volume�� �����ϴ� array �� �̸� ���� �ּ��� ������ �����ϴ� �ڷᱸ��
 * @sa vxobjects::SVXVolumeDataArchive
 */
struct SVXSimpleVolumeDefinition{
	/// ���� ����� Volume�� data type
	EnumVXRDataType eVolumeDataType;
	/// Volume�� �����ϰ� �ִ� 2D pointer
	void** ppvVolumeSlices;
	/**
	 * @brief Volume �� ũ�� i3VolumeSize = (width, height, depth or slices) \n
	 * i3SizeExtraBoundary �� ���Ե��� ����
	 */
	vxint3 i3VolumeSize;
	/**
	 * @brief CPU Memory Access Violation�� ���ϱ� ���� System Memory ���� Extra Boundary ������ ���ʸ� ũ��
	 * @details i3SizeExtraBoundary = (���� x�� ���� ũ��, ���� y�� ���� ũ��, ���� z�� ���� ũ��)
	 */
	int iSizeExtraBoundary;
	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
	SVXSimpleVolumeDefinition(){ 
		VXSTRUCTURENULLSET(i3VolumeSize); iSizeExtraBoundary = 0;
		ppvVolumeSlices = NULL; eVolumeDataType = vxrDataTypeUNDEFINED;
	}
	/// ������ ppvVolumeSlices �� �Ҵ�� �޸� ����
	void Delete(){
		VXSAFE_DELETE2DARRAY(ppvVolumeSlices, i3VolumeSize.z + iSizeExtraBoundary*2);
	}
};

/**
 * @class SVXAlignAxisOS2WS
 * @brief Object Space ���ǵ� x��(1,0,0), y��(0,1,0), z��(0,0,z)�� ó�� World Space �� ��ġ�� ���� ������ ���� \n
 * pitch �� ������� ������ ���⸸ ����, (�� vector �� ���ؼ��� ��ȿ��)
 * @sa
 * @ref vxobjects::SVXVolumeDataArchive
 */
struct SVXAlignAxisOS2WS{
	/**
	 * @brief Object Space ���ǵ� x��(1,0,0)�� �����Ǵ� World Space �� ��ġ�� Object�� x���� ����, unit vector
	 */
	vxfloat3 f3VecAxisOSX2WSX;
	/**
	 * @brief Object Space ���ǵ� y��(0,1,0)�� �����Ǵ� World Space �� ��ġ�� Object�� y���� ����, unit vector
	 */
	vxfloat3 f3VecAxisOSY2WSY;
	/**
	 * @brief Object Space ���ǵ� z��(0,0,1)�� �����Ǵ� World Space �� ��ġ�� Object�� z���� �����ϱ� ���� XY RHS cross vecor ������ reverse ����\n
	 * true �� RHS �� ��ġ�Ǹ� Affine Space ���� ��ȯ ����, false �� LHS �� z���� ��ġ
	 */
	bool bIsAxisZ_RHS;
	/**
	 * @brief f3VecAxisOSX2WSX, f3VecAxisOSY2WSY, bIsAxisZ_RHS �κ��� ���ǵǴ� �ʱ� OS2WS ��ȯ ���
	 */
	vxmatrix matAlignOS2WS;
	/**
	 * @brief constructor, �ʱ�ȭ �۾� ����
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
	/// ���ǵ� f3VecAxisOSX2WSX�� f3VecAxisOSX2WSY�κ��� matInitOS2WS ����Ͽ� ���
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
 * @brief VXFramework���� �����ϴ� Volume�� �� ������ ���� �ڷᱸ��
 * @sa
 * @ref vxobjects::CVXVObjectVolume \n
 * vxhelpers::VXHVolumeFillHistogramBasedOnVolumeValues
 */
struct SVXVolumeDataArchive{
	/**
	 * @brief Volume array �� data type
	 */
	EnumVXRDataType eVolumeDataType;
	/**
	 * @brief �޸𸮿� ����Ǳ� �� Volume Original data type
	 * @par ex. 
	 * file format float ����� ������ ushort ���� ������ ���
	 * @par
	 * >> eDataType = vxrDataTypeUNSIGNEDSHORT, eDataTypeOriginal = vxrDataTypeFLOAT;
	 */
	EnumVXRDataType eVolumeDataTypeOriginal;
	/**
	 * @brief Volume�� ������ 2D array
	 * @details 
	 * ���� �Ҵ�� x �� ���� ũ�� = i3VolumeSize.x + i3SizeExtraBoundary.x*2 \n
	 * ���� �Ҵ�� y �� ���� ũ�� = i3VolumeSize.y + i3SizeExtraBoundary.y*2 \n
	 * ���� �Ҵ�� z �� ���� ũ�� = i3VolumeSize.z + i3SizeExtraBoundary.z*2 \n
	 * @par ex. 
	 * ushort 512x512x512 Volume���� (100, 120, 150) index �� sample \n
	 * @par
	 * >> int iSamplePosX = 100 + i3SizeExtraBoundary.x; \n
	 * >> int iSamplePosY = 120 + i3SizeExtraBoundary.y; \n
	 * >> int iSamplePosZ = 150 + i3SizeExtraBoundary.z; \n
	 * >> ushort usValue = ((ushort**)ppvVolumeSlices)[iSamplePosZ][iSamplePosX + iSamplePosY*(i3VolumeSize.x + i3SizeExtraBoundary.x*2)];
	 */
	void** ppvVolumeSlices;
	/**
	 * @brief CPU Memory Access Violation�� ���ϱ� ���� System Memory ���� Extra Boundary ������ ���ʸ� ũ��
	 * @details i3SizeExtraBoundary = (���� x�� ���� ũ��, ���� y�� ���� ũ��, ���� z�� ���� ũ��)
	 */
	vxint3 i3SizeExtraBoundary;
	/**
	 * @brief Volume �� ũ�� i3VolumeSize = (width, height, depth or slices)
	 * @details i3SizeExtraBoundary �� ���Ե��� ����
	 */
	vxint3 i3VolumeSize;
	/**
	 * @brief ���� Voxel�� ���� OS �� cell �� WS ���� ũ��
	 * @details d3VoxelPitch = (OS ���� x ���� voxel ũ��, OS ���� y ���� voxel ũ��, OS ���� z ���� voxel ũ��)
	 */
	vxfloat3 f3VoxelPitch;
	/**
	 * @brief Volume���� �����(ppvVolumeSlices)�� �ּҰ� d2MinMaxValue.x, �ִ밪 d2MinMaxValue.y
	 */
	vxdouble2 d2MinMaxValue;
	/**
	 * @brief Volume���� ����Ǳ� ���� ���ǵ� �ּҰ� d2ActualMinMaxValue.x, �ִ밪 d2ActualMinMaxValue.y
	 * @par ex. 
	 * file format float���� -1.5 ~ 2.5 ����� ������ ushort ���� ������ ��� 
	 * @par 
	 * >> d2MinMaxValue = vxdouble(0, 65535), d2ActualMinMaxValue = vxdouble(-1.5, 2.5);
	 */
	vxdouble2 d2ActualMinMaxValue;
	/**
	 * @brief Volume�� ���� Histogram �� �����ϴ� array
	 * @details 
	 * array ũ��� uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5) \n
	 * pullHistogram[volume value] = # of voxels
	 */
	ullong* pullHistogram;	
	/**
	 * @brief memory �� ����� volume space (���� ��ǥ)�� �ʱ� world space �� ��ġ�Ǵ� ��ȯ matrix ����
	 */
	SVXAlignAxisOS2WS svxAlignAxisOS2WS;
	/**
	 * @brief constructor, �ʱ�ȭ �۾� ����
	 * @details 
	 * >> eVolumeDims = {vxrMetricTypePOSITIONX, vxrMetricTypePOSITIONY, vxrMetricTypePOSITIONZ, vxrMetricTypeDENSITY};\n
	 * >> d2ActualMinMaxValue = d2MinMaxValue = vxdouble(DBL_MAX, -DBL_MAX);\n
	 * �� �ܴ� ��� NULL (or 0, false) �� �ʱ�ȭ
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
	 * @brief Histogram array �� ũ�⸦ ����, uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5)
	 */
	uint GetHistogramSize(){return (uint)(max(d2MinMaxValue.y - d2MinMaxValue.x + 1.5, 1.0));}
	/**
	 * @brief ppvVolumeSlices array ũ�⸦ ����, Extra Boundary�� ����� ũ��
	 */
	vxint3 GetSampleSize(){return vxint3(i3VolumeSize.x + i3SizeExtraBoundary.x*2, i3VolumeSize.y + i3SizeExtraBoundary.y*2, 
		i3VolumeSize.z + i3SizeExtraBoundary.z*2);}

	/// ������ ppvVolumeSlices �� pullHistogram �� �Ҵ�� �޸� ����
	void Delete(){
		VXSAFE_DELETE2DARRAY(ppvVolumeSlices, i3VolumeSize.z + i3SizeExtraBoundary.z*2);
		VXSAFE_DELETEARRAY(pullHistogram);
	}
};

/**
 * @class SVXPrimitiveDataArchive
 * @brief VXFramework���� �����ϴ� Primitive �� �̷���� ��ü�� �� ������ ���� �ڷᱸ��
 * @sa vxobjects::CVXVObjectPrimitive
 */

struct SVXPrimitiveDataArchive{
public:
	/**
	 * @brief vertex array�� �����ϴ� container map
	 * memory �Ҵ�� pointer�� value�� ����, @ref SVXPrimitiveDataArchive::Delete ���� ������.
	 */
	map<EnumVXRVertexType, vxfloat3*> mapVerticeDefinitions;
public:
	/**
	 * @brief Primitive�� ���� �� ��ü�� Polygon �� normal vector ������ vertex �迭 ����
	 */
	bool bIsPrimitiveFrontCCW;
	/**
	 * @brief Primitive Type
	 */
	EnumVXRPrimitiveType ePrimitiveType;
	/**
	 * @brief Primitive�� �����ϴ� vertex�� �迭 ����, true : stripe, false : list
	 */
	bool bIsPrimitiveStripe;
	/**
	 * @brief Primitive�� ���� �� ��ü�� Polygon ����
	 */
	uint uiNumPolygons;
	/**
	 * @brief �ϳ��� Primitive(Polygon)�� �����ϴ� index ����
	 */
	uint uiStrideIndexListPerPolygon;
	/**
	 * @brief Vertex�� Index������� Polygon�� �����ϱ� ���� Index Buffer (puiIndexList) �� ũ��
	 * @details
	 * >> if(bIsPrimitiveStripe)\n
	 * >>    uiNumIndice = uiNumPolygons + (uiStrideIndexListPerPolygon - 1);\n
	 * >> else\n
	 * >>    uiNumIndice = uiNumPolygons*uiStrideIndexListPerPolygon;
	 */
	uint uiNumIndice;
	/**
	 * @brief Vertex�� Index������� Polygon�� ���� Index Buffer�� ���ǵ� array
	 */
	uint* puiIndexList;
	/**
	 * @brief Primitive�� ���� �� ��ü�� Vertex ����
	 */
	uint uiNumVertice;
	/**
	 * @brief SVXPrimitiveDataArchive ������ OS �󿡼� ���ǵǴ� bounding box
	 */
	SVXOrthoBox svxVertexBoundingOrthoBox;
	/**
	 * @brief Texture Resource �� ���� Pointer \n
	 */
	void* pvTextureResource;

	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
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
	 * @brief ������ puiIndexList �� mapVerticeDefinitions �� value�� ���ǵ� pointer �� �Ҵ�� �޸� ����
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
	 * @brief mapVerticeDefinitions �� value�� ���ǵ� pointer ��ȯ�ϴ� method, 
	 * @param eVertexType [in] \n EnumVXRVertexType \n vertex buffer�� �̸�, (mapVerticeDefinitions�� key)
	 * @return vxfloat3 \n vertex buffer�� �����ϴ� vxfloat3�� ������. ������ NULL ��ȯ
	 */
	vxfloat3* GetVerticeDefinition(EnumVXRVertexType eVertexType){
		map<EnumVXRVertexType, vxfloat3*>::iterator itrVtxDef = mapVerticeDefinitions.find(eVertexType);
		if(itrVtxDef == mapVerticeDefinitions.end())
			return NULL;
		return itrVtxDef->second;
	}
	/*!
	 * @fn void vxobjects::SVXPrimitiveDataArchive::ReplaceOrAddVerticeDefinition(EnumVXRVertexType eVertexType, vxfloat3* pf3Vertex)
	 * @brief mapVerticeDefinitions �� value�� ���ǵ� pointer ��ȯ�ϴ� method, 
	 * @param eVertexType [in] \n EnumVXRVertexType \n vertex buffer�� �̸�, (mapVerticeDefinitions�� key)
	 * @param pf3Vertex [in] \n vxfloat3* \n vertex buffer�� �����ϴ� vxfloat3�� ������.
	 * @remarks EnumVXRVertexType �� Ű�� ���� ������ �̹� ��ϵ� vertex definition �� ���� ���, \n
	 * �ش� definition �� vertex pointer �� �޸𸮿��� �����ϰ� ���ο� vertex pointer �� ���
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
	 * @brief ��ϵ� vertex definition �� ������ ����
	 * @return int \n ��ϵ� vertex definition �� ����
	 */
	int GetNumVertexDefinitions()
	{
		return (int)mapVerticeDefinitions.size();
	}
	/*!
	 * @fn void vxobjects::SVXPrimitiveDataArchive::ClearVertexDefinitionContainer()
	 * @brief mapVerticeDefinitions �� clear ��. 
	 * @remarks ��ϵ� vertex pointer �� ���� �޸� ������ ���� �ʰ� container �� clear ��.
	 */
	void ClearVertexDefinitionContainer()
	{
		mapVerticeDefinitions.clear();
	}
};

/**
 * @class SVXTransferFunctionArchive
 * @brief VXFramework���� �����ϴ� OTF�� ���� �� ������ ���� �ڷᱸ��
 * @sa vxobjects::CVXTObject
 */
struct SVXTransferFunctionArchive{
	/**
	 * @brief OTF array �� pointer
	 * @details 
	 * 1D : [0][0 to (i3DimSize.x - 1)] - default, [1][0 to (i3DimSize.x - 1)] - summed preintegration
	 * 2D : [0][0 to (i3DimSize.x*i3DimSize.y - 1)] - default, [...][0 to (i3DimSize.x*i3DimSize.y - 1)] - customized
	 * 3D : [0 to (i3DimSize.z - 1)][0 to (i3DimSize.x*i3DimSize.y - 1)]
	 */
	void** ppvArchiveTF;
	/**
	 * @brief OTF array �� pointer dimension
	 * @details iNumDims = 1 or 2 or 3
	 */
	int iNumDims;	
	/**
	 * @brief OTF array �� �Ҵ�Ǿ� �ִ� ������ dimension�� ���� OTF array �� �� �ּҰ�
	 * @details
	 * i3ValidMinIndex.x : 1st dimension �� array �� �ּҰ� \n
	 * i3ValidMinIndex.y : 2nd dimension �� array �� �ּҰ� \n
	 * i3ValidMinIndex.z : 3rd dimension �� array �� �ּҰ�
	 */
	vxint3 i3ValidMinIndex;
	/**
	 * @brief OTF array �� �Ҵ�Ǿ� �ִ� ������ dimension�� ���� OTF array �� �� �ִ밪
	 * @details
	 * i3ValidMaxIndex.x : 1st dimension �� array �� �ִ밪 \n
	 * i3ValidMaxIndex.y : 2nd dimension �� array �� �ִ밪 \n
	 * i3ValidMaxIndex.z : 3rd dimension �� array �� �ִ밪
	 */
	vxint3 i3ValidMaxIndex;
	/**
	 * @brief OTF array �� �� dimension �� ���� ũ��
	 * @details 
	 * i3DimSizes.x : 1st dimension �� array ũ�� \n
	 * i3DimSizes.y : 2nd dimension �� array ũ�� \n
	 * i3DimSizes.z : 3rd dimension �� array ũ�� \n
	 * Valid dimension�� ���Ͽ� i3DimSizes.xyz > 0, Invalid demension�� ���Ͽ� i3DimSizes.xyz <= 0
	 */
	vxint3 i3DimSize;
	/**
	 * @brief OTF metric�� ������ �Ǵ� Volume ���� ������ ���� bin ũ��
	 * @par ex.
	 * 16 bit Volume Data�� ���� 2D OTF (density, gradient magnitude), ������ ������ 0~65535 ��� ������ �� \n
	 * 512x1024 ũ���� 2D OTF �� �����ϸ�, bin �� XY ũ��� (65536/512, 65536/1024) �� ��.
	 */
	vxdouble3 d3BinSize;
	/**
	 * @brief OTF array ���� ���� data type
	 */
	EnumVXRDataType eValueDataType;
	/**
	 * @brief constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
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
	 * @brief OTF array �� ���� ������ ppvArchiveTF �� �Ҵ�� memory ����
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
 * @brief block ����� volume�� ���� �ڷᱸ��
 * @sa vxobjects::CVXVObjectVolume, vxobjects::SVXVolumeDataArchive
 */
struct SVXVolumeBlock{
	/**
	 * @brief block �� �ּ� �ִ� ���� ���� ���� channel data type
	 * @details �Ϲ������� vxobjects::SVXVolumeDataArchive::eVolumeDataType �� ����
	 */
	EnumVXRDataType eVolumeMinMaxDataType;
	/**
	 * @brief ���� block�� ũ��
	 * @details extra boundary�� ���Ե��� ���� ũ�� \n
	 * i3BlockSize = vxint3(size of x, size of y, size of z)
	 */
	vxint3 i3BlockSize;
	/**
	 * @brief �ϳ��� SVXVolumeBlock �� �� �� �������� ���ǵ� block���� ����
	 * @details ��� block�� ���� = i3BlocksNumber.x * i3BlocksNumber.y * i3BlocksNumber.z;
	 */
	vxint3 i3BlocksNumber;
	/**
	 * @brief block ����� volume�� �����ϴ� ppvVolumeBricks �� �Ҵ�� extra boundary �� ���ʸ� ũ��
	 * @details 
	 * xyz �������� ��� ���� 
	 */
	int iBrickExtraSize;
	/**
	 * @brief block����� volume�� ������ 2D array
	 * @details 
	 * ppvVolumeBricks[block id][xyz sample position in a block] \n
	 * ���� block�� ���� �Ҵ�� x �� ���� ũ�� = i3BlockSize.x + iBrickExtraSize*2 \n
	 * ���� block�� ���� �Ҵ�� y �� ���� ũ�� = i3BlockSize.y + iBrickExtraSize*2 \n
	 * ���� block�� ���� �Ҵ�� z �� ���� ũ�� = i3BlockSize.z + iBrickExtraSize*2 
	 * @par ex. 
	 * 8x8x8 block�� ������ ���ǵ� 512x512x512 volume (ushort) ���� OS�� (100, 100, 100) ��ǥ�� sampling \n 
	 * �� ��� i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
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
	 * @brief block ������ �ּ� �ִ밪�� �����ϴ� 1D array
	 * @details 
	 * array�� ũ��� block�� ��ü ������ ����, extra boundary�� ������� ���� \n
	 * data type�� 2 channel�� ���� volume type�� ����. x : �ּҰ�, y : �ִ밪
	 * @par ex. 
	 * 8x8x8 block�� ������ ���ǵ� 512x512x512 volume (ushort) ���� OS�� (100, 100, 100) ��ǥ�� �ش��ϴ� block�� �ּ� �ִ밪 \n 
	 * �� ��� i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
	 *
	 * @par
	 * >> vxint3 i3BlockId = vxint3(floor(100/8), floor(100/8), floor(100/8)); \n
	 * >> int iBlockIdIndex = i3BlockId.x + i3BlockId.y*(i3BlocksNumber.x) + i3BlockId.y*(i3BlocksNumber.x*i3BlocksNumber.y); \n
	 * >> vxushort2 d2minmax = ((vxushort2*)pvMinMaxBlocks)[iBlockIdIndex];
	 */
	void* pvMinMaxBlocks;			
	/**
	 * @brief block ������ binary tag�� ������ 1D array
	 * @details 
	 * array�� ũ��� block�� ��ü ������ ����, extra boundary�� ������� ���� \n
	 * @par ex. 
	 * 8x8x8 block�� ������ ���ǵ� 512x512x512 volume (ushort) ���� OS�� (100, 100, 100) ��ǥ�� �ش��ϴ� block�� tag \n 
	 * �� ��� i3BlockSize = vxint3(8, 8, 8), i3BlocksNumber = (ceil(512/8), ceil(512/8), ceil(512/8)) \n
	 *
	 * @par
	 * >> vxint3 i3BlockId = vxint3(floor(100/8), floor(100/8), floor(100/8)); \n
	 * >> int iBlockIdIndex = i3BlockId.x + i3BlockId.y*(i3BlocksNumber.x) + i3BlockId.y*(i3BlocksNumber.x*i3BlocksNumber.y); \n
	 * >> bool bIsTagged = pbTaggedActivatedBlocks[iBlockIdIndex];
	 */
	bool* pbTaggedActivatedBlocks;	
	
	/**
	 * @brief block ��� volume�� data type
	 * @details �Ϲ������� vxobjects::SVXVolumeDataArchive::eVolumeDataType �� ����
	 */
	EnumVXRDataType eVolumeBrickDataType;
	/**
	 * @brief @ref vxobjects::SVXVolumeDataArchive::i3SizeExtraBoundary �� �ݿ��� ��ǥ�� ���� xyz�� block id table
	 * @details 
	 * block x id = piBlockIndexMap[0][0 to (SVXVolumeDataArchive::i3VolumeSize.x + SVXVolumeDataArchive::i3SizeExtraBoundary.x*2)] \n
	 * block y id = piBlockIndexMap[1][0 to (SVXVolumeDataArchive::i3VolumeSize.y + SVXVolumeDataArchive::i3SizeExtraBoundary.y*2)] \n
	 * block z id = piBlockIndexMap[2][0 to (SVXVolumeDataArchive::i3VolumeSize.z + SVXVolumeDataArchive::i3SizeExtraBoundary.z*2)]
	 */
	int* piBlockIndexMap[3];

	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
	SVXVolumeBlock(){ 
		VXSTRUCTURENULLSET(i3BlockSize); VXSTRUCTURENULLSET(i3BlocksNumber);
		pvMinMaxBlocks = NULL; ppvVolumeBricks = NULL; pbTaggedActivatedBlocks = NULL; 
		piBlockIndexMap[0] = NULL; piBlockIndexMap[1] = NULL; piBlockIndexMap[2] = NULL;
		iBrickExtraSize = 0;
		eVolumeBrickDataType = eVolumeMinMaxDataType = vxrDataTypeUNDEFINED;
	}

	/*!
	 * @fn void vxobjects::SVXVolumeBlock::Delete()
	 * @brief �Ҵ�� memory ��� ����
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
 * @brief VXFramework���� �����ϴ� frame buffer�� ���� �� ������ ���� �ڷᱸ��
 * @sa vxobjects::CVXIObject
 */
struct SVXFrameBuffer{
	/**
	 * @brief frame buffer �� width
	 */
	int iWidth;
	/**
	 * @brief frame buffer �� height
	 */
	int iHeight;
	/**
	 * @brief array�� ���ǵ� frame buffer
	 */
	void* pvBuffer;
	/**
	 * @brief frame buffer�� data type
	 */
	EnumVXRDataType ePixelDataType;
	/**
	 * @brief frame buffer�� �� pixel�� ���� (data type) ũ�� (bytes)
	 */
	uint uiSizeBytesPerPixel;
	/**
	 * @brief frame buffer�� ��� �뵵
	 * @details eFrameBufferUsage == vxrFrameBufferUsageRENDEROUT �� ���, �ݵ�� vxrDataTypeBYTE4CHANNEL �� ������.
	 */
	EnumVXRFrameBufferUsage eFrameBufferUsage;
	/**
	 * @brief frame buffer�� ���� mention
	 */
	wstring strHelpMent;

#ifdef __WINDOWS
	/**
	 * @brief win32���� file memory�� ���� buffer interoperation�� ���� handle
	 */
	HANDLE hFileMap;
#endif

	/// constructor, ��� 0 (NULL or false)���� �ʱ�ȭ
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
	 * @brief �Ҵ�� memory ��� ����
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
 * @brief VXFramework���� �����ϴ� progress �� ���� �ڷᱸ��
 */
struct SVXLocalProgress{
	/**
	 * @brief progress�� ����, 0.0 ���� 100.0 ����
	 */
	double dStartProgress;
	/**
	 * @brief progress�� ����, 0.0 ���� 100.0 ����
	 */
	double dRangeProgress;
	/**
	 * @brief progress�� ���� ���� ������ ��ϵǴ� module/function ���� static parameter�� ���� ������
	 */
	double* pdProgressOfCurWork; /*out*/
	/// constructor, �ʱ�ȭ
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
 * @brief VXObject�� �ֻ��� class �� VXObject family�� ���� parameter�� ����
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
	 * @brief VXObject�� content�� ���ǵǾ� �ִ°��� Ȯ��
	 * @remarks content�� VXObject�� ��� �޴� ������ VXObject���� ���ǵ�
	 * @li @ref vxobjects::CVXIObject
	 * @li @ref vxobjects::CVXLObject
	 * @li @ref vxobjects::CVXTObject
	 * @li @ref vxobjects::CVXVObjectVolume
	 * @li @ref vxobjects::CVXVObjectPrimitive
	 */
	bool IsDefinedResource();
	/*!
	 * @brief VXObject�� visibility�� ����
	 * @param bVisibility [in] \n bool \n true �� visible, false �� hidden
	 * @remarks customized purpose
	 */
	void SetVisibility(bool bVisibility);
	/*!
	 * @brief VXObject�� visibility�� ����
	 * @param bVisibility [in] \n bool \n boolean visibility
	 * @return bool \n visible �̸� true, hidden �̸� false ��ȯ
	 * @remarks customized purpose
	 */
	bool GetVisibility() const;
	/*!
	 * @brief VXObject�� Object ID ����
	 * @param iObjectID [in] \n int \n 32 bit ID
	 * @remarks 
	 * �Ϲ������� Resource Manager (@ref CVXResourceManager) ���� ���� \n
	 * [8bit : Object Type][8bit : Magic Bits][16 bit : Count-based ID]
	 */
	void SetObjectID(int iObjectID);
	/*!
	 * @brief VXObject�� Object ID�� ����
	 * @return int \n Object ID�� ��ȯ
	 */
	int GetObjectID() const;
	/*!
	 * @brief VXObject�� �����ϴµ� ���� ���� ������ ���� VXObject�� ID�� ����
	 * @param iReferenceObjectID [in] \n int \n VXObject�� �����ϴµ� ���� ���� ������ ���� VXObject�� ID
	 * @remarks default ID�� 0���� �����Ǿ� ����
	 */
	void SetReferenceObjectID(int iReferenceObjectID);
	/*!
	 * @brief VXObject�� �����ϴµ� ���� ���� ������ ���� VXObject�� ID�� ����
	 * @return int \n VXObject�� �����ϴµ� ���� ���� ������ ���� VXObject�� ID
	 * @remarks ������ VXObject�� ID�� �����Ǿ� ���� ���� ��� 0�� ��ȯ
	 */
	int GetReferenceObjectID() const;
	/*!
	 * @brief VXObject�� ���� ����� mention�� ����
	 * @param strMent [in] \n wstring \n ������ VXObject�� ���� ����� mention
	 */
	void SetHelptMent(wstring strMent);
	/*!
	 * @brief VXObject�� ���� ����� mention�� ����
	 * @return wstring \n VXObject�� ���� ����� mention ��ȯ
	 */
	wstring GetHelptMent() const;
	/*!
	 * @brief VXObject�� ������ module���� ������ �����
	 * @param strDefinedByModuleSpecifier [in] \n wstring \n VXObject�� ������ module���� mention
	 */
	void SetDefineModuleSpecifier(wstring strDefinedByModuleSpecifier);
	/*!
	 * @brief VXObject�� ������ module���� ������ ����� mention�� ����
	 * @return wstring \n VXObject�� ������ module���� ������ ����� mention ��ȯ
	 */
	wstring GetDefineModuleSpecifier();
	
	/*!
	 * @brief ���ǵ� VXObject�� type�� ���� 
	 * @return EnumVXRObjectType \n ���ǵ� VXObject�� type ��ȯ
	 * @remarks VXObject�� ���ǵǾ� �ִ� ���¸� �̰Ϳ� ���� VXObject�� instance�� ������
	 */
	EnumVXRObjectType GetObjectType();

	// Custom Parameter
	/*!
	 * @brief VXObject�� ������ ���ڿ��� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param strData [in] \n wstring \n ������ ���ڿ�
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, wstring strData);
	/*!
	 * @brief VXObject�� ������ double�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param dData [in] \n double \n ������ double�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, double dData);
	/*!
	 * @brief VXObject�� ������ vxdouble2�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param d2Data [in] \n vxdouble2 \n ������ vxdouble2�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble2 d2Data);
	/*!
	 * @brief VXObject�� ������ vxdouble3�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param d3Data [in] \n vxdouble3 \n ������ vxdouble3�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble3 d3Data);
	/*!
	 * @brief VXObject�� ������ vxdouble4�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param d4Data [in] \n vxdouble4 \n ������ vxdouble4�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxdouble4 d4Data);
	/*!
	 * @brief VXObject�� ������ int�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param iData [in] \n int \n ������ int�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, int iData);
	/*!
	 * @brief VXObject�� ������ vxint2�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param i2Data [in] \n vxint2 \n ������ vxint2�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint2 i2Data);
	/*!
	 * @brief VXObject�� ������ vxint3�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param i3Data [in] \n vxint3 \n ������ vxint3�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint3 i3Data);
	/*!
	 * @brief VXObject�� ������ vxint4�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param i4Data [in] \n vxint4 \n ������ vxint4�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxint4 i4Data);
	/*!
	 * @brief VXObject�� ������ bool�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param bData [in] \n bool \n ������ bool�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, bool bData);
	/*!
	 * @brief VXObject�� ������ vxmatrix�� ���� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param matData [in] \n vxmatrix \n ������ vxmatrix�� ��
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �׿��� Ư�� �������� �����ص� ����.
	 */
	bool RegisterCustomParameter(wstring strStandardParameterName, vxmatrix matData);
	/*!
	 * @brief VXObject�� ������ �����͸� �����
	 * @param strStandardParameterName [in] \n wstring \n ���Ƿ� ������ data�� ���� key name
	 * @param pData [in] \n void \n ������ ������ void���� ������
	 * @return bool \n ����� �����ϸ� true, �׷��� ������ false
	 * @remarks 
	 * �Ϲ������� key ���ڿ��� @ref vxhelpers::VXHStringGetParameterFromCustomStringMap ���� �����ϴ� �������� ��\n
	 * �����Ͱ� ����Ű�� �ּҸ� �����ϸ� �ش� �����Ϳ� �Ҵ�� memory�� ���� ��� �̸� ���������� ����.
	 */
	bool GetCustomParameter(wstring strStandardParameterName, void* pData/*out*/) const; 

	// Static Helper Functions //
	/*!
	 * @brief VXObject ID �����κ��� object type�� ��ȯ�ϴ� static helper �Լ�
	 * @param iObjectID [in] \n int \n VXObject�� ID
	 * @return EnumVXRObjectType \n VXObject�� ID�� ���ڵ��Ǿ� �ִ� object type
	 * @remarks 
	 * VXObject ID ���Ŀ� ���ؼ� ����.\n
	 * VXObject Magic Key�� ���� ���� ���� ::: ���� ��ġ
	 */
	static EnumVXRObjectType GetObjectTypeFromID(int iObjectID);
	/*!
	 * @brief VXObject ID �����κ��� object type�� VXVObject�ΰ��� Ȯ���ϴ� static helper �Լ�
	 * @param iObjectID [in] \n int \n VXObject�� ID
	 * @return bool \n VXVObject�̸� true, �׷��� ������ false ��ȯ
	 * @remarks 
	 * VXObject ID ���Ŀ� ���ؼ� ����.\n
	 * VXObject Magic Key�� ���� ���� ���� ::: ���� ��ġ
	 */
	static bool IsVObjectType(int iObjectID);
};

struct SVXVirtualArchiveVObject;
/**
 * @class CVXVObject
 * @brief VXObject�� ��� ������ VXVObjectVolume�� VXVObjectPrimitive�� ���� ������ ���� ���� class
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
	 * @brief content�� �����ϴ� axis-aligned bounding box in OS �� ����
	 * @param psvxOrthoBox [out] \n SVXOrthoBox \n object space (OS) ������ axis-aligned bounding box�� ���ǵ� SVXOrthoBox�� ������
	 */
	void GetOrthoBoundingBox(SVXOrthoBox* psvxOrthoBox/*out*/);
	
	/*!
	 * @brief OS�� WS�� ��ġ�Ǵ� ���� ���ǵǾ������� Ȯ���ϴ� �Լ�
	 * @return bool \n ���ǵǾ� ������ true, �׷��� ������ false
	 */
	bool IsGeometryDefined();
	
	/*!
	 * @brief VXVObject�� customized ������ ����
	 * @param f4Color [in] \n vxfloat4 \n normalized color RGBA (xyzw)
	 * @remarks �ݵ�� �� �������� ���̴� ���� �ƴϸ�, VXVObject�� ����ȭ�ϴ� module���� ��� ó���ϴ°��� ����.
	 */
	void SetVObjectColor(vxfloat4 f4Color);
	/*!
	 * @brief VXVObject�� customized ������ ����
	 * @return vxfloat4 \n VXVObject�� customized ������ ��ȯ
	 */
	vxfloat4 GetVObjectColor() const;

	// Object Origin Resource //
	/*!
	 * @brief VXVObject�� content�� ���簡 �ƴ� �ٸ� VXObject�� source�� reference�� ���� �� �ش� VXObject�� ID�� ����
	 * @return int \n source VXObject�� ID�� ��ȯ
	 */
	int GetSourceVObjectID() const;
	
	// Transform //
	/*!
	 * @brief VXVObject�� OS�� WS���� ȸ�� ��ȯ��
	 * @param pf3VecRotationAxis [in] \n vxfloat3 \n �ش� space������ ȸ������ �����ϰ� �ִ� normalized vector�� ����� vxfloat3�� ������
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n ���� ��ȯ�� �̷���� space
	 * @param fAngleDegree [in] \n float \n ȸ����, radian
	 * @remarks VXFramework�� RHS�� �⺻���� �����ϰ� ������, �Ϲ������� ȸ�� ������ ���� ���縦 ����. (�������� ������ ��ǥ�谡 RHS�� LHS�ΰ��� ����)
	 */
	void TransformRotation(vxfloat3* pf3VecRotationAxis, EnumVXVCoordSpace eCoordSpace, float fAngleDegree);
	/*!
	 * @brief VXVObject�� OS�� WS���� Ȯ��/��� ��ȯ��
	 * @param pf3ScaleFactor [in] \n vxfloat3 \n �ش� space������ ������� �������� �� scaling factor�� ����� vxfloat3�� ������
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n ���� ��ȯ�� �̷���� space
	 */
	void TransformScale(vxfloat3* pf3ScaleFactor, EnumVXVCoordSpace eCoordSpace);
	/*!
	 * @brief VXVObject�� OS�� WS���� �����̵� ��ȯ��
	 * @param pf3VecTranslationAxis [in] \n vxfloat3 \n �ش� space������ �̵� ������ �����ϴ� normalized vector�� ���ǵ� vxfloat3�� ������
	 * @param eCoordSpace [in] \n EnumVXVCoordSpace \n ���� ��ȯ�� �̷���� space
	 * @param fTranslationLength [in] \n float \n ���� ��ȯ�� �̷���� space������ �̵� ����
	 */
	void TransformTranslation(vxfloat3* pf3VecTranslationAxis, EnumVXVCoordSpace eCoordSpace, float fTranslationLength);
	/*!
	 * @brief VXVObject�� OS�� WS�� ��ȯ�� �����ϴ� matrix�� ����
	 * @param pmatOS2WS [in] \n vxmatrix \n OS�� WS�� ��ȯ�� �����ϴ� matrix�� �����ϰ� �ִ� vxmatrix�� ������
	 * @remarks ���������� OS�� WS ��ȯ�� ���õ� ��ǥ�谡 �缳���Ǹ� ���� matrix�� �缳����
	 */
	void SetTransformMatrixOS2WS(const vxmatrix* pmatOS2WS);
	/*!
	 * @brief VXVObject���� ���ǵǾ� �ִ� OS�� WS�� ��ȯ�� �����ϴ� matrix�� ����
	 * @return vxmatrix \n OS�� WS�� ��ȯ�� �����ϴ� matrix�� ���ǵ� vxmatrix�� ������
	 */
	vxmatrix* GetMatrixOS2WS();
	/*!
	 * @brief VXVObject���� ���ǵǾ� �ִ� WS�� OS�� ��ȯ�� �����ϴ� matrix�� ����
	 * @return vxmatrix \n WS�� OS�� ��ȯ�� �����ϴ� matrix�� ���ǵ� vxmatrix�� ������
	 */
	vxmatrix* GetMatrixWS2OS();
};

struct SVXVirtualArchiveVObjectVolume;
/**
 * @class CVXVObjectVolume
 * @brief VXVObject�� ���� OS�� WS�� ��ġ ���谡 ������ Volume�� ������ ���� class
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
	 * @brief volume ������ ���� �ִ� @ref vxobjects::SVXVolumeDataArchive �ڷ� ������ VXVObjectVolume�� �����
	 * @param psvxVolumeDataArchive [in] \n SVXVolumeDataArchive \n Volume ������ ���ǵ� SVXVolumeDataArchive �� ������
	 * @param i3BlockSizes[2] [in] \n vxint3 \n 
	 * volume�� �����ϴ� ���� block �ڷᱸ������ block ũ�⸦ �����ϰ� �ִ� ũ�� 2�� static array�� ���� vxint3�� ������ \n
	 * block ũ�Ⱑ �����Ǹ� ���������� block ������ �ּ�, �ִ밪 �ڷᱸ���� �����ǳ�, block ������ volume �籸�������� ���� \n
	 * i3BlockSizes[0] : ū block, i3BlockSizes[1] : ���� block \n
	 * NULL �̸� block ������ �� ��
	 * @param iSourceVolumeID [in] \n int \n 
	 * content�� pointer reference�� �ٸ� VXObjectVolume�� ������ �� �ش� VXObjectVolume�� ID \n
	 * �Ϲ������� copy method�� ���Ǹ�, pointer ������ �� �ϸ�, �� ��� 0�� ��. �⺻���� 0
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 * �Լ��� ����Ǵ� progress ������ �����ϴ� SVXLocalProgress�� ������ \n
	 * �⺻���� NULL�̸�, NULL�̸� ��� �� ��.
	 * @remarks i3BlockSizes�� �־����� ���������� @ref CVXVObjectVolume::GenerateVolumeMinMaxBlocks �� ȣ���, CVXVObjectVolume::GenerateBricks �� �Ҹ��� ����
	 */
	bool RegisterVolumeArchive(SVXVolumeDataArchive* psvxVolumeDataArchive, vxint3 i3BlockSizes[2]/* 0 : Large, 1: Small */, int iSourceVolumeID = 0, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief VXVObjectVolume�� ���ǵǾ� �ִ� volume ������ ����.
	 * @return SVXVolumeDataArchive \n volume ������ ����Ǿ� �ִ� SVXVolumeDataArchive �� ������
	 */
	SVXVolumeDataArchive* GetVolumeArchive();

	// Optional //
	/*!
	 * @brief volume �� ���� block ������ �ּ� �ִ밪�� ���������� �����Ͽ� @ref SVXVolumeBlock �ڷᱸ�� ���� �Ҵ��ϴ� �Լ�
	 * @param iLevel [in] \n int \n block �� level, 0 or 1
	 * @param i3BlockSize [in] \n vxint3 \n ���� block �� ũ��
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 �Լ��� ����Ǵ� progress ������ �����ϴ� SVXLocalProgress �� ������ \n
	 �⺻���� NULL�̸�, NULL�̸� ��� �� ��.
	 * @remarks 
	 * ������ �����Ǿ� �ִ� �ּ�, �ִ밪 block�� class�� ��ϵǾ� ������ ���� �� �ٽ� ��� \n
	 * @ref SVXVolumeBlock �� �ּ� �ִ밪�� ���� array�� ������
	 * CVXVObjectVolume::GenerateBricks �Լ� ȣ�� ���� �ҷ��� �ϴ� �Լ�
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void GenerateVolumeMinMaxBlocks(int iLevel, vxint3 i3BlockSize, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief volume �� block ������ �籸���Ͽ� @ref SVXVolumeBlock �ڷᱸ�� ���� �Ҵ��ϴ� �Լ�
	 * @param iLevel [in] \n int \n block �� level, 0 or 1
	 * @param iBrickExtraSize [in] \n vxint3 \n 
	 * block ������ volume�� �籸���� �� ����� ���ʸ鿡 ���� extra boundary�� ũ��, xyz�࿡ �����ϰ� ��� \n
	 * ���� ���� block�� ũ��� SVXVolumeBlock::i3BlockSize + vxint3(iBrickExtraSize, iBrickExtraSize, iBrickExtraSize) * 2 �� ����
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 �Լ��� ����Ǵ� progress ������ �����ϴ� SVXLocalProgress�� ������ \n
	 �⺻���� NULL�̸�, NULL�̸� ��� �� ��.
	 * @remarks 
	 * ������ �����Ǿ� �ִ� �ּ�, �ִ밪 block�� class�� ��ϵǾ� �־�� �� \n
	 * ��, CVXVObjectVolume::GenerateVolumeMinMaxBlocks �� ���� �ҷ��� ��. \n
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void GenerateBricks(int iLevel, int iBrickExtraSize, SVXLocalProgress* psvxLocalProgress = NULL);
	/*!
	 * @brief volume �� block �ڷᱸ���� ��� �Լ�
	 * @param iLevel [in] \n int \n block �� level, 0 or 1
	 * @return  SVXVolumeBlock \n 
	 * volume �� block �ڷᱸ���� ����� SVXVolumeBlock�� ������ \n
	 * volume �� block �ڷᱸ���� ���ǵǾ� ���� �ʰų� level ���� �߸� ���� NULL ��ȯ.
	 */
	SVXVolumeBlock* GetVolumeBlock(int iLevel);	// 0 or 1
	/*!
	 * @brief volume �� block �ڷᱸ������ �����ϴ� �ּ�, �ִ밪 ������ ���� �����ϰ� �ִ� block�� ���� tag�� update�ϴ� �Լ�
	 * @param iLevel [in] \n int \n block �� level, 0 or 1
	 * @param d2ActiveValueRangeMinMax [in] \n vxdouble2 \n ����� �ּ�(x), �ִ밪(y) \n
	 * @param psvxLocalProgress [out] \n SVXLocalProgress \n 
	 �Լ��� ����Ǵ� progress ������ �����ϴ� SVXLocalProgress�� ������ \n
	 �⺻���� NULL�̸�, NULL�̸� ��� �� ��.
	 * @remarks 
	 * ������ �����Ǿ� �ִ� �ּ�, �ִ밪 block�� class�� ��ϵǾ� �־�� �� \n
	 * @sa vxobjects::SVXVolumeBlock
	 */
	void UpdateTaggedActivatedBlocks(int iLevel, vxdouble2 d2ActiveValueRangeMinMax, SVXLocalProgress* psvxLocalProgress = NULL);

	// Static Helper Functions for Tagged Binary Access //
	/*!
	 * @brief tagged binary volume���� ���ǵ� array���� tagged binary ���� ��� static helper �Լ�
	 * @param ppiTaggedBinaryVolume [in] \n int \n tagged binary volume�� �����ϰ� �ִ� 2D array
	 * @param i3SamplePos [in] \n vxint3 \n volume�� OS ������ position
	 * @param iSampleXOffet [in] \n iSampleXOffet \n 
	 * x�� ���⿡ ���� extra boundary�� ���Ե� read offset ũ�� \n
	 * i3VolumeSize.x + i3SizeExtraBoundary.x * 2
	 * @remarks tagged binary volume�� int type���� bit encoding�Ǿ� ����
	 */
	inline static bool GetBitBinaryTag(int** ppiTaggedBinaryVolume, vxint3 i3SamplePos/*include extra boundary*/, int iSampleXOffet/*include extra boundary*/);
	/*!
	 * @brief tagged binary volume���� �ϳ��� z-slice�� ���� OS�� xy position�� encoding�� array�� iQuet�� iRem�� ���ϴ� static helper �Լ�
	 * @param i2SamplePosXY [in] \n vxint2 \n extra boundary�� ���Ե� volume�� sample index ��ǥ
	 * @param iSampleXOffet [in] \n iSampleXOffet \n 
	 * x�� ���⿡ ���� extra boundary�� ���Ե� read offset ũ�� \n
	 * i3VolumeSize.x + i3SizeExtraBoundary.x * 2
	 * @remarks tagged binary volume�� int type���� bit encoding�Ǿ� ����
	 */
	inline static vxint2 GetBitBinaryIndex(vxint2 i2SamplePosXY/*include extra boundary*/, int iSampleXOffet/*include extra boundary*/);
};

struct SVXVirtualArchiveTObject;
/**
 * @class CVXTObject
 * @brief VXObject�� ��� ������ volume�� visibility �Ӽ��� �����ϴ� OTF ���� ������ ���� class
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
	 * @brief OTF ������ ���� �ִ� @ref vxobjects::SVXTransferFunctionArchive �ڷ� ������ VXTObject�� �����
	 * @param psvxTransferFunctionArchive [in] \n SVXTransferFunctionArchive \n OTF ������ ���ǵ� SVXTransferFunctionArchive �� ������
	 * @param iReferenceObjectID [in] \n int \n 
	 * �Ϲ������� �ϳ��� VXTObject�� �ϳ��� VXVObjectVolume�� Ưȭ�Ǿ� �����Ǹ�, VXVObjectVolume�� ID \n
	 * VXVObjectVolume�� data type �� ����� volume �� ���� ������ ���� ��� ȥ���ص� ������ ����. \n
	 * VXVObjectVolume�� �������� �ʰ� ���������� ������ ��� 0�� ����.
	 */
	bool RegisterTransferFunctionArchive(SVXTransferFunctionArchive* psvxTransferFunctionArchive, int iReferenceObjectID);
	/*!
	 * @brief VXTObject�� ���ǵǾ� �ִ� OTF ������ ����.
	 * @return SVXVolumeDataArchive \n OTF ������ ����Ǿ� �ִ� SVXTransferFunctionArchive�� ������
	 */
	SVXTransferFunctionArchive* GetTransferFunctionArchive();
	
	// Static Helper Functions //
	/*!
	 * @brief SVXVolumeDataArchive �� ����Ǵ� OTF array�� �Ҵ��ϴ� static helper �Լ�
	 * @param iNumDims [in] \n int \n OTF dimension
	 * @param i3DimSize [in] \n vxint3 \n OTF dimension �� ũ��
	 * @param eValueDataType [in] \n EnumVXRDataType \n OTF array�� data type
	 * @param pppvArchiveTF [in] \n void \n 2D OTF array�� ���� void**�� ������ (3D ������)
	 * @remarks OTF array �� �׻� 2D OTF�� �����
	 * @sa vxobjects::SVXTransferFunctionArchive
	 */
	static void CreateArchive(int iNumDims, vxint3 i3DimSize, EnumVXRDataType eValueDataType, void*** pppvArchiveTF/*out*/);
};

/**
 * @class CVXCObject
 * @brief ī�޶� ���� ������ �ٷ�� VXIObject�� �ϳ��� instance�� ���Ե� class
 * @remarks 
 * �� class���� ���Ǵ� space�� ������ ����.
 * @li WS (World Space) : ī�޶� �� ��ü�� ��ġ�Ǵ� real world.
 * @li CS (Camera Space or Viewing Space) : ī�޶� ������ space, WS�� unit�� ���� \n
 * ���� : ī�޶� ��ġ, y�� : up vector, -z�� : viewing direction
 * @li PS (Projection Space) : CS���� ���ǵ� view frustum�� ���θ� �������� normalized cube������ frustum���� ���ǵ� space \n
 * ���� : near plane�� viewing direction�� ������ ��.\n
 * y �� z ���� CS�� ����� ������, ���̴� view frustum���� ���ǵǴ� ���̰� 1�� normalized �ǰԲ� scaling ��.
 * @li SS (Screen Space or Window Space) : buffer pixel�� �����ϴ� space \n
 * ���� : PS ���� normalized view frustum ���� z = 0�� plane�� ������� \n
 * x�� : PS ���� x�� ����� ����, y�� : PS ���� -y�� ����, z�� : PS ���� -z�� ���� \n
 * xy scaling : screen or window�� �����ϴ� buffer�� �ػ� ũ�� \n
 * z scaling : 1 (�� PS�� z���� ��ȣ�� �ݴ�)
 * @remarks image plane�� near plane �� ���ǵ�.
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
	 * @brief constructor, ī�޶� �ʱ�ȭ�ϱ� ���� parameter �ʿ�
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS�� axis-aligned �� cube�� scene stage�� ���ǵ� SVXOrthoBox �� ������
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n �־��� scene stage ���� ��� ��ġ�� �������� camera�� �����ϴ°��� ����
	 * @param pi2SizeScreen [in](optional) \n vxint2 \n 
	 * SS�� �����ϱ� ���� screen �� �ػ󵵰� ���ǵ� vxint2 �� ������ \n
	 * �⺻���� NULL�̸�, �� ��� �ػ󵵴� width = 1, height = 1�� ������
	 */
	CVXCObject(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType, const vxint2* pi2SizeScreen = NULL);
	~CVXCObject();
	
	/*!
	 * @brief ī�޶� scene stage (or view stage) �������� �����ϴ� �Լ�
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS�� axis-aligned �� cube�� scene stage�� ���ǵ� SVXOrthoBox �� ������
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n �־��� scene stage ���� ��� ��ġ�� �������� camera�� �����ϴ°��� ����
	 * @remarks WS, CS, PS �� ��ȯ matrix�� �ٽ� ����.
	 */
 	void SetViewStage(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType);
 
	/*!
	 * @brief CS ���� PS �� ��ȯ (projection) �Ǵ� ���¸� ����
	 * @param bPerspective [in] \n bool \n true �� perspective projection�� �Ǹ�, false �� orthogonal projection�� ��.
	 * @param pfFov [in] \n float \n 
	 * perspective projection �� up vector ���� ���� field of view�� angle (radian) �� ���ǵ� float ������ \n
	 * NULL�̸� ������ ������ field of view ���� ���.
	 * @remarks field of view �� �ʱⰪ�� PI/4 �� �����Ǿ� ����.
	 */
 	void SetPerspectiveViewingState(bool bPerspective, float* pfFov = NULL); 
	/*!
	 * @brief perspective projection �� ���¸� ��� �Լ�
	 * @param pfFov [out](optional) \n float \n ���� ������ field of view (radian) ���� ����� float�� ������
	 * @return bool \n perspective projection �̸� true ��ȯ, orthogonal projection �̸� false ��ȯ
	 * perspective projection �� up vector ���� ���� field of view�� angle (radian) �� ���ǵ� float ������ \n
	 * @remarks field of view �� �ʱⰪ�� PI/4 �� �����Ǿ� ����.
	 */
 	bool GetPerspectiveViewingState(float* pfFov/*out*/);
 
 	// Image Plane is defined as Near Plane from Camera
	/*!
	 * @brief viewing state�� ���� CS, PS, SS �� ��ȯ matrix�� �缳���ϴ� �Լ�
	 * @param pf2ImagePlaneSize [in] \n vxfloat2 \n WS(or CS) ���� near plane�� width(x), height(y)�� ����� vxfloat2 �� ������
	 * @param pfAspectWdivByH [in] \n float \n WS(or CS) ���� up vector ���� field of view�� ���ǵ� float �� ������
	 * @param pfNearPlaneDistFromCamera [in] \n float \n WS(or CS) ���� camera���� near plane ������ �Ÿ��� ���ǵ� float �� ������
	 * @param pfFarPlaneDistFromCamera [in] \n float \n WS(or CS) ���� camera���� far plane ������ �Ÿ��� ���ǵ� float �� ������
	 * @param pi2ScreenSize [in] \n vxint2 \n SS ���� screen ũ��(width(x), height(y))�� ���ǵ� vxint2 �� ������. 
	 * @param pf2FittingSize [in] \n vxfloat2 \n Orthogonal Projection���� Camera ���� ���� ratio �� ������Ű�� ���� Fitting ������ ����� vxfloat2�� ������
	 * @param pfFittingFovY [in] \n float \n Perspective Projection���� Camera ���� ���� ratio �� ������Ű�� ���� Fitting ������ ����� vxfloat2�� ������
	 * @remarks WS to CS, CS to PS, PS to SS �� ���� ��ȯ matrix �� ������ ���� inverse matrix�� �ٽ� ����.
	 */
 	void SetDefaultViewingState(vxfloat2* pf2ImagePlaneSize, float* pfAspectWdivByH, float* pfNearPlaneDistFromCamera, float* pfFarPlaneDistFromCamera, vxint2* pi2ScreenSize, vxfloat2* pf2FittingSize = NULL, float* pfFittingFovY = NULL);
	/*!
	 * @brief ���� CS, PS, SS �� ��ȯ matrix�� �����ϴ� viewing state�� ��� �Լ�
	 * @param pf2ImagePlaneSize [out] \n vxfloat2 \n WS(or CS) ���� near plane�� width(x), height(y)�� ����� vxfloat2 �� ������
	 * @param pfAspectWdivByH [out] \n float \n WS(or CS) ���� up vector ���� field of view�� ����� float �� ������
	 * @param pfNearPlaneDistFromCamera [out] \n float \n WS(or CS) ���� camera���� near plane ������ �Ÿ��� ����� float �� ������
	 * @param pfFarPlaneDistFromCamera [out] \n float \n WS(or CS) ���� camera���� far plane ������ �Ÿ��� ����� float �� ������
	 * @param pi2ScreenSize [out] \n vxint2 \n SS ���� screen ũ��(width(x), height(y))�� ����� vxint2 �� ������
	 * @param pf2FittingSize [out] \n vxfloat2 \n Orthogonal Projection���� Camera ���� ���� ratio �� ������Ű�� ���� Fitting ������ ����� vxfloat2�� ������
	 * @param pfFittingFovY [out] \n float \n Perspective Projection���� Camera ���� ���� ratio �� ������Ű�� ���� Fitting ������ ����� vxfloat2�� ������
	 * @remarks ��� ���� ���� parameter�� ���� NULL�� ������ �ش� parameter�� ���� ���� �� ��.
	 */
 	void GetDefaultViewingState(vxfloat2* pf2ImagePlaneSize/*out*/, float* pfNearPlaneDistFromCamera/*out*/, float* pfFarPlaneDistFromCamera/*out*/, vxint2* pi2ScreenSize/*out*/, vxfloat2* pf2FittingSize = NULL/*out*/, float* pfFittingFovY = NULL/*out*/);
	/*!
	 * @brief WS �󿡼� ���ǵ� ī�޶� state�� �����ϴ� �Լ�
	 * @param pf3PosCamera [in] \n vxfloat3 \n WS ���� ī�޶� ��ġ�� ���ǵ� vxfloat3 �� ������
	 * @param pf3VecView [in] \n vxfloat3 \n WS ���� viewing direction�� ���ǵ� vxfloat3 �� ������
	 * @param pf3VecUp [in] \n vxfloat3 \n WS ���� up vector�� ���ǵ� float�� ������
	 * @remarks WS �� CS �� ��ȯ�� �����ϴ� matrix�� �ٽ� ����.
	 */
 	void SetCameraState(vxfloat3* pf3PosCamera, vxfloat3* pf3VecView, vxfloat3* pf3VecUp);
	/*!
	 * @brief WS ���� ī�޶� ������ ��� �Լ�
	 * @param pf3PosCamera [in] \n vxfloat3 \n WS ���� ī�޶� ��ġ�� ����� vxfloat3 �� ������
	 * @param pf3VecView [in] \n vxfloat3 \n WS ���� viewing direction�� ����� vxfloat3 �� ������
	 * @param pf3VecUp [in] \n vxfloat3 \n WS ���� up vector�� ����� float�� ������
	 */
 	void GetCameraState(vxfloat3* pf3PosCamera/*out*/, vxfloat3* pf3VecView/*out*/, vxfloat3* pf3VecUp/*out*/);
	/*!
	 * @brief WS �󿡼� ���ǵ� ī�޶� zoom ���¸� 1.0 (100% ����) ���� �����ϴ� up vector ������ field of view �� �����ϴ� �Լ�
	 * @param fFittingFovY [in] \n float \n WS ���� up vector ������ field of view
	 */
	void SetFittingFovY(float fFittingFovY);
	/*!
	 * @brief WS �󿡼� ���ǵ� ī�޶� zoom ���¸� 1.0 (100% ����) ���� �����ϴ� up vector ������ field of view �� ��� �Լ�
	 * @param pfFittingFovY [out] \n float \n WS ���� up vector ������ field of view�� ����� float �� ������
	 */
	void GetFittingFovY(float* pfFittingFovY/*out*/);
	/*!
	 * @brief perspective projection �� ���θ� ��� �Լ�
	 * @return bool \n perspective projection �̸� true ��ȯ, orthogonal projection �̸� false ��ȯ
	 */
 	bool IsPerspective();
 
	/*!
	 * @brief WS ���� SS ���� ��ȯ�� �����ϴ� matrix�� ����
	 * @param psvxMatrixWS2CS [out] \n vxmatrix \n WS ���� CS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @param psvxMatrixCS2PS [out] \n vxmatrix \n CS ���� PS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @param psvxMatrixPS2SS [out] \n vxmatrix \n PS ���� SS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @remarks ��� ���� ���� parameter�� ���� NULL�� ������ �ش� parameter�� ���� ���� �� ��.
	 */
 	void GetMatricesWStoSS(vxmatrix* psvxMatrixWS2CS/*out*/, vxmatrix* psvxMatrixCS2PS/*out*/, vxmatrix* psvxMatrixPS2SS/*out*/);
	/*!
	 * @brief SS ���� WS ���� ��ȯ�� �����ϴ� matrix�� ����
	 * @param psvxMatrixSS2PS [out] \n vxmatrix \n SS ���� PS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @param psvxMatrixPS2CS [out] \n vxmatrix \n PS ���� CS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @param psvxMatrixCS2WS [out] \n vxmatrix \n CS ���� WS�� ��ȯ�ϴ� matrix�� ����� vxmatrix �� ������
	 * @remarks ��� ���� ���� parameter�� ���� NULL�� ������ �ش� parameter�� ���� ���� �� ��.
	 */
	void GetMatricesSStoWS(vxmatrix* psvxMatrixSS2PS/*out*/, vxmatrix* psvxMatrixPS2CS/*out*/, vxmatrix* psvxMatrixCS2WS/*out*/);
	
	/*!
	 * @brief WS or CS or PS or SS ���� ���ǵǴ� image plane�� rectangle�� �����ϴ� �� ���� ����
	 * @param eCoordSapce [in] \n EnumVXVCoordSpace \n image plane�� ����� �ϴ� space
	 * @param pf3PosTopLeft [out] \n vxfloat3 \n eCoordSapce ���� ���ǵ� image plane�� rectangle�� ������� ���� ����� vxfloat3 �� ������
	 * @param pf3PosTopRight [out] \n vxfloat3 \n eCoordSapce ���� ���ǵ� image plane�� rectangle�� ������� ���� ����� vxfloat3 �� ������
	 * @param pf3PosBottomLeft [out] \n vxfloat3 \n eCoordSapce ���� ���ǵ� image plane�� rectangle�� �����ϴ� ���� ����� vxfloat3 �� ������
	 * @param pf3PosBottomRight [out] \n vxfloat3 \n eCoordSapce ���� ���ǵ� image plane�� rectangle�� �����ϴ� ���� ����� vxfloat3 �� ������
	 * @remarks 
	 * ��� ���� ���� parameter�� ���� NULL�� ������ �ش� parameter�� ���� ���� �� ��. \n
	 * OS �� ���ؼ� �������� ����.
	 */
	void GetImagePlaneRectPoints(EnumVXVCoordSpace eCoordSapce, vxfloat3* pf3PosTopLeft/*out*/, vxfloat3* pf3PosTopRight/*out*/, vxfloat3* pf3PosBottomLeft/*out*/, vxfloat3* pf3PosBottomRight/*out*/);
};

struct SVXVirtualArchiveIObject;
/**
 * @class CVXIObject
 * @brief image plane�� buffer �� �ٷ�� �ϳ��� VXCObject�� ���� VXObject�� ��� �޴� class
 * @remarks 
 * �ϳ��� �ػ� (width, height)�� ���� �پ��� �뵵�� ���� �������� image buffer (frame buffer)�� ���ǵ�. \n
 * image plane�� ����Ǵ� �ϳ��� ī�޶� ������ ���� CVXCObject �� instance�� ����.
 * @sa vxobjects::CVXObject, vxobjects::CVXCObject
 */
__vxstaticclass CVXIObject : public CVXObject
{
private:
protected:
	SVXVirtualArchiveIObject* m_pVAIO;

public:
	/*!
	 * @brief constructor, image plane �� mapping�Ǵ� frame buffer�� �����ϴ� �ػ� �ʿ�
	 * @param iWidthPix [in](optional) \n int \n �ػ� width (pixel), �⺻���� 0
	 * @param iHeightPix [in](optional) \n int \n �ػ� height (pixel), �⺻���� 0
	 * @remarks width �Ǵ� height �� 0 ���ϸ� frame buffer�� �����ϴ� �Լ� (@ref CVXIObject::ResizeFrameBuffer, @ref  CVXIObject::InsertFrameBuffer)�� ����
	 */
	CVXIObject(int iWidthPix = 0, int iHeightPix = 0);
	~CVXIObject();
	
	/*!
	 * @brief ���ǵǾ� �ִ� frame buffer�� ũ�⸦ �缳���ϴ� �Լ�
	 * @param iWidthPix [in] \n int \n �ػ� width (pixel), 1 �̻�
	 * @param iHeightPix [in] \n int \n �ػ� height (pixel), 1 �̻�
	 * @remarks 
	 * ������ ���ǵ� frame buffer�� memory���� ����  �� �־��� ũ��� �ٽ� �Ҵ�\n
	 * frame buffer�� ����Ǿ��� ���빰�� ��� ������ (�ٽ� ä��� module or function�� ȣ��Ǿ� ��)\n
	 * image plane�� pixel�� ���� x pitch�� y pitch�� ���ٰ� �����ϹǷ� width�� height�� ���� ratio�� �ٲ�\n
	 * ����, CVXCObject ���� �����ϴ� WS ���� image plane�� ������ �缳���ǰ�, ���õ� ��ȯ matrix�� �缳����.
	 */
	void ResizeFrameBuffer(int iWidthPix, int iHeightPix);
	/*!
	 * @brief ���ǵǾ� �ִ� frame buffer�� ������ ��� �Լ�
	 * @param pi2SizeScreen [out] \n vxint2 \n frame buffer�� �ػ󵵰� ����� vxint2 �� ������. width(x), height(y)
	 * @param piNumFrameBuffers [out](optional) \n int \n ���� ���ǵ� frame buffer�� ������ ����� int ������
	 * @param piBytesSizePerPixel [out](optional) \n int \n ���� ���ǵ� ��� frame buffer�� ���Ͽ� �ϳ��� pixel�� �����ϴ� type���� ũ������ ����� int�� ������
	 * @remarks ��� ���� ���� parameter�� ���� NULL�� ������ �ش� parameter�� ���� ���� �� ��.
	 */
	void GetFrameBufferInfo(vxint2* pi2SizeScreen/*out*/, int* piNumFrameBuffers = NULL/*out*/, int* piBytesSizePerPixel = NULL/*out*/);
	/*!
	 * @brief ���ǵǾ� �ִ� frame buffer�� ������ �����(array ����) @ref vxobjects::SVXFrameBuffer �� ��� �Լ�
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n ���ǵ� frame buffer�� �� �ش� usage�� buffer�� ����
	 * @param uiBufferIndex [in] \n uint \n �ش� usage�� frame buffer �� index ��°�� ���ǵǾ� �ִ� buffer�� ����
	 * @return SVXFrameBuffer \n frame buffer�� ������ �����(array ����) @ref vxobjects::SVXFrameBuffer �� ������
	 */
	SVXFrameBuffer* GetFrameBuffer(EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex);
	
	/*!
	 * @brief frame buffer�� �ϳ� �߰��ϴ� �Լ�
	 * @param ePixelDataType [in] \n EnumVXRDataType \n �߰��� frame buffer�� data type
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n �߰��� frame buffer�� usage
	 * @param strMent [in] \n wstring \n �߰��� frame buffer�� ���� mention
	 * @param uiSizeBytesPerPixel [in](optional) \n uint \n ePixelDataType == enums::vxrDataTypeSTRUCTURED �� ��쿡�� ���̸�, �� ���� ���� pixel�� ũ��(bytes)
	 * @remarks eFrameBufferUsage == vxenums::vxrFrameBufferUsageRENDEROUT �� ��� data type�� vxenums::vxrDataTypeBYTE4CHANNEL �̾�� ��.
	 */
	void InsertFrameBuffer(EnumVXRDataType ePixelDataType, EnumVXRFrameBufferUsage eFrameBufferUsage, wstring strMent, uint uiSizeBytesPerPixel = 0/*Only Used when vxrDataTypeSTRUCTURED as EnumVXRDataType*/);

	/*!
	 * @brief frame buffer�� �����ϴ� �Լ�
	 * @param ePixelDataType [in] \n EnumVXRDataType \n ������ frame buffer�� data type
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n ������ frame buffer�� usage
	 * @param uiBufferIndex [in] \n uint \n ������ frame buffer�� index
	 * @param strMent [in] \n wstring \n �߰��� frame buffer�� ���� mention
	 * @param uiSizeBytesPerPixel [in](optional) \n uint \n ePixelDataType == enums::vxrDataTypeSTRUCTURED �� ��쿡�� ���̸�, �� ���� ���� pixel�� ũ��(bytes)
	 * @return bool \n �ش� uiBufferIndex �� ���۰� ������ true, ������ false ��ȯ
	 * @remarks �ش� index �� ���۰� ePixelDataType �� �̹� ����Ǿ� ������ �ƹ� �۾� ���� true ��ȯ
	 */
	bool ReplaceFrameBuffer(EnumVXRDataType ePixelDataType, EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex, wstring strMent, uint uiSizeBytesPerPixel = 0/*Only Used when vxrDataTypeSTRUCTURED as EnumVXRDataType*/);

	/*!
	 * @brief frame buffer�� �����ϴ� �Լ�
	 * @param eFrameBufferUsage [in] \n EnumVXRFrameBufferUsage \n ������ frame buffer�� usage
	 * @param uiBufferIndex [in] \n uint \n �ش� usage (eFrameBufferUsage)�� frame buffer �� index ��°�� buffer�� ����
	 * @return �ش� frame buffer�� �����ϸ� ������ �����ϸ� true, �׷��� ������ false ��ȯ
	 * @remarks �ش� frame buffer(eFrameBufferUsage && uiBufferIndex)�� memory���� ������.
	 */
	bool DeleteFrameBuffer(EnumVXRFrameBufferUsage eFrameBufferUsage, uint uiBufferIndex);

	/*!
	 * @brief CVXCObject�� instance�� ����.
	 * @param psvxViewStage [in] \n SVXOrthoBox \n WS�� axis-aligned �� cube�� scene stage�� ���ǵ� SVXOrthoBox �� ������
	 * @param eStageViewType [in] \n EnumVXVStageViewType \n �־��� scene stage ���� ��� ��ġ�� �������� camera�� �����ϴ°��� ����
	 * @remarks 
	 * �̹� �����Ǿ� ������ �������� ����. \n
	 * instance ���� �� CVXIObject �� content �� ���ǵǾ� �־��(@ref vxobjects::CVXObject::IsDefinedResource) ��. (�� buffer�� �����Ǿ� �־�� ��)
	 * @sa vxobjects::CVXObject, vxobjects::CVXCObject
	 */
	void AttachCamera(const SVXOrthoBox* psvxViewStage, EnumVXVStageViewType eStageViewType);
	/*!
	 * @brief CVXCObject�� instance�� ��� �Լ�
	 * @return CVXCObject \n instance�� ���ǵǾ� �ִ� CVXCObject �� ������
	 */
	CVXCObject* GetCameraObject();

	/*!
	 * @brief vxenums::vxrFrameBufferUsageRENDEROUT �� ���ǵ� SVXFrameBuffer ���� ����� vector container�� ����
	 * @return ���ǵ� SVXFrameBuffer ���� ����� vector container�� ���� vector<SVXFrameBuffer> �� ������
	 * @remarks ���ǵ� frame buffer �� data type �� vxenums::vxrDataTypeBYTE4CHANNEL ��
	 */
	vector<SVXFrameBuffer>* GetRenderBufferPointerList();
	/*!
	 * @brief vxenums::vxrFrameBufferUsageDEPTH �� ���ǵ� SVXFrameBuffer ���� ����� vector container�� ����
	 * @return ���ǵ� SVXFrameBuffer ���� ����� vector container�� ���� vector<SVXFrameBuffer> �� ������
	 * @remarks ���ǵ� frame buffer �� data type �� vxenums::vxrDataTypeFLOAT ��
	 */
	vector<SVXFrameBuffer>* GetDepthBufferPointerList();
	/*!
	 * @brief vxenums::vxrFrameBufferUsageCUSTOM �� ���ǵ� SVXFrameBuffer ���� ����� vector container�� ����
	 * @return ���ǵ� SVXFrameBuffer ���� ����� vector container�� ���� vector<SVXFrameBuffer> �� ������
	 * @remarks ���ǵ� frame buffer �� data type �� ���� Ư���� ������ ����
	 */
	vector<SVXFrameBuffer>* GetCustomBufferPointerList();
};

struct SVXVirtualArchiveVObjectPrimitive;
/**
 * @class CVXVObjectPrimitive
 * @brief VXVObject�� ���� OS�� WS�� ��ġ ���谡 ������ Volume�� ������ ���� class.
 * @remarks 
 * ������ OS�� OS �� VOS �� �ΰ��� ������ ó��. \n
 * @li OS : SVXPrimitiveDataArchive ����Ǵ� primitive ��ǥ��
 * @li VOS : SVXPrimitiveDataArchive �� ���ǵǴ� ���� ��ü�� WS ��ǥ�� ������ ��ġ�Ǵ� ��ǥ�� \n
 * CVXVObject �� OS �� VOS �� �Ǹ�, OS/VOS/WS �� ��ȯ�� ����� ���ǿ� ���� \n
 * @remarks OS �� VOS ���� ��ü ��ȯ�� ���� SVXPrimitiveDataArchive �� ���ǵǴ� ���� ��ü�� WS �� ���� ����(affine transform)�� ��ġ �� ���� �� �� ����.
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
	 * @brief primitive�� ���ǵ� ��ü ������ ���� �ִ� @ref vxobjects::SVXPrimitiveDataArchive �ڷ� ������ VXVObjectPrimitive�� �����
	 * @param psvxPrimitiveDataArchive [in] \n SVXPrimitiveDataArchive \n primitive�� ���ǵ� ��ü ������ ����� SVXVolumeDataArchive �� ������
	 * @param psvxLocalProgress [out](optional) \n SVXLocalProgress \n 
	 * �Լ��� ����Ǵ� progress ������ �����ϴ� SVXLocalProgress�� ������ \n
	 * �⺻���� NULL�̸�, NULL�̸� ��� �� ��.
	 */
	bool RegisterPrimitiveArchive(SVXPrimitiveDataArchive* psvxPrimitiveDataArchive, SVXLocalProgress* psvxProgress = NULL);
	/*!
	 * @brief VXVObjectPrimitive�� ���ǵǾ� �ִ� primitive�� ���ǵ� ��ü ������ ����.
	 * @return SVXPrimitiveDataArchive \n primitive�� ���ǵ� ��ü ������ ����Ǿ� �ִ� SVXPrimitiveDataArchive �� ������
	 */
	SVXPrimitiveDataArchive* GetPrimitiveArchive();
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�Ǵ� ������ ����
	 * @return int \n SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�� ����
	 */
	int GetNumArrangementComponents();
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�Ǵ� ���¸� ����.
	 * @param iIndex [in] \n int \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�鿡 ���� index
	 * @param psvxMatrix [out] \n vxmatrix \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�� ������ vxmatrix�� ������, NULL �̸� ��� �� ��
	 * @param pf4Color [out] \n vxfloat4 \n ���� ��ü�� VOS �� ��ġ�� �� �ο��� normalized color RGBA ������ ������ vxfloat4�� ������, NULL �̸� ��� �� ��
	 * @param pbIsVisible [out] \n bool \n ���� ��ü�� VOS �� ��ġ�� �� �ο��� visibility�� ������ bool ������, true�� visible, false�� hidden, NULL �̸� ��� �� ��
	 * @param pbIsWireFame [out] \n bool \n ���� ��ü�� wireframe ���� �׷������� ���� bool ������, true�� wire, false�� solid, NULL �̸� ��� �� ��
	 * @return bool \n �ش� index�� ��ȯ�� �����ϸ� true, �׷��� ������ false ��ȯ
	 * @remarks ,
	 * ��ȯ�� RHS, row major matrix�� ���ǵ�.\n
	 * ���� ��ü�� VOS �� ��ġ�ϴ� ������ ���ο� list�� ���ǵǸ� zero count-based index�� ���е�.\n
	 * ex.[0][1][2] -> �ϳ� �߰� -> [0][1][2][3] -> [1] ���� -> [0][2->1][3->2] => [0][1][2]
	 */
	bool GetPrimitiveArrangementComponent(int iIndex, vxmatrix* psvxMatrix/*out*/, vxfloat4* pf4Color/*out*/, bool* pbIsVisible/*out*/, bool* pbIsWireFame/*out*/);
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�ϴ� ����(��ȯ matrix �� ����)�� �߰�
	 * @param psvxMatrix [in] \n vxmatrix \n SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�ϴ� ��ȯ�� ����� vxmatrix�� ������
	 * @param pf4Color [in] \n vxfloat4 \n SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�� �� �ο��� normalized color RGBA ������ ������ vxfloat4�� ������
	 * @return bool \n ���������� �߰��Ǹ� true, �׷��� ������ false ��ȯ
	 * @remarks 
	 * ��ȯ�� RHS, row major matrix�� ���ǵ�.\n
	 * ���� ��ü�� VOS �� ��ġ�ϴ� ������ ���ο� list�� ���ǵǸ� zero count-based index�� ���е�.\n
	 * ex.[0][1][2] -> �ϳ� �߰� -> [0][1][2][3] -> [1] ���� -> [0][2->1][3->2] => [0][1][2]
	 */
	bool InsertPrimitiveArrangement(const vxmatrix* psvxMatrix, const vxfloat4* pf4Color);
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�Ǵ� ���¸� ����.
	 * @param iIndex [in] \n int \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�鿡 ���� index
	 * @param psvxMatrix [in] \n vxmatrix \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�� ������ vxmatrix�� ������, NULL �̸� ���� ���� ����
	 * @param pf4Color [in] \n vxfloat4 \n ���� ��ü�� VOS �� ��ġ�� �� �ο��� normalized color RGBA ������ ������ vxfloat4�� ������, NULL �̸� ���� ���� ����
	 * @param bIsVisible [in] \n bool \n ���� ��ü�� VOS �� ��ġ�� �� �ο��� visibility�� ����, true�� visible, false�� hidden
	 * @param bIsWireFrame [in] \n bool \n ���� ��ü�� wireframe ���� �׷������� ����, true�� wire, false�� solid
	 * @return bool \n �ش� index�� ��ȯ�� �����ϸ� true, �׷��� ������ false ��ȯ
	 * @remarks 
	 * ��ȯ�� RHS, row major matrix�� ���ǵ�.\n
	 */
	bool ModifyPrimitiveArrangement(int iIndex, const vxmatrix* psvxMatrix, const vxfloat4* pf4Color, bool bIsVisible, bool bIsWireFrame);
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� VOS �� ��ġ�Ǵ� ���¸� ����.
	 * @param iIndex [in] \n int \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�鿡 ���� index
	 * @return bool \n �ش� index�� ��ȯ�� ���� �����ϸ� true, �׷��� ������ false ��ȯ
	 * @remarks 
	 * ��ȯ�� RHS, row major matrix�� ���ǵ�.\n
	 * ���� ��ü�� VOS �� ��ġ�ϴ� ������ ���ο� list�� ���ǵǸ� zero count-based index�� ���е�.\n
	 * ex.[0][1][2] -> �ϳ� �߰� -> [0][1][2][3] -> [1] ���� -> [0][2->1][3->2] => [0][1][2]
	 */
	bool RemovePrimitiveArrangement(int iIndex);
	/*!
	 * @brief SVXPrimitiveDataArchive �� ���ǵ� ���� ��ü�� WS�� ��ġ(OS->VOS->WS)�� ���� pitch ������ ����
	 * @param pf3PitchOS2WS [out] \n vxfloat3 \n ���� ��ü�� WS�� ��ġ(OS->VOS->WS)�� ���� pitch ������ ����� vxfloat3 �� ������
	 * @param iIndex [in] \n int \n ���� ��ü�� VOS �� ��ġ�Ǵ� ��ȯ�鿡 ���� index
	 * @return bool \n pitch ������ ���������� ������ true, �׷��� ������ false ��ȯ
	 */
	bool GetPrimitiveArrangementComponentPitch(vxfloat3* pf3PitchOS2WS/*out*/, int iIndex);

	// Helper
	/*!
	 * @brief scale �� translation�� �����ϴ� ��ȯ matrix�� �����ϴ� static helper �Լ�
	 * @param psvxMatrix [out] \n vxmatrix \n ���� ��ȯ matrix�� ������ vxmatrix�� ������
	 * @param f3Scale1st [in] \n vxfloat3 \n scale factor
	 * @param f3Translation2nd [in] \n vxfloat3 \n translation factor
	 * @remarks OS �������� ��Ī�� ������ü�� VOS �� ��ġ�ϴ� matrix�� ���� �� ���
	 */
	static void ComputeSymmetricPrimitiveArrangementMatrix(vxmatrix* psvxMatrix/*out*/, vxfloat3 f3Scale1st, vxfloat3 f3Translation2nd);
	/*!
	 * @brief source line �� destination line ���� ��ȯ�ϴ� matrix�� �����ϴ� static helper �Լ�
	 * @param psvxMatrix [out] \n vxmatrix \n ���� ��ȯ matrix�� ������ vxmatrix�� ������
	 * @param f3PosSrc[2] [in] \n vxfloat3 \n source line�� �����ϴ� ũ�� 2�� static array, f3PosSrc[0] : ������, f3PosSrc[1] : ��
	 * @param f3PosDest[2] [in] \n vxfloat3 \n destination line�� �����ϴ� ũ�� 2�� static array, f3PosDest[0] : ������, f3PosDest[1] : ��
	 * @par ex. (0, 0, 0), (0, 0, 1) �� �����ϰ� �ִ� line �� (10, 10, 10), (20, 20, 20) ���� �����ϴ� line���� ��ȯ�ϴ� matrix
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
 * @brief CVXObject�� ��� �޴� customized list ������ ���� class.
 * @remarks 
 * VXFramework���� �����ϴ� VXObject �ڷ� �������� ���� resource manager�� �����ϴ� �ڷᱸ���� ����ϱ� ���� ���� \n
 * ���������� (key, value)�� map container�� �����Ǿ� ������ key�� ������ "_vlist_[type string]_[Name]" ���� ���� \n
 * value�� vector<data type>�� vector container�� �����Ǿ� ����.
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
	 * @brief ��ϵǾ� �ִ� ��� list�� ��� ����
	 * @remarks 
	 */
	void ClearAll();
	/*!
	 * @brief �ش� ���ڿ��� key name�� ���� list�� ����
	 * @param strListName [in] \n wstring \n key name�� ���ڿ�
	 * @return bool \n �ش� ���ڿ��� key name�� ���� list�� ���� �����ϸ� true, �׷��� ������ false ��ȯ.
	 */
	bool ClearList(wstring strListName);

	/*!
	 * @brief �ش� ���ڿ��� key name�� ���� list�� ����
	 * @param strListName [in] \n wstring \n 
	 * key name�� ���ڿ�, key�� ������ "_vlist_[type string or VOIDPTR]_[Name]" \n
	 * [type string]�� VXFramework���� ������ ���ڿ�(@ref vxhelpers::VXHGetDataTypeFromString)�� ����. \n
	 * ex. "_vlist_FLOAT3_customname", "_vlist_VOIDPTR_customname", ...
	 * @param ppvtrList [in] \n void* \n �ش� key�� �����ϴ� list�� �����ϴ� vector<data type>�� ������ �ּҸ� ���� void�� ���� ������
	 * @return bool \n �ش� ���ڿ��� key name�� ���� list�� ��µ� �����ϸ� true, �׷��� ������ false ��ȯ.
	 * @remarks 
	 * "_vlist_[type string]_[Name]" ������ ������ type string ���� user defined ���ڿ��� key �� �� �� ������ pointer ���� user defined pointer�� ����� ���� ����.\n
	 * �� ���, user defined pointer �� resource �� �Ҵ�Ǿ� ���� ��, �̸� ���������� ����.
	 * user defined pointer �� resource �� �����ϰ��� �� �� type string �� VOIDPTR �� �ؾ� �ϸ�, �� ��� ���������� resource ����
	 * @sa vxhelpers::VXHGetDataTypeFromString
	 */
	bool GetList(wstring strListName, void** ppvtrList/*out*/);
	/*!
	 * @brief �ش� ���ڿ��� key name�� ���� list�� �����
	 * @param strListName [in] \n wstring \n 
	 * key name�� ���ڿ�, key�� ������ "_vlist_[type string or VOIDPTR]_[Name]" \n
	 * [type string]�� VXFramework���� ������ ���ڿ�(@ref vxhelpers::VXHGetDataTypeFromString)�� ����. \n
	 * ex. "_vlist_FLOAT3_customname", "_vlist_VOIDPTR_customname", ...
	 * @param pvtrList [in] \n void \n 
	 * ������ list�� ���� void ������ \n
	 * �Ϲ������� vector<data type> �� list�� ����Ǿ� �ְ� �̰��� �����͸� parameter�� ���. \n
	 * user defined pointer�� ����� ���� ����.
	 * @return bool \n �ش� ���ڿ��� key name�� ���� list�� ��µ� �����ϸ� true, �׷��� ������ false ��ȯ.
	 * @remarks 
	 * "_vlist_[type string]_[Name]" ������ ������ type string ���� user defined ���ڿ��� key �� �� �� ������ pointer ���� user defined pointer�� ����� ���� ����.\n
	 * �� ���, user defined pointer �� resource �� �Ҵ�Ǿ� ���� ��, �̸� ���������� ����.
	 * user defined pointer �� resource �� �����ϰ��� �� �� type string �� VOIDPTR �� �ؾ� �ϸ�, �� ��� ���������� resource ����
	 * @sa vxhelpers::VXHGetDataTypeFromString
	 */
	bool RegisterList(wstring strListName, void* pvtrList);
	
	/*!
	 * @brief RegisterList ���� VOIDPTR Ÿ������ ��ϵ� �ڷᱸ���� ���� Ư���� resource ������ delete function �� �ʿ��� ��� \n
	 * �̿� ���õ� delegation function �� class �� �����ϸ� ������ key name �� �ڷᱸ���� ������ �� ȣ���
	 * @param strListName [in] \n wstring \n 
	 * key name�� ���ڿ�, key�� ������ "_vlist_VOIDPTR_[Name]" \n
	 * @param pvDelegate [in] \n void \n delegation function pointer
	 * @param pvFunctionClass [in] \n void \n delegation function �� �����ϰ� �ִ� class instance pointer \n
	 * �ش� class �� function ���� ��� resource �� ���� ���� �ʵ��� �Ͽ�, void* delete �� �ǵ��� ��
	 * @return bool \n �ش� ���ڿ��� key name�� ���� list�� ��µ� �����ϸ� true, �׷��� ������ false ��ȯ.
	 * @remarks 
	 * �Ϲ������� static function �� ����ϳ�, static memory ���� Ÿ�̹��� CVXLObject ���� Ÿ�ֺ̹��� ���� ��� �߻��ϴ� ������ �ذ��ϱ� ����.
	 * @sa CVXLObject::RegisterList
	 */
	bool RegisterDeleteDelegater(wstring strCallbackListName, void*pvDelegate);
	  
	/*!
	 * @brief LObject �� ���� �ִ� ��� List element �� ��
	 * @return ullong \n LObject �� ���� �ִ� ��� List element �� ���� byte �� ��ȯ
	 */
	ullong GetSizeOfAllLists();
};
}
using namespace vxobjects;

/**
 * @package vxhelpers
 * @brief VXFramework���� ���Ǵ� helper functions�� ���� namespace
 */
namespace vxhelpers
{
//=========================
// Global Helper Functions
//=========================
/*!
 * @fn __vxstatic bool VXHAllocatorVoidPointer1D(void** ppVoidTarget, int iSize, EnumVXRDataType eValueDataType, uint iUnitSizeBytes)
 * @brief ������ Data Type�� ���� 1D�� �޸𸮸� �Ҵ�
 * @param ppVoidTarget [out] \n void* @n �Ҵ�� �޸𸮿� ���� 1D ������
 * @param iSize [in] \n int \n �Ҵ��� �޸� ũ��. (bytes)
 * @param eValueDataType [in] \n EnumVXRDataType \n �Ҵ��� �޸𸮿� �ش� Data Type
 * @param iUnitSizeBytes [in](optional) \n uint \n Default ���� 0 �̸�, eValueDataType == vxrDataTypeSTRUCTURED �� ��츸 ������ Structure�� ũ��� ���
 * @return bool \n �޸� �Ҵ��� �����ϸ� true, �����ϸ� false ��ȯ
 * @remarks 
 * �Ҵ�� �޸� �����Ϳ� ���� ���ų� �б� ���ؼ� Data Type �� ���� Casting �� �ʿ�. \n
 * vxrDataTypeSTRING �� �������� ����. \n
 * eValueDataType != vxrDataTypeSTRUCTURED �� ��� iUnitSizeBytes �Ķ���ʹ� ������ ����
*/
__vxstatic bool VXHAllocatorVoidPointer1D(void** ppVoidTarget, int iSize, EnumVXRDataType eValueDataType, uint iUnitSizeBytes = 0);

/*!
 * @fn __vxstatic bool vxhelpers::VXHAllocatorVoidPointer2D(void*** pppVoidTarget, int iSize1, int iSize2, EnumVXRDataType eValueDataType, uint iUnitSizeBytes)
 * @brief ������ Data Type�� ���� 2D�� �޸𸮸� �Ҵ�
 * @param pppVoidTarget [out] \n void** @n �Ҵ�� �޸𸮿� ���� 2D ������
 * @param iSize1 [in] \n int \n void** �� �Ҵ��� �޸� ũ��. (bytes), y dimension
 * @param iSize2 [in] \n int \n void* �� �Ҵ��� �޸� ũ��. (bytes), x dimension
 * @param eValueDataType [in] \n EnumVXRDataType \n �Ҵ��� �޸𸮿� �ش� Data Type
 * @param iUnitSizeBytes [in](optional) \n uint \n Default ���� 0 �̸�, eValueDataType == vxrDataTypeSTRUCTURED �� ��츸 ������ Structure�� ũ��� ���
 * @return bool \n �޸� �Ҵ��� �����ϸ� true, �����ϸ� false ��ȯ
 * @remarks 
 * �Ҵ�� �޸� �����Ϳ� ���� ���ų� �б� ���ؼ� Data Type �� ���� Casting �� �ʿ�. \n
 * vxrDataTypeSTRING �� �������� ����. \n
 * eValueDataType != vxrDataTypeSTRUCTURED �� ��� iUnitSizeBytes �Ķ���ʹ� ������ ���� \n
 * �Ҵ�� �޸� �����Ϳ� ���� (x, y) �� indexing �� (*pppVoidTarget)[y][x]���� ����� ��
*/
__vxstatic bool VXHAllocatorVoidPointer2D(void*** pppVoidTarget, int iSize1, int iSize2, EnumVXRDataType eValueDataType, uint iUnitSizeBytes = 0);

/*!
 * @fn __vxstatic void vxhelpers::VXHGetSystemMemoryInfo(ullong* pullFreeMemoryBytes, ullong* pullAvailablePhysicalMemoryBytes)
 * @brief ���� �����ǰ� �ִ� OS�� ���� System Memory ���¸� ����
 * @param pullFreeMemoryBytes [out] \n ullong \n ���� ��� ������ �޸� ũ�� (bytes)
 * @param pullAvailablePhysicalMemoryBytes [out] \n ullong \n ���� System �� �νĵǴ� ���� �޸� ũ�� (bytes)
 * @remarks x86 �Ǵ� x64, ���� ���� OS�� ���¿� ���� ���� �޸𸮿� �ٸ��� ���� �� ����.
*/
__vxstatic void VXHGetSystemMemoryInfo(ullong* pullFreeMemoryBytes, ullong* pullAvailablePhysicalMemoryBytes);

/*!
 * @fn __vxstatic ullong vxhelpers::VXHGetCurrentTimePack()
 * @brief ���� �ð��� ���� ������ 64bit���� ���ڵ��Ͽ� ����
 * @return ullong \n  0~9 bit : milli-seconds , 10~15 bit : second, 16~21 bit : minute, 22~26 bit : hour,  27~31 bit : day, 32~35 bit : month, 36~65 bit : year
 * @remarks 
*/
__vxstatic ullong VXHGetCurrentTimePack();

/*!
 * @fn __vxstatic int vxhelpers::VXHGetDataTypeSizeByte(EnumVXRDataType eValueDataType)
 * @brief EnumVXRDataType ���� ���ǵǴ� Data Type�� ���� ���� bytes ũ�⸦ ����
 * @param eValueDataType [in] \n EnumVXRDataType \n Target Data Type
 * @return int \n Data Type�� ũ�� (bytes)
 * @remarks vxrDataTypeUNDEFINED, vxrDataTypeSTRUCTURED, vxrDataTypeSTRING �� ��� 0�� ��ȯ
*/
__vxstatic int VXHGetDataTypeSizeByte(EnumVXRDataType eValueDataType);

/*!
 * @fn __vxstatic EnumVXRDataType vxhelpers::VXHGetDataTypeFromString(wstring strDataType)
 * @brief VXFramework���� ������ ���ڿ��� Data Type���� ����
 * @param strDataType [in] \n wstring \n Data Type�� �����ϴ� ���ڿ�
 * @return EnumVXRDataType \n VXFramework���� ������ ���ڿ��� �ٸ� ���ڿ��� ��� ���� �� vxrDataTypeUNDEFINED ��ȯ
 * @remarks 
 * @par VXFramework���� ������ ���ڿ� :
 * "CHAR", "BYTE", "SHORT", "USHORT", "INT", "INT3", "UINT", "FLOAT", "DOUBLE",  \n
 * "BYTE4", "FLOAT2", "FLOAT3", "FLOAT4", "DOUBLE2", "DOUBLE3", "DOUBLE4", "STRING"
*/
__vxstatic EnumVXRDataType VXHGetDataTypeFromString(wstring strDataType);

// VolumeArchive Helper //
/*!
 * @fn __vxstatic void vxhelpers::VXHVolumeFillBoundaryWithMinValue(SVXVolumeDataArchive* psvxVolumeArchive, bool bIsClampAxisZ, SVXLocalProgress* psvxProgress)
 * @brief SVXVolumeDataArchive ���� ���ǵǴ� Extra Boundary ���� ������ Volume�� �ּҰ��� ä��
 * @param psvxVolumeArchive [in-out] \n 
 * SVXVolumeDataArchive \n Volume�� ���ǵǾ� �ִ� �ڷᱸ�� SVXVolumeDataArchive�� ���� ������ \n
 * psvxVolumeArchive->ppvVolumeSlices �� Extra Boundary ������ ���� ä��.
 * @param bIsClampAxisZ [in] \n bool \n Volume�� z �� ���⿡ ���� Extra Boundary �� ���� Clamp ����(��谪���� ����)���� ä��� ���� ����
 * @param psvxProgress [in](optional)  
 * SVXLocalProgress \n ���� ���� ������ ó���ϴ� �ڷᱸ�� SVXLocalProgress�� ���� ������ \n
 * Default�� NULL �̸�, �� ��� ���� ������ ó������ �ʰ� �Լ� ����
 * @remarks 
 * psvxVolumeArchive ���� Volume �� ���� �� �Ҵ�Ǿ� �־�� ��. \n
 * bIsClampAxisZ �� false �̸� Extra Boundary ������ ������ �ּҰ����� ä��
*/
__vxstatic void VXHVolumeFillBoundaryWithMinValue(SVXVolumeDataArchive* psvxVolumeArchive, bool bIsClampAxisZ = false, SVXLocalProgress* psvxProgress = NULL);

/*!
 * @fn __vxstatic void vxhelpers::VXHVolumeFillHistogramBasedOnVolumeValues(SVXVolumeDataArchive* psvxVolumeArchive, SVXLocalProgress* psvxProgress)
 * @brief SVXVolumeDataArchive ���� ���ǵǴ� ������ ���� Histogram�� ����
 * @param psvxVolumeArchive [in-out] \n 
 * SVXVolumeDataArchive \n Volume�� ���ǵǾ� �ִ� �ڷᱸ�� SVXVolumeDataArchive�� ���� ������ \n
 * psvxVolumeArchive->pullHistogram �� �޸� �Ҵ� �� Histogram ����
 * @param psvxProgress [in](optional) \n 
 * SVXLocalProgress \n ���� ���� ������ ó���ϴ� �ڷᱸ�� SVXLocalProgress�� ���� ������ \n
 * Default�� NULL �̸�, �� ��� ���� ������ ó������ �ʰ� �Լ� ����
 * @remarks 
 * ������ ���� �� ���ǵǾ� ������ ���� ���� ���� �� �ٽ� ���� �� ���� \n
 * Histogram�� Array(psvxVolumeArchive->pullHistogram) ũ��� uint(d2MinMaxValue.y - d2MinMaxValue.x + 1.5)���� ����
*/ 
__vxstatic void VXHVolumeFillHistogramBasedOnVolumeValues(SVXVolumeDataArchive* psvxVolumeArchive, SVXLocalProgress* psvxProgress = NULL);

/*!
 * @fn __vxstatic void VXHVolumeGetMinMaxValue(vxdouble2* pd2MinMaxValue, void** ppvVolume, EnumVXRDataType eDataType, vxint3 i3SizeVolume, vxint3 i3SizeExtraBoundary, SVXLocalProgress* psvxProgress)
 * @brief ������ �ּ� �ִ밪�� ����
 * @param pd2MinMaxValue [out] \n vxdouble2 \n pd2MinMaxValue->x : �ּҰ�, pd2MinMaxValue->y : �ִ밪
 * @param ppvVolume [in] \n void** @n ������ �Ҵ�Ǿ� �ִ� 2D Pointer
 * @param eDataType [in] \n EnumVXRDataType \n ������ Data Type
 * @param i3SizeVolume [in] \n vxint3 \n ������ ũ�� i3SizeVolume.xyz = (size X, size Y, size Z)
 * @param i3SizeExtraBoundary [in] \n vxint3 \n ������ Extra Boundary ũ�� i3SizeExtraBoundary.xyz = (size X, size Y, size Z), ���ʸ�
 * @param psvxProgress [in](optional) \n 
 * SVXLocalProgress \n ���� ���� ������ ó���ϴ� �ڷᱸ�� SVXLocalProgress�� ���� ������ \n
 * Default�� NULL �̸�, �� ��� ���� ������ ó������ �ʰ� �Լ� ����
 * @remarks �Լ� ������ �����ϸ� pd2MinMaxValue�� ��ȿ���� ������� ����.
*/
__vxstatic void VXHVolumeGetMinMaxValue(vxdouble2* pd2MinMaxValue, void** ppvVolume, EnumVXRDataType eDataType, vxint3 i3SizeVolume, vxint3 i3SizeExtraBoundary, SVXLocalProgress* psvxProgress = NULL);

// PrimitiveArchive Helper //
/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Sphere(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCenter, double dRadius, int iNumIter)
 * @brief SVXPrimitiveDataArchive�� ���� ����
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive �ڷᱸ���� ���ǵǾ� �ִ� SVXVolumeDataArchive�� ���� ������
 * @param d3PosCenter [in] \n vxdouble3 \n ���� �߽��� ��ġ
 * @param dRadius [in] \n double \n ���� ������
 * @param iNumIter [in] \n int \n ���� �����ϴ� Polygon�� ���е��� ����, 0���� ū ���̾�� �ϸ� ���� 3 �Ǵ� 4�� ���� ���
 * @remarks 
 * sphere-approximating polygons based on surface refinement method \n
 * described in http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/sphere_cylinder/ by Paul Bourke (1992). \n
 * Given iNumIter, # of vertices:(4 ^ (iNumIter+1)) + 2, # of edges:(4 ^ (iNumIter+1)) * 3, # of triangles:(4 ^ iNumIter) * 8 \n
 * ������ psvxPrimitiveArchive�� Primitive�� �����Ǿ� ������ �� ��.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Sphere(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCenter, double dRadius, int iNumIter);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cone(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations)
 * @brief SVXPrimitiveDataArchive�� ��� ���� ���� �ִ� ������ ����
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive �ڷᱸ���� ���ǵǾ� �ִ� SVXVolumeDataArchive�� ���� ������
 * @param d3PosStart [in] \n vxdouble3 \n ������ �ؿ��� �߽��� ��ġ
 * @param d3PosEnd [in] \n vxdouble3 \n ������ ���� ������ ��ġ
 * @param dRadius [in] \n double \n ������ �ؿ��� ������
 * @param iNumInterpolations \n int \n ������ �����ϴ� Polygon�� ���е��� ����, 3�̻��� ���̾�� ��
 * @remarks 
 * �ؿ��� (360 / iNumInterpolations) ���� �ﰢ�� Polygon���� ���ǵǸ� �̰Ϳ� �����ϴ� ������ �ﰢ������ ������ ���� \n
 * ������ psvxPrimitiveArchive�� Primitive�� �����Ǿ� ������ �� ��.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cone(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cylinder(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, int iNumInterpolations)
 * @brief SVXPrimitiveDataArchive�� ������ �ؿ��� ���� �ִ� ������ ����
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive �ڷᱸ���� ���ǵǾ� �ִ� SVXVolumeDataArchive�� ���� ������
 * @param d3PosStart [in] \n vxdouble3 \n ������ �ؿ��� �߽��� ��ġ
 * @param d3PosEnd \n vxdouble3 \n ������ ������ �߽��� ��ġ
 * @param dRadius \n double \n ������ ������
 * @param bIsOpenCylinder \n bool \n ������ ���� �Ʒ����� �շ� �ִ°��� ����
 * @param iNumInterpolations \n int \n ������ �����ϴ� Polygon�� ���е��� ����, 3�̻��� ���̾�� ��
 * @remarks 
 * ������ ���� (360 / iNumInterpolations) ���� �ﰢ�� Polygon���� ���ǵǸ� �̰Ϳ� �����ϴ� ������ �ﰢ������ ������ ���� \n
 * ������ psvxPrimitiveArchive�� Primitive�� �����Ǿ� ������ �� ��.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cylinder(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd, double dRadius, bool bIsOpenCylinder, int iNumInterpolations);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Cube(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCubeMin, vxdouble3 d3PosCubeMax, double dEdgeNormalWeight, bool bIsOnlyOutline)
 * @brief SVXPrimitiveDataArchive�� ��ǥ��� align�� ����ü ����
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive �ڷᱸ���� ���ǵǾ� �ִ� SVXVolumeDataArchive�� ���� ������
 * @param d3PosCubeMin [in] \n vxdouble3 \n aligned axis ��ǥ�迡�� ����ü�� �ּ� ��ġ��
 * @param d3PosCubeMax [in] \n vxdouble3 \n aligned axis ��ǥ�迡�� ����ü�� �ִ� ��ġ��
 * @param dEdgeNormalWeight [in] \n double \n �������� normal vector�� ����ü�� ���� �����ϴ� vertex�� ������ �ִ� ����, 0.0 ~ 1.0 ������ ���̾�� ��
 * @param bIsOnlyOutline [in] \n bool \n true�� ��� polygon�� �ƴ� line primitive ����, false�� ��� polygon���� ���� �����ϰԲ� primitive ����
 * @remarks 
 * ����ü�� ���ǵǴ� OS(Object Space) ���� �� �࿡ ������ ����ü�� �� ���� min / max ������ d3PosCubeMin �� d3PosCubeMax ���� ���� \n
 * d3PosCubeMin.xyz < d3PosCubeMax.xyz �̾�� �� \n
 * ������ psvxPrimitiveArchive�� Primitive�� �����Ǿ� ������ �� ��.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Cube(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosCubeMin, vxdouble3 d3PosCubeMax, double dEdgeNormalWeight/*0.0 to 1.0*/, bool bIsOnlyOutline);

/*!
 * @fn __vxstatic void vxhelpers::VXHPrimitiveGenerateTemplate_Line(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd)
 * @brief SVXPrimitiveDataArchive�� line ����
 * @param psvxPrimitiveArchive [in-out] \n SVXPrimitiveDataArchive \n Primitive �ڷᱸ���� ���ǵǾ� �ִ� SVXVolumeDataArchive�� ���� ������
 * @param d3PosStart [in] \n vxdouble3 \n line�� ������ ��ġ
 * @param d3PosEnd [in] \n vxdouble3 \n line�� ���� ��ġ
 * @remarks ������ psvxPrimitiveArchive�� Primitive�� �����Ǿ� ������ �� ��.
*/
__vxstatic void VXHPrimitiveGenerateTemplate_Line(SVXPrimitiveDataArchive* psvxPrimitiveArchive, vxdouble3 d3PosStart, vxdouble3 d3PosEnd);

/*!
 * @fn __vxstatic void vxhelpers::VXHCheckValidGPUSDK()
 * @brief ���� ���� ȯ�濡���� ��ȿ�� GPU SDK�� vector list �� ������
 * @param strSDK [in] \n string \n "DX11", "CUDA", "OpenGL", "OpenCL" �� �ϳ� 
 * @return bool \n valid �ϸ� true, �׷��� ������ false
 * @remarks �ð��� �ణ �ɸ��Ƿ� �ǽð� ���꿡�� ������ ����ϸ� �� ��.
*/
__vxstatic bool VXHCheckValidGPUSDK(string strSDK);

/*!
 * @fn __vxstatic void vxhelpers::VXHComputeIntialAlignmentMatrixOS2WS()
 * @brief OS ���� ��� WS ���� ���� Object Bound Box �� �߽��� WS ���� �������� ��ġ��Ű�� ��ȯ ��� ���
 * @param pvxAlignAxisOS2WS [in] \n SVXAlignAxisOS2WS \n OS ���� xyz���� WS ���� xyz �࿡ mapping �Ǵ� ���踦 ���ϴ� SVXAlignAxisOS2WS �� �����ϰ� �ִ� ������
 * @param f3PitchInOS [in] \n vxfloat3 \n OS ���� ���� ���⿡ ���� 1 ������ �����ϴ� WS ���� ��ġ
 * @param svxOrthoBoxInOS [in] \n SVXOrthoBox \n OS ���� Object Bound Box
 * @param pmatInitOS2WS [out] \n vxmatrix \n ���Ǵ� ��ȯ ����� ������ ������
 * @return bool \n valid �ϸ� true, �׷��� ������ false
 * @remarks �ð��� �ణ �ɸ��Ƿ� �ǽð� ���꿡�� ������ ����ϸ� �� ��.
*/
__vxstatic bool VXHComputeIntialAlignmentMatrixOS2WS(vxmatrix* pmatInitOS2WS/*out*/, SVXAlignAxisOS2WS* pvxAlignAxisOS2WS, vxfloat3 f3PitchInOS, SVXOrthoBox svxOrthoBoxInOS);
//==========================================
// Module Parameter Structures : 2010.10.28
//==========================================
/*!
 * @fn __vxstatic int vxhelpers::VXHStringSplit(vector<wstring>* pvtrTokens, const wstring* pstrSrc, const wstring* pstrDelimiter)
 * @brief ���ڿ��� delimiter ���ڿ� �������� ����.
 * @param pvtrTokens [out] \n vector<wstring> \n ������ ���ڿ��� vector list �ڷᱸ���� ���� ������
 * @param pstrSrc [in] \n wstring \n source ���ڿ��� ���� ������
 * @param pstrDelimiter [in] \n wstring \n delimiter ���ڿ��� ���� ������
 * @return int \n ������ ���ڿ��� ����
 * @remarks �ϳ��� delimiter �� ���� ����.
*/
__vxstatic int VXHStringSplit(vector<wstring>* pvtrTokens, const wstring* pstrSrc, const wstring* pstrDelimiter);

/*!
 * @fn __vxstatic bool vxhelpers::VXHStringGetParameterFromCustomStringMap(void* pvParameter, map<wstring, wstring>* pmapCustomParameter, wstring strParameterName)
 * @brief map<wstring, wstring>���� ���ǵǴ� container���� VXFramework format�� custom parameter�� ��� �Լ�
 * @param pvParameter [out] \n void \n VXFramework format���� ���ǵ� custom parameter ���ڿ����� ���� ���� ���� ������
 * @param pmapCustomParameter [in] \n map<wstring, wstring> \n VXFramework format�� custom parameter�������� �ۼ��� map<wstring, wstring>�� ������
 * @param strParameterName [in] \n wstring \n VXFramework format�� custom parameter name�� ���ڿ�
 * @return bool \n strParameterName�� pmapCustomParameter�� �����ϰ� VXFramework format�� �´� ���ڿ��� ���� �����Ǿ� ������ true, �׷��� ������ false
 * @remarks 
 * map<wstring, wstring> container���� key�� ������ "_[data type]_[name]"���� content�� ������ "x,x,..."���� ������ \n
 * [data type]�� ���ڿ��� ������ ������ \n
 * @li bool, int, double, int2, int3, int4, double2, double3, double4, matrix44, string \n
 * content�� ���ڿ��� ������ ���Ͽ� \n
 * @li [bool] : "true" or "false", int/double : "number", >> bool type \n
 * @li [int2/int3/int4/double2/double3/double4] : "number, number, ..." channel ������ ��ġ���Ѿ� ��. >> vx[type] type \n
 * @li [matrix44] : "number, number, ..., number" �� 16��. �տ������� vxmatrix�� _m11, _m12, ..., _m43, _m44 >> vxmatrix type \n
 * @li [string] : "string" >> wstring type \n
 * ���� ������ ������, ���� ���Ŀ� ���� ����ؾ� �� \n
 * @details
 * @par ex. 
 * key:"_double2_customname", content:"2.0, 1.5"�� ����� container map�� ���Ͽ� : \n
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
 * @brief map< wstring, vector<CVXObject*> >���� ���ǵǴ� container���� VXFramework�� VXObject List�� ��� �Լ�
 * @param pvtrVXObjects [out] \n vector<CVXObject*> \n �Լ��� ����� VXFramework�� VXObject�� ���� vector list�� ���� ������
 * @param pmapVXObjects [in] \n map< wstring, vector<CVXObject*> > \n VXObject List�� VXFramework�� Module Parameter �������� �ۼ��� map<wstring, vector<CVXObject*>>�� ������
 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject�� type
 * @param bIsInput [in] \n bool \n true�̸� input���� ��� ���� VXObject List, false�̸� output���� ��� ���� VXObject List
 * @return int \n ������� VXObject List�� ���Ե� VXObjects ����
 * @remarks �ش��ϴ� VXObject�� ������ 0�� ��ȯ.
 *		\n map<wstring, vector<CVXObject*>>�� key�� ������ �������� ����Ǿ� ����
 *		\n _[in/out]_[VOLUME/PRIMITIVE/CUSTOMLIST/TRANSFERFUNCTION/IMAGEPLANE]_vector
 *		\n bIsInput�� true�� ��� _in_[...]_vector�� false�� ��� _out_[...]_vector�� �ش��ϴ� VXObject List�� ����.
*/
__vxstatic int VXHStringGetVXObjectListFromObjectStringMap(vector<CVXObject*>* pvtrVXObjects, map< wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput);

/*!
 * @fn __vxstatic CVXObject* vxhelpers::VXHStringGetVXObjectFromObjectStringMap(map<wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
 * @brief map< wstring, vector<CVXObject*> >���� ���ǵǴ� container���� VXFramework�� VXObject�� ��� �Լ�
 * @param pmapVXObjects [in] \n map< wstring, vector<CVXObject*> > \n VXObject List�� VXFramework�� Module Parameter �������� �ۼ��� map<wstring, vector<CVXObject*>>�� ������
 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject�� type
 * @param bIsInput [in] \n bool \n true�̸� input���� ��� ���� VXObject List, false�̸� output���� ��� ���� VXObject List
 * @param iIndex [in] \n int \n VXObject List���� ����� �ϴ� VXObject�� index
 * @return CVXObject \n ����� VXObject�� ������
 * @remarks 
 * ��ȯ�Ǵ� VXObject�� VXObject type�� ���� ���� VXFramework���� �����ϴ� VXObjects �� �ֻ��� CVXObject�� ��ȯ.\n
 * VXObject type�� �°� casting �ʿ�.
 * @details
 * @par ex.
 * >> CVXIObject* pCIObject = (CVXIObject*)vxhelpers::VXHStringGetVXObjectFromObjectStringMap(&mapVXObjects, vxenums::vxrObjectTypeIMAGEPLANE, false, 0);
*/
__vxstatic CVXObject* VXHStringGetVXObjectFromObjectStringMap(map< wstring, vector<CVXObject*> >* pmapVXObjects, EnumVXRObjectType eObjectType, bool bIsInput, int iIndex);

/*!
 * @fn __vxstatic void* vxhelpers::VXHStringGetCustomObjectFromPointerStringMap(map<wstring, void*>* pmapCustomObjects, wstring strParameterName)
 * @brief map<wstring, void*>���� ���ǵǴ� container���� VXFramework���� ���� Custom Pointer�� ��� �Լ�
 * @param pmapCustomObjects [in] \n map<wstring, void*> \n Custom Pointer�� content�� ����Ǿ� �ִ� map<wstring, void*>�� ������
 * @param strParameterName [in] \n wstring \n VXFramework format�� custom parameter name�� ���ڿ�
 * @return void \n ����� Custom Pointer, strParameterName�� �ش��ϴ� �����Ͱ� ���� ��� NULL ��ȯ
 * @remarks 
 * map<wstring, void*> container���� ������ ������ key ���ڿ��� ������ ����. \n
 * custom pointer�� platform �� module���� ������ ���ǵ� ��ü�� instance pointer�� �� ���� ������, \n
 * platform �� module�� interoperation�� ���� function callback�� ���� function pointer�� Ȱ��� �� ����. \n
*/
__vxstatic void* VXHStringGetCustomObjectFromPointerStringMap(map<wstring, void*>* pmapCustomObjects, wstring strParameterName);
}
using namespace vxhelpers;

namespace vxobjects
{
/*!
 * @class SVXModuleParameters
 * @brief Module�� �������� ���� input, output�� �����ϴ� �ڷᱸ��
 * @remarks ������ container�� Module ������ �ƴ� Ư���ϰ� ����� container �������� parameter�� ����� ���� ����
 * @sa 
 * vxhelpers::VXHStringGetVXObjectFromObjectStringMap \n 
 * vxhelpers::VXHStringGetParameterFromCustomStringMap \n
 * vxhelpers::VXHStringGetCustomObjectFromPointerStringMap
*/
struct SVXModuleParameters{
	/*!
	 * @brief Module�� ���� VXObject �鿡 ���� ID ���� ��ϵ� container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetVXObjectFromObjectStringMap �� ���� ID�� ���� �� �ֵ��� container�� ���� \n
	 * >> wstring key �� ���� : _[in/out]_[VOLUME/PRIMITIVE/IMAGEPLANE/TRANSFERFUNCTION/CUSTOMLIST]_vector
	 */
	map< wstring, vector<CVXObject*> > mapVXObjects;
	/*!
	 * @brief Module�� ���� VXFramework format���� ���ǵ� container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetParameterFromCustomStringMap �� ���� ���� ���� �� �ֵ��� container�� ���� \n
	 * >> wstring key �� Value �� ���� \n
	 * >> _[bool/int/double/int2/int3/int4/double2/double3/double4/matrix44/string]_[name] \n
	 * >> ex. Key : "_bool_IsTfChanged", Value : "true"
	 */
	map<wstring, wstring> mapCustomParamters;
	/*!
	 * @brief Module�� ���� custom pointer�� ���ǵ� container map
	 * @remarks 
	 * @ref vxhelpers::VXHStringGetCustomObjectFromPointerStringMap �� ���� �����͸� ���� �� �ֵ��� container�� ���� \n
	 * >> wstring key �� ���� \n
	 * >> _[in/out]_[class/vector/custom...]_[name] \n
	 * >> ex. "_in_class_GPUMANAGER"
	 */
	map<wstring, void*> mapCustomObjects;

	/*!
	 * @fn bool vxobjects::SVXModuleParameters::GetCustomParameter(void* pvParameter, wstring strParameterName)
	 * @brief map<wstring, wstring>���� ���ǵǴ� container���� VXFramework format�� custom parameter�� ��� �Լ�
	 * @param pvParameter [out] \n void \n VXFramework format���� ���ǵ� custom parameter ���ڿ����� ���� ���� ���� ������
	 * @param strParameterName [in] \n map<wstring, wstring> \n VXFramework format�� custom parameter�������� �ۼ��� map<wstring, wstring>�� ������
	 * @return bool \n strParameterName�� pmapCustomParameter�� �����ϰ� VXFramework format�� �´� ���ڿ��� ���� �����Ǿ� ������ true, �׷��� ������ false
	 * @remarks @ref vxhelpers::VXHStringGetParameterFromCustomStringMap �� �����ϰ� ����
	*/
	bool GetCustomParameter(void* pvParameter, wstring strParameterName)
	{
		return VXHStringGetParameterFromCustomStringMap(pvParameter, &mapCustomParamters, strParameterName);
	}
	/*!
	 * @fn int vxobjects::SVXModuleParameters::GetVXObjectList(vector<CVXObject*>* pvtrVXObjects, EnumVXRObjectType eObjectType, bool bIsInput)
	 * @brief map<wstring, vector<CVXObject*>>���� ���ǵǴ� container���� VXFramework�� VXObject List�� ��� �Լ�
	 * @param pvtrVXObjects [out] \n vector<CVXObject*> \n �Լ��� ����� VXFramework�� VXObject�� ���� vector list�� ���� ������
	 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject�� type
	 * @param bIsInput [in] \n bool \n true�̸� input���� ��� ���� VXObject List, false�̸� output���� ��� ���� VXObject List
	 * @return int \n ������� VXObject List�� ���Ե� VXObjects ����
	 * @remarks @ref vxhelpers::VXHStringGetVXObjectListFromObjectStringMap �� �����ϰ� ����
	*/
	int GetVXObjectList(vector<CVXObject*>* pvtrVXObjects, EnumVXRObjectType eObjectType, bool bIsInput)
	{
		return VXHStringGetVXObjectListFromObjectStringMap(pvtrVXObjects, &mapVXObjects, eObjectType, bIsInput);
	}
	/*!
	 * @fn CVXObject* vxobjects::SVXModuleParameters::GetVXObject(EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
	 * @brief map<wstring, vector<CVXObject*>>���� ���ǵǴ� container���� VXFramework�� VXObject�� ��� �Լ�
	 * @param eObjectType [in] \n EnumVXRObjectType \n VXObject�� type
	 * @param bIsInput [in] \n bool \n true�̸� input���� ��� ���� VXObject List, false�̸� output���� ��� ���� VXObject List
	 * @param iIndex [in] \n int \n VXObject List���� ����� �ϴ� VXObject�� index
	 * @return CVXObject \n ����� VXObject�� ������
	 * @remarks @ref vxhelpers::VXHStringGetVXObjectFromObjectStringMap �� �����ϰ� ����
	*/
	CVXObject* GetVXObject(EnumVXRObjectType eObjectType, bool bIsInput, int iIndex)
	{
		return VXHStringGetVXObjectFromObjectStringMap(&mapVXObjects, eObjectType, bIsInput, iIndex);
	}
	/*!
	 * @fn void* vxobjects::SVXModuleParameters::GetCustomObject(wstring strParameterName)
	 * @brief map<wstring, void*>���� ���ǵǴ� container���� VXFramework���� ���� Custom Pointer�� ��� �Լ�
	 * @param strParameterName [in] \n map<wstring, void*> \n Custom Pointer�� content�� ����Ǿ� �ִ� map<wstring, void*>�� ������
	 * @return void \n ����� Custom Pointer, strParameterName�� �ش��ϴ� �����Ͱ� ���� ��� NULL ��ȯ
	 * @remarks @ref vxhelpers::VXHStringGetCustomObjectFromPointerStringMap �� �����ϰ� ����
	*/
	void* GetCustomObject(wstring strParameterName)
	{
		return VXHStringGetCustomObjectFromPointerStringMap(&mapCustomObjects, strParameterName);
	}
};
}
