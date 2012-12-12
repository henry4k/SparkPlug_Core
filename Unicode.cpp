#include <cstring> // memcpy
#include <SparkPlug/Common.h>
#include <SparkPlug/Unicode.h>

namespace SparkPlug
{
namespace Unicode
{
	static const Utf32 ReplacementChar = 0x0000FFFD;
	static const Utf32 MaxBmp          = 0x0000FFFF;
	static const Utf32 MaxUtf16        = 0x0010FFFF;
	static const Utf32 MaxUtf32        = 0x7FFFFFFF;
	static const Utf32 MaxLegalUtf32   = 0x0010FFFF;

	static const int   HalfShift = 10;
	static const Utf32 HalfBase = 0x0010000UL;
	static const Utf32 HalfMask = 0x3FFUL;
	
	static const Utf32 ByteMask = 0xBF;
	static const Utf32 ByteMark = 0x80;

	static const Utf32 SurrogateHighStart = 0xD800;
	static const Utf32 SurrogateHighEnd   = 0xDBFF;
	static const Utf32 SurrogateLowStart  = 0xDC00;
	static const Utf32 SurrogateLowEnd    = 0xDFFF;

	/**
	 * Index into the table below with the first byte of a UTF-8 sequence to
	 * get the number of trailing bytes that are supposed to follow it.
	 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
	 * left as-is for anyone who may want to do such conversion, which was
	 * allowed in earlier algorithms.
	 */
	static const Utf8 TrailingBytesForUtf8[256] =
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
	};

	/**
	 * Magic values subtracted from a buffer value during Utf8 conversion.
	 * This table contains as many values as there might be trailing bytes
	 * in a UTF-8 sequence.
	 */
	static const Utf32 OffsetsFromUtf8[6] =
	{
		0x00000000UL, 0x00003080UL, 0x000E2080UL, 
		0x03C82080UL, 0xFA082080UL, 0x82082080UL
	};
	
	/**
	 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
	 * into the first byte, depending on how many bytes follow.  There are
	 * as many entries in this table as there are UTF-8 sequence types.
	 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
	 * for *legal* UTF-8 will be 4 or fewer bytes total.
	 */
	static const Utf8 FirstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

} // namespace Unicode

using namespace Unicode;


const char* AsString( CharConversionResult v )
{
	switch(v)
	{
		case CharConversionResult_Success:
			return "success";

		case CharConversionResult_UnfinishedChar:
			return "unfinished char";

		case CharConversionResult_IllegalChar:
			return "illegal char";

		default:
			return "invalid enum";
	}
}

