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

	static const int HalfShift = 10;
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
	static const Utf8 TrailingBytesForUTF8[256] =
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
	 * Magic values subtracted from a buffer value during UTF8 conversion.
	 * This table contains as many values as there might be trailing bytes
	 * in a UTF-8 sequence.
	 */
	static const Utf32 OffsetsFromUTF8[6] =
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

const char* AsString( CharConversionFlags v )
{
	switch(v)
	{
		case CharConversionFlags_Strict:
			return "strict";

		case CharConversionFlags_Lenient:
			return "lenient";

		default:
			return "invalid enum";
	}
}



// ---

bool IsLegalChar( Utf32 ch )
{
	if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
		return false;

	if(ch > MaxLegalUtf32)
		return false;

	return true;
}

class CharReader
{
public:
	Utf32 ch;
	int elementsToRead;

	CharReader()
	{
		reset();
	}

	void reset()
	{
		ch = 0;
		elementsToRead = 0;
	}

	CharConversionResult read( Utf8 e )
	{
		if(elementsToRead)
		{
			--elementsToRead;
			ch += e;
			if(elementsToRead > 0)
			{
				ch <<= 6;
				return CharConversionResult_UnfinishedChar;
			}
			else
			{
				if(IsLegalChar(ch))
					return CharConversionResult_Success;
				else
					return CharConversionResult_IllegalChar;
			}
		}
		else
		{
			elementsToRead = TrailingBytesForUTF8[e];
			if(elementsToRead == 0)
			{
				ch = e;
				if(IsLegalChar(ch))
					return CharConversionResult_Success;
				else
					return CharConversionResult_IllegalChar;
			}
			else
			{
				return CharConversionResult_UnfinishedChar;
			}
		}
	}

	CharConversionResult read( Utf16 e )
	{
		if(elementsToRead)
		{
			ch = e;
			if(e >= SurrogateHighStart && <= SurrogateHighEnd)
			{
				elementsToRead = 1;
				return CharConversionResult_UnfinishedChar;
			}
			else
			{
				if(IsLegalChar(ch))
					return CharConversionResult_Success;
				else
					return CharConversionResult_IllegalChar;
			}
		}
		else
		{
			if(e )
		}
	}

	CharConversionResult read( Utf32 e )
	{
		ch = e;
		if(IsLegalChar(ch))
			return CharConversionResult_Success;
		else
			return CharConversionResult_IllegalChar;
	}
};



// ---




int StringLength( const Utf8* source, int maxLength )
{
	return 0;
}

int StringLength( const Utf32* source, int maxLength )
{
	int i = 0;
	for(; (*source != 0) && (i <= maxLength); ++i, ++source)
		;
	return i;
}

