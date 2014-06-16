/*******************************************************************************

Copyright (C) 2006  Jess Telford

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*******************************************************************************/


// globals.h
// A necessary evil

#ifndef __globals
#define __globals

extern GlobStruct* g_pGlob;
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern HMODULE g_SetupDebug;

#define ARGBB16(n) (n & 0xF)
#define ARGBG16(n) ((n >> 4) & 0xF)
#define ARGBR16(n) ((n >> 8) & 0xF)
#define ARGBA16(n) ((n >> 12) & 0xF)

#define ARGBB32(n) (n & 0xFF)
#define ARGBG32(n) ((n >> 8) & 0xFF)
#define ARGBR32(n) ((n >> 16) & 0xFF)
#define ARGBA32(n) ((n >> 24) & 0xFF)


// Typedefs for accessing DBP functions
typedef UINT*				(*IpV)	( void );	// UINT* = (Void)
typedef void				(*VV)	( void );	// (Void)
typedef LPDIRECT3DDEVICE9	(*OV)	( void );	// D3D_Device = (Void)
typedef bool				(*BI)	( int );	// bool = (int)
typedef int					(*II)	( int );	// int = (int)
typedef LPDIRECT3DTEXTURE9	(*OI)	( int );	// IDirect3DTexture9* = (int)
typedef DWORD				(*DD)	( DWORD );	// DWORD = (DWORD)


// DBP Functions
extern IpV GetBackBufferPtr;
extern VV LockBackBuffer;
extern VV UnlockBackBuffer;
extern BI ImageExist;
extern OI GetImageTexture;
extern OV getD3DDevice;
extern DD GetInternalData;


struct sCamera
{
	D3DXMATRIX matProjection; // projection matrix
	D3DXMATRIX matView; // view matrix
	D3DXVECTOR3 vecLook; // look vector
	D3DXVECTOR3 vecUp; // up vector
	D3DXVECTOR3 vecRight; // right vector
	D3DXVECTOR3 vecPosition; // position vector
	D3DVIEWPORT9 viewPort2D; // view port for 2D ( area on screen to take up )
	D3DVIEWPORT9 viewPort3D; // view port for 3D ( area on screen to take up )

	int iCameraToImage;
	LPDIRECT3DTEXTURE9 pCameraToImageTexture;
	LPDIRECT3DSURFACE9 pCameraToImageSurface;
	LPDIRECT3DTEXTURE9 pCameraToImageAlphaTexture;
	LPDIRECT3DSURFACE9 pCameraToImageAlphaSurface;
	int iBackdropState; // used as an automatic clear
	DWORD dwBackdropColor;// color of backdrop

	// Can use XYZ rotation or FreeFlight(YPR)
	bool bUseFreeFlightRotation;
	D3DXMATRIX matFreeFlightRotate;
	float fXRotate, fYRotate, fZRotate; // rotateangle

	float fAspect; // aspect ratio
	float fAspectMod;
	float fFOV; // field of view
	float fZNear; // z near
	float fZFar; // z far
	bool bRotate; // which rotate to use

	// mike - 250903 - override camera
	bool bOverride;
	D3DXMATRIX matOverride;

	// leeadd - 310506 - u62 - added clipping planes (so can do reflection tricks)
	int iClipPlaneOn;
	D3DXPLANE planeClip;
};



#ifndef TPCCommand
#define TPCCommand __declspec ( dllexport )
#endif


// Export table
extern "C"{

	TPCCommand	void	RenderSIRD		( );
	TPCCommand	void	UpdateSIRD		( );
	TPCCommand	void	initSIRD		( int, int, int, float );
	TPCCommand	void	GetCameraNormal ( int, D3DVECTOR* );
}

#endif