bool IsLegalString( const Utf8* source, int units )
{
    Utf8 a;
    const Utf8* srcptr = source+units;
    switch(units)
	{
		default: return false;
		// Everything else falls through when "true"...
		case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
		case 2: if ((a = (*--srcptr)) > 0xBF) return false;

		switch(*source)
		{
			// no fall-through in this inner switch
			case 0xE0: if (a < 0xA0) return false; break;
			case 0xED: if (a > 0x9F) return false; break;
			case 0xF0: if (a < 0x90) return false; break;
			case 0xF4: if (a > 0x8F) return false; break;
			default:   if (a < 0x80) return false;
		}

		case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

template<typename T>
class CharReceiver
{
public:
	virtual ~CharReceiver() {}
	virtual void onReceiveChar( T ch ) = 0;
};

template<typename T>
class CharCounter : public CharReceiver<T>
{
public:
	int count;
	CharCounter() : count(0) {}
	void onReceiveChar( T ch )  { ++count; }
};

template<typename T>
class CharAppender : public CharReceiver<T>
{
public:
	std::vector<T>* target;
	CharAppender( std::vector<T>* d ) : target(d) {}
	void onReceiveChar( T ch )  { target->push_back(ch); }
};

bool IsLegalChar( Utf32 ch )
{
	if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
		return false;

	if(ch > MaxLegalUtf32)
		return false;

	return true;
}

CharConversionResult ConvertCharsEx( int flags, int units, const Utf32* source, CharReceiver<Utf16>* target )
{
	int i = 0;
	for(; (units == -1) || (i < units); ++i)
	{
		Utf32 ch = source[i];

		if((ch == 0) && (flags & CharConversionFlags_ZeroTerminated))
			break;

		if(ch <= MaxBmp)
		{
			// UTF-16 surrogate values are illegal in UTF-32.
			if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
			{
				if(flags & CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				target->onReceiveChar(ReplacementChar);
			}
			else
				target->onReceiveChar(ch);
		}
		else if(ch > MaxLegalUtf32)
		{
			if(flags & CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			target->onReceiveChar(ReplacementChar);
		}
		else
		{
			ch -= HalfBase;
			target->onReceiveChar( Utf16(ch >> HalfShift) + SurrogateHighStart );
			target->onReceiveChar( Utf16(ch  &  HalfMask) + SurrogateLowStart );
		}
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertCharsEx( int flags, int units, const Utf16* source, CharReceiver<Utf32>* target )
{
	int i = 0;
	for(; (units == -1) || (i < units); ++i)
	{
		Utf32 ch = source[i];
		Utf32 ch2;

		if((ch == 0) && (flags & CharConversionFlags_ZeroTerminated))
			break;

		// If we have a surrogate pair, convert to UTF-32 first.
		if(ch >= SurrogateHighStart && ch <= SurrogateHighEnd)
		{
			++i;
			if(i >= units)
				return CharConversionResult_UnfinishedChar;

			ch2 = source[i];
			if((ch2 == 0) && (flags & CharConversionFlags_ZeroTerminated))
				return CharConversionResult_UnfinishedChar;

			// If its a low surrogate, convert to UTF-32.
			if(ch2 >= SurrogateLowStart && ch2 <= SurrogateLowEnd)
			{
				ch = ((ch - SurrogateHighStart) << HalfShift) + (ch2 - SurrogateLowStart) + HalfBase;
				target->onReceiveChar(ch);
			}
			else
			{
				// Its an unpaired high surrogate.
				if(flags & CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				target->onReceiveChar(ReplacementChar);
			}
		}
		else
		{
			// UTF-16 surrogate values are illegal in UTF-32.
			if(ch >= SurrogateLowStart && ch <= SurrogateLowEnd)
			{
				if(flags & CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				target->onReceiveChar(ReplacementChar);
			}
			else
				target->onReceiveChar(ch);
		}
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertCharsEx( int flags, int units, const Utf32* source, CharReceiver<Utf8>* target )
{
	Utf8 buffer[4];
	int i = 0;
	for(; (units == -1) || (i < units); ++i)
	{
		int bytesToWrite = 0;
		Utf32 ch = source[i];

		if((ch == 0) && (flags & CharConversionFlags_ZeroTerminated))
			break;
		
		if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
		{
			if(flags & CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			bytesToWrite = 3;
			ch = ReplacementChar;
		}
		else if( ch < Utf32(0x80) )     bytesToWrite = 1;
		else if( ch < Utf32(0x8000) )   bytesToWrite = 2;
		else if( ch < Utf32(0x100000) ) bytesToWrite = 3;
		else if( ch <= MaxLegalUtf32 )  bytesToWrite = 4;
		else
		{
			if(flags & CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			bytesToWrite = 3;
			ch = ReplacementChar;
		}

		int pos = bytesToWrite;
		switch(bytesToWrite)
		{
			case 4: buffer[pos] = Utf8((ch | ByteMark) & ByteMask); ch >>= 6; --pos;
			case 3: buffer[pos] = Utf8((ch | ByteMark) & ByteMask); ch >>= 6; --pos;
			case 2: buffer[pos] = Utf8((ch | ByteMark) & ByteMask); ch >>= 6; --pos;
			case 1: buffer[pos] = Utf8(ch | FirstByteMark[bytesToWrite]);
			default: ;
		}

		for(int j = 0; j < bytesToWrite; ++j)
			target->onReceiveChar(buffer[j]);
	}
	return CharConversionResult_Success;	
}

CharConversionResult ConvertCharsEx( int flags, int units, const Utf8* source, CharReceiver<Utf32>* target )
{
	int i = 0;
	for(; (units == -1) || (i < units); ++i)
	{
		int extraBytesToRead = TrailingBytesForUtf8[source[i]];
		Utf32 ch = 0;
		
		if((source[i] == 0) && (flags & CharConversionFlags_ZeroTerminated))
			break;

		if((units != -1) && (i+extraBytesToRead >= units))
			return CharConversionResult_UnfinishedChar;

		if(!IsLegalString(&source[i], extraBytesToRead+1))
			return CharConversionResult_IllegalChar;

		for(int j = extraBytesToRead; j >= 0; --j)
		{
			if(source[i] == 0)
				return CharConversionResult_UnfinishedChar;
			ch += source[i];
			++i;
			if(j != 0)
				ch <<= 6;
		}
		
		ch -= OffsetsFromUtf8[extraBytesToRead];

		if(ch <= MaxLegalUtf32)
		{
			// UTF-16 surrogate values are illegal in UTF-32, and anything
			// over Plane 17 (> 0x10FFFF) is illegal.
			if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
			{
				if(flags & CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				ch = ReplacementChar;
			}
		}
		else
		{
			if(flags & CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			ch = ReplacementChar;
		}

		target->onReceiveChar(ch);
	}
	return CharConversionResult_Success;
}




CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf16>* target )
{
	CharAppender<Utf16> receiver(target);
	return ConvertCharsEx(flags, units, source, &receiver);
}

CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf8>* target )
{
	CharAppender<Utf8> receiver(target);
	return ConvertCharsEx(flags, units, source, &receiver);
}

CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf32>* target )
{
	CharAppender<Utf32> receiver(target);
	return ConvertCharsEx(flags, units, source, &receiver);
}

CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf32>* target )
{
	CharAppender<Utf32> receiver(target);
	return ConvertCharsEx(flags, units, source, &receiver);
}

CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf8>* target )
{
	std::vector<Utf32> buffer;
	if(units != -1)
		buffer.reserve(units);

	CharConversionResult result = ConvertChars(flags, units, source, &buffer);
	if(result != CharConversionResult_Success)
		return result;

	return ConvertChars(flags, buffer.size(), &buffer[0], target);
}

CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf16>* target )
{
	std::vector<Utf32> buffer;
	if(units != -1)
		buffer.reserve(units);

	CharConversionResult result = ConvertChars(flags, units, source, &buffer);
	if(result != CharConversionResult_Success)
		return result;

	return ConvertChars(flags, buffer.size(), &buffer[0], target);
}

template<typename T>
int StringSize( const T* source, int maxUnits )
{
	int i = 0;
	for(; ((maxUnits == -1) || (i < maxUnits)) && (source[i] != 0); ++i)
		;
	return i;
}

template<typename T>
CharConversionResult CopyChars( int flags, int units, const T* source, std::vector<T>* destination )
{
	int oldSize = destination->size();
	if(flags & CharConversionFlags_ZeroTerminated)
	{
		if(units != -1)
			destination->reserve(oldSize+units);

		for(int i = 0; (units == -1) || (i < units); ++i)
		{
			if(source[i] == 0)
				break;
			destination->push_back(source[i]);
		}
	}
	else
	{
		if(units == -1)
			units = StringSize(source, -1);

		destination->resize(oldSize+units);
		memcpy(&destination[oldSize], source, units);
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf32>* target )
{
	return CopyChars<Utf32>(flags, units, source, target);
}

CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf16>* target )
{
	return CopyChars<Utf16>(flags, units, source, target);
}

CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf8>* target )
{
	return CopyChars<Utf8>(flags, units, source, target);
}



int StringLength( const Utf8* source, int maxUnits )
{
	CharCounter<Utf32> counter;
	ConvertCharsEx(CharConversionFlags_ZeroTerminated, maxUnits, source, &counter);
	// Ignore conversion result
	return counter.count;
}

int StringLength( const Utf16* source, int maxUnits )
{
	CharCounter<Utf32> counter;
	ConvertCharsEx(CharConversionFlags_ZeroTerminated, maxUnits, source, &counter);
	// Ignore conversion result
	return counter.count;
}

int StringLength( const Utf32* source, int maxUnits )
{
	return StringSize(source, maxUnits);
}

} // namespace SparkPlug