bool IsLegalString( const Utf8* source, int length )
{
    Utf8 a;
    const Utf8* srcptr = source+length;
    switch(length)
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


#define ILLEGAL_CHAR_ENCOUNTERED() \
			if(flags == CharConversionFlags_Strict) \
				return CharConversionResult_IllegalChar; \
			else \
				destination->push_back(ReplacementChar);
// ^- Evil hack. :I

CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf32* source, std::vector<Utf16>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		Utf32 ch = source[i];
		if(ch <= MaxBmp)
		{
			// UTF-16 surrogate values are illegal in UTF-32.
			if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
			{
				if(flags == CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				destination->push_back(ReplacementChar);
			}
			else
				destination->push_back(ch);
		}
		else if(ch > MaxLegalUtf32)
		{
			if(flags == CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			destination->push_back(ReplacementChar);
		}
		else
		{
			ch -= HalfBase;
			destination->push_back( Utf16(ch >> HalfShift) + SurrogateHighStart );
			destination->push_back( Utf16(ch  &  HalfMask) + SurrogateLowStart );
		}
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf16* source, std::vector<Utf32>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		Utf32 ch = source[i];
		Utf32 ch2;

		// If we have a surrogate pair, convert to UTF-32 first.
		if(ch >= SurrogateHighStart && ch <= SurrogateHighEnd)
		{
			++i;
			if(i >= length)
				return CharConversionResult_UnfinishedChar;

			ch2 = source[i];

			// If its a low surrogate, convert to UTF-32.
			if(ch2 >= SurrogateLowStart && ch2 <= SurrogateLowEnd)
			{
				ch = ((ch - SurrogateHighStart) << HalfShift) + (ch2 - SurrogateLowStart) + HalfBase;
				destination->push_back(ch);
			}
			else
			{
				// Its an unpaired high surrogate.
				if(flags == CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				destination->push_back(ReplacementChar);
			}
		}
		else
		{
			// UTF-16 surrogate values are illegal in UTF-32.
			if(ch >= SurrogateLowStart && ch <= SurrogateLowEnd)
			{
				if(flags == CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				destination->push_back(ReplacementChar);
			}
			else
				destination->push_back(ch);
		}
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf32* source, std::vector<Utf8>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		int bytesToWrite = 0;
		Utf32 ch = source[i];
		
		if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
		{
			if(flags == CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			destination->push_back(ReplacementChar);
		}
		else
		{
			if( ch < Utf32(0x80) )          bytesToWrite = 1;
			else if( ch < Utf32(0x8000) )   bytesToWrite = 2;
			else if( ch < Utf32(0x100000) ) bytesToWrite = 3;
			else if( ch <= MaxLegalUtf32 )  bytesToWrite = 4;
			else
			{
				if(flags == CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				bytesToWrite = 3;
				ch = ReplacementChar;
			}

			destination->resize(destination->size()+bytesToWrite);
			int pos = destination->size()-1;
			
			
			switch(bytesToWrite)
			{
				case 4:
					(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
					ch >>= 6;
					--pos;
				
				case 3:
					(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
					ch >>= 6;
					--pos;
				
				case 2:
					(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
					ch >>= 6;
					--pos;
				
				case 1:
					(*destination)[pos] = Utf8(ch | FirstByteMark[bytesToWrite]);

				default:
					;
			}
		}
	}
	return CharConversionResult_Success;	
}


CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf8* source, std::vector<Utf32>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		int extraBytesToRead = TrailingBytesForUTF8[source[i]];
		Utf32 ch = 0;

		if(i+extraBytesToRead >= length)
		{
			return CharConversionResult_UnfinishedChar;
		}

		if(!IsLegalString(&source[i], extraBytesToRead+1))
		{
			return CharConversionResult_IllegalChar;
		}

		switch(extraBytesToRead)
		{
			case 5: ch += source[i++]; ch <<= 6;
			case 4: ch += source[i++]; ch <<= 6;
			case 3: ch += source[i++]; ch <<= 6;
			case 2: ch += source[i++]; ch <<= 6;
			case 1: ch += source[i++]; ch <<= 6;
			case 0: ch += source[i];

			default:
					;
		}
		
		ch -= OffsetsFromUTF8[extraBytesToRead];

		if(ch <= MaxLegalUtf32)
		{
			// UTF-16 surrogate values are illegal in UTF-32, and anything
			// over Plane 17 (> 0x10FFFF) is illegal.
			if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
			{
				if(flags == CharConversionFlags_Strict)
					return CharConversionResult_IllegalChar;
				ch = ReplacementChar;
			}
		}
		else
		{
			if(flags == CharConversionFlags_Strict)
				return CharConversionResult_IllegalChar;
			ch = ReplacementChar;
		}

		destination->push_back(ch);
	}
	return CharConversionResult_Success;
}

CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf8* source, std::vector<Utf16>* destination )
{
	std::vector<Utf32> buffer;
	buffer.reserve(length);

	CharConversionResult result = ConvertChars(flags, length, source, &buffer);
	if(result != CharConversionResult_Success)
		return result;

	return ConvertChars(flags, buffer.size(), &buffer[0], destination);
}

CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf16* source, std::vector<Utf8>* destination )
{
	std::vector<Utf32> buffer;
	buffer.reserve(length);

	CharConversionResult result = ConvertChars(flags, length, source, &buffer);
	if(result != CharConversionResult_Success)
		return result;

	return ConvertChars(flags, buffer.size(), &buffer[0], destination);
}






// --------------------- OLD ----------------------
/*
int ConvertChars( int length, const Utf16* source, std::vector<Utf8>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		int bytesToWrite = 0;
		Utf32 ch = source[i];
		Utf32 ch2;
		
		// If we have a surrogate pair, convert to UTF32.
		if(ch >= SurrogateHighStart && ch <= SurrogateHighEnd)
		{
			ch2 = ch;
			if(ch2 >= SurrogateLowStart && ch2 <= SurrogateLowEnd)
			{
				ch = ((ch - SurrogateLowEnd) << HalfShift) + (ch2 - SurrogateLowStart) + HalfBase;
			}
			else
			{
				// It's an unpaired high surrogate.
				ch = ReplacementChar;
			}
		}
		else
		{
			// UTF-16 surrogate values are illegal in UTF-32.
			if(ch >= SurrogateLowStart && ch <= SurrogateLowEnd)
			{
				ch = ReplacementChar;
			}
		}

		if(ch < Utf32(0x80))
			bytesToWrite = 1;
		else if(ch < Utf32(0x800))
			bytesToWrite = 2;
		else if(ch < Utf32(0x10000))
			bytesToWrite = 3;
		else if(ch <= MaxLegalUtf32)
			bytesToWrite = 4;
		else
		{
			bytesToWrite = 3;
			ch = ReplacementChar;
		}

		destination->resize(destination->size()+bytesToWrite);
		int pos = destination->size()-1;

		switch(bytesToWrite)
		{
			case 4:
				(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
				ch >>= 6;
				--pos;
			
			case 3:
				(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
				ch >>= 6;
				--pos;
			
			case 2:
				(*destination)[pos] = Utf8((ch | ByteMark) & ByteMask);
				ch >>= 6;
				--pos;
			
			case 1:
				(*destination)[pos] = Utf8(ch | FirstByteMark[bytesToWrite]);

			default:
				;
		}
	}
	return i;
}


int ConvertChars( int length, const Utf8* source, std::vector<Utf16>* destination )
{
	int i = 0;
	for(; i < length; ++i)
	{
		int extraBytesToRead = TrailingBytesForUTF8[source[i]];
		Utf32 ch = 0;

		if(i+extraBytesToRead >= length)
		{
			return -1;
		}

		if(!IsLegalString(&source[i], extraBytesToRead+1))
		{
			return -1;
		}

		switch(extraBytesToRead)
		{
			case 5: ch += source[i++]; ch <<= 6;
			case 4: ch += source[i++]; ch <<= 6;
			case 3: ch += source[i++]; ch <<= 6;
			case 2: ch += source[i++]; ch <<= 6;
			case 1: ch += source[i++]; ch <<= 6;
			case 0: ch += source[i];

			default:
					;
		}

		ch -= OffsetsFromUTF8[extraBytesToRead];

		if(ch <= MaxBmp)
		{
			// UTF-16 surrogate values are illegal in UTF-32
			if(ch >= SurrogateHighStart && ch <= SurrogateLowEnd)
			{
				ch = ReplacementChar;
			}

			destination->push_back(ch);
		}
		else if(ch > MaxUtf16)
		{
			ch = ReplacementChar;
		}
		else
		{
			ch -= HalfBase;
			destination->push_back( Utf16((ch >> HalfShift) + SurrogateHighStart ) );
			destination->push_back( Utf16((ch  &  HalfMask) + SurrogateLowStart ) );
		}
	}
	return i;
}
*/


} // namespace SparkPlug

