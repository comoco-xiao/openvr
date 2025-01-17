//========= Copyright Valve Corporation ============//
#pragma once

#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <vector>

/** returns true if the string has the prefix */
bool StringHasPrefix( const std::string & sString, const std::string & sPrefix );
bool StringHasPrefixCaseSensitive( const std::string & sString, const std::string & sPrefix );

/** returns if the string has the suffix */
bool StringHasSuffix( const std::string &sString, const std::string &sSuffix );
bool StringHasSuffixCaseSensitive( const std::string &sString, const std::string &sSuffix );

/** returns sModify with all instances of sFind replaced with sReplace */
std::string StringReplace( const std::string &sModify, const std::string &sFind, const std::string &sReplace );

/** converts a UTF-16 string to a UTF-8 string */
std::string UTF16to8( const wchar_t * in );
std::string UTF16to8( const std::wstring & in );

/** converts a UTF-8 string to a UTF-16 string */
std::wstring UTF8to16(const char * in);
std::wstring UTF8to16( const std::string & in );
#define Utf16FromUtf8 UTF8to16

#if defined( _WIN32 )
std::string DefaultACPtoUTF8( const char *pszStr );
#endif

/** Repairs a should-be-UTF-8 string to a for-sure-is-UTF-8 string, plus return boolean if we subbed in '?' somewhere */
bool RepairUTF8( const char *begin, const char *end, std::string & sOutputUtf8 );
bool RepairUTF8( const std::string & sInputUtf8, std::string & sOutputUtf8 );

/** Trims trailing CR, LF, Tab, and Space characters */
std::string TrimTrailingWhitespace( const std::string& in );

/** safely copy a string into a buffer */
void strcpy_safe( char *pchBuffer, size_t unBufferSizeBytes, const char *pchSource );
template< size_t bufferSize >
void strcpy_safe( char (& buffer) [ bufferSize ], const char *pchSource ) 
{
	strcpy_safe( buffer, bufferSize, pchSource );
}

/** Turns printf-style format args into a std::string */
std::string Format( const char *pchFormat, ... );


/** converts a string to upper case */
std::string StringToUpper( const std::string & sString );

/** converts a string to lower case */
std::string StringToLower( const std::string & sString );

// we stricmp (from WIN) but it isn't POSIX - OSX/LINUX have strcasecmp so just inline bridge to it
#if defined( OSX ) || defined( LINUX ) || defined( IOS )
int stricmp(const char *pStr1, const char *pStr2);
#ifndef _stricmp
#define _stricmp stricmp
#endif
int strnicmp( const char *pStr1, const char *pStr2, size_t unBufferLen );
#ifndef _strnicmp
#define _strnicmp strnicmp
#endif

#ifndef _vsnprintf_s
#define _vsnprintf_s vsnprintf
#endif

#define _TRUNCATE ((size_t)-1)

#endif

#if defined( OSX ) || defined( IOS )
// behaviors ensure NULL-termination at least as well as _TRUNCATE does, but
// wcsncpy_s/strncpy_s can non-NULL-terminate, wcslcpy/strlcpy can not.
inline errno_t wcsncpy_s(wchar_t *strDest, size_t numberOfElements, const wchar_t *strSource, size_t count)
{
	return wcslcpy(strDest, strSource, numberOfElements);
}

inline errno_t strncpy_s(char *strDest, size_t numberOfElements, const char *strSource, size_t count)
{
	return strlcpy(strDest, strSource, numberOfElements);
}

#endif

#if defined( LINUX )
// this implementation does not return whether or not the destination was 
// truncated, but that is straightforward to fix if anybody actually needs the
// return code. 
#include "string.h"
inline void wcsncpy_s(wchar_t *strDest, size_t numberOfElements, const wchar_t *strSource, size_t count)
{
	wcsncpy(strDest, strSource, numberOfElements);
	strDest[numberOfElements-1] = '\0';
}

inline void strncpy_s(char *strDest, size_t numberOfElements, const char *strSource, size_t count)
{
	strncpy(strDest, strSource, numberOfElements);
	strDest[numberOfElements-1] = '\0';
}

#endif

#if defined( _WIN32 ) && _MSC_VER < 1800
inline uint64_t strtoull(const char *str, char **endptr, int base) { return _strtoui64( str, endptr, base ); }
#endif

/* Handles copying a std::string into a buffer as would be provided in an API */
uint32_t ReturnStdString( const std::string & sValue, char *pchBuffer, uint32_t unBufferLen );

/** Returns a std::string from a uint64_t */
std::string Uint64ToString( uint64_t ulValue );

/** returns a uint64_t from a string */
uint64_t StringToUint64( const std::string & sValue );

//-----------------------------------------------------------------------------
// Purpose: Encodes a string (or binary data) from URL encoding format, see rfc1738 section 2.2.  
//          This version of the call isn't a strict RFC implementation, but uses + for space as is
//          the standard in HTML form encoding, despite it not being part of the RFC.
//
//          Dest buffer should be at least as large as source buffer to guarantee room for decode.
//-----------------------------------------------------------------------------
void V_URLEncode( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen );

/** Same as V_URLEncode, but without plus for space. */
void V_URLEncodeNoPlusForSpace( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen );

/** Same as V_URLEncodeNoPlusForSpace, but without escaping / and : */
void V_URLEncodeFullPath( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen );


//-----------------------------------------------------------------------------
// Purpose: Decodes a string (or binary data) from URL encoding format, see rfc1738 section 2.2.  
//          This version of the call isn't a strict RFC implementation, but uses + for space as is
//          the standard in HTML form encoding, despite it not being part of the RFC.
//
//          Dest buffer should be at least as large as source buffer to guarantee room for decode.
//			Dest buffer being the same as the source buffer (decode in-place) is explicitly allowed.
//-----------------------------------------------------------------------------
size_t V_URLDecode( char *pchDecodeDest, int nDecodeDestLen, const char *pchEncodedSource, int nEncodedSourceLen );

/** Same as V_URLDecode, but without plus for space. */
size_t V_URLDecodeNoPlusForSpace( char *pchDecodeDest, int nDecodeDestLen, const char *pchEncodedSource, int nEncodedSourceLen );

//-----------------------------------------------------------------------------
// Purpose: strip extension from a path
//-----------------------------------------------------------------------------
void V_StripExtension( std::string &in );

/** Returns an IP address as a string */
std::string IpToString( uint32_t unIpH );

/** Returns an IP address and port as a string */
std::string IpAndPortToString( uint32_t unIpH, uint16_t usPortH );


/** Tokenizes a string into a vector of strings */
std::vector<std::string> TokenizeString( const std::string & sString, char cToken );
