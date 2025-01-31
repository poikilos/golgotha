// A one-dimensional array of bits, similar to STL bitset.
//
// Copyright 2000 Andrew Kirmse.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#ifndef _BITARRAY_H
#define _BITARRAY_H

#include "memory/malloc.h"
#include <stdlib.h>

class BitArray
{
public:

	//
	// Constructors and destructor
	//
	typedef unsigned long store_type;
	explicit BitArray(unsigned int size)
	{
		Init(size);

		// Clear last bits
		Trim();
	}

	BitArray(const BitArray &that)
	{
		mpStore = 0;
		*this = that;
	};

	//deserializes object. Warning: size is here in bytes, not bits
	explicit BitArray(unsigned int size, store_type single_word, char * data)
	{
		Init(size*bits_per_byte);
		Trim();
		mSingleWord=single_word;
		if (data)
		{
			memcpy(mpStore,data,size);
		}
	}

	virtual ~BitArray()
	{
		if (mLength > 1)
		{
			delete mpStore;
		}
	}

	//
	// Operators
	//

	class BitProxy;

	BitArray &operator=(const BitArray &that)
	{
		if (this != &that)
		{
			if (mLength > 1)
			{
				delete mpStore;
			}

			Init(that.mNumBits);

			memcpy(mpStore, that.mpStore, mLength * sizeof(store_type));
		}
		return *this;
	}

	BitProxy operator[](unsigned pos)
	{
		I4_ASSERT(pos < mNumBits,"INTERNAL: Array index out of bounds.");
		return BitProxy(*this, pos);
	}

	const BitProxy operator[](unsigned pos) const
	{
		I4_ASSERT(pos < mNumBits,"INTERNAL: Array index out of bounds.");
		return BitProxy(const_cast<BitArray &>(*this), pos);
	}

	bool operator==(const BitArray &that) const
	{
		if (mNumBits != that.mNumBits)
		{
			return false;
		}

		for (unsigned i = 0; i < mLength; i++)
		{
			if (mpStore[i] != that.mpStore[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const BitArray &that) const
	{
		return !(*this == that);
	}

	BitArray &operator&=(const BitArray &that)
	{
		I4_ASSERT(mNumBits == that.mNumBits,"INTERNAL: Illegal operation on BitArrays: Size differs");
		for (unsigned i = 0; i < mLength; i++)
		{
			mpStore[i] &= that.mpStore[i];
		}
		return *this;
	}

	BitArray operator|=(const BitArray &that)
	{
		I4_ASSERT(mNumBits == that.mNumBits,"INTERNAL: Illegal operation on bitarrays: Size differs.");
		for (unsigned i = 0; i < mLength; i++)
		{
			mpStore[i] |= that.mpStore[i];
		}
		return *this;
	}

	BitArray operator^=(const BitArray &that)
	{
		I4_ASSERT(mNumBits == that.mNumBits,"INTERNAL: Illegal operation on bitarrays: Size differs.");
		for (unsigned i = 0; i < mLength; i++)
		{
			mpStore[i] ^= that.mpStore[i];
		}
		return *this;
	}

	BitArray operator~() const
	{
		return BitArray(*this).FlipAllBits();
	}

	//hmmm... declaring these methods friend seems to be the same
	//as declaring them outside the class. They are global and non-member
	//functions. Is this notation portable?
	friend BitArray operator&(const BitArray &a1, const BitArray &a2)
	{
		return BitArray(a1) &= a2;
	}

	friend BitArray operator|(const BitArray &a1, const BitArray &a2)
	{
		return BitArray(a1) |= a2;
	}

	friend BitArray operator^(const BitArray &a1, const BitArray &a2)
	{
		return BitArray(a1) ^= a2;
	}

	//
	// Plain English interface
	//

	// Set all bits to false.
	void Clear()
	{
		memset(mpStore, 0, mLength * sizeof(store_type));
	}

	// Set the bit at position pos to true.
	void SetBit(unsigned pos)
	{
		I4_ASSERT(pos<mNumBits,"INTERNAL: Bitarray index out of range.");
		mpStore[GetIndex(pos)] |= 1 << GetOffset(pos);
	}

	// Set the bit at position pos to false.
	void ClearBit(unsigned pos)
	{
		I4_ASSERT(pos<mNumBits,"INTERNAL: Bitarray index out of range.");
		mpStore[GetIndex(pos)] &= ~(1 << GetOffset(pos));
	}

	// Toggle the bit at position pos.
	void FlipBit(unsigned pos)
	{
		I4_ASSERT(pos<mNumBits,"INTERNAL: Bitarray index out of range.");
		mpStore[GetIndex(pos)] ^= 1 << GetOffset(pos);
	}

	// Set the bit at position pos to the given value.
	void Set(unsigned pos, bool val)
	{
		val ? SetBit(pos) : ClearBit(pos);
	}

	// Returns true iff the bit at position pos is true.
	bool IsBitSet(unsigned pos) const
	{
		I4_ASSERT(pos<mNumBits,"INTERNAL: Bitarray index out of range.");
		return (mpStore[GetIndex(pos)] & (1 << GetOffset(pos))) != 0;
	}

	// Returns true iff all bits are false.
	bool AllBitsFalse() const
	{
		for (unsigned i=0; i < mLength; i++)
		{
			if (mpStore[i] != 0)
			{
				return false;
			}
		}
		return true;
	}

	// Change value of all bits
	BitArray &FlipAllBits()
	{
		for (unsigned i=0; i < mLength; i++)
		{
			mpStore[i] = ~mpStore[i];
		}

		Trim();
		return *this;
	}
	int countbits()
	{
		unsigned int n=0;

		for (unsigned i=0; i<mNumBits; i++)
		{
			if (IsBitSet(i))
			{
				n++;
			}
		}
		return n;
	}
	//returns the data needed to serialize this object
	char *PrepareWrite(int &len, int &numbits, store_type &single_word)
	{
		len= (sizeof(store_type)*mLength);
		numbits=mNumBits;
		single_word=mSingleWord;
		return (char *)mpStore;
	}

	//
	// Bit proxy (for operator[])
	//

	friend class BitProxy;

	class BitProxy
	{
public:
		BitProxy(BitArray &array, unsigned pos) :
			mArray(array),
			mPos(pos)
		{
		}

		BitProxy &operator=(bool value)
		{
			mArray.Set(mPos, value);
			return *this;
		}

		BitProxy &operator=(const BitProxy &that)
		{
			mArray.Set(mPos, that.mArray.IsBitSet(that.mPos));
			return *this;
		}

		operator bool() const
		{
			return mArray.IsBitSet(mPos);
		}

		bool Flip()
		{
			mArray.FlipBit(mPos);
			return mArray.IsBitSet(mPos);
		}

private:
		BitArray &mArray;
		unsigned mPos;
	};


private:


	enum
	{
		bits_per_byte = 8,
		cell_size     = sizeof(store_type) * bits_per_byte
	};

	store_type * mpStore;
	store_type mSingleWord;        // Use this buffer when mLength is 1
	unsigned mLength;              // Length of mpStore in units of store_type
	unsigned mNumBits;

public:
	// Get the index and bit offset for a given bit number.
	static unsigned GetIndex(unsigned bit_num)
	{
		return bit_num / cell_size;
	}
	//returns the amount of data allocated.
	unsigned Size()
	{
		return sizeof(store_type)*mLength;
	}
	store_type *GetStorage()
	{
		return mpStore;
	}
protected:
	static unsigned GetOffset(unsigned bit_num)
	{
		return bit_num % cell_size;
	}

	void Init(unsigned size)
	{
		mNumBits = size;

		if (size == 0)
		{
			mLength = 0;
		}
		else
		{
			mLength = 1 + GetIndex(size - 1);
		}

		// Avoid allocation if length is 1 (common case)
		if (mLength <= 1)
		{
			mpStore = &mSingleWord;
		}
		else
		{
			mpStore = new store_type[mLength];
		}
		mSingleWord=0;
	}

	// Force overhang bits at the end to 0
	inline void Trim()
	{
		unsigned extra_bits = mNumBits % cell_size;

		if (mLength > 0 && extra_bits != 0)
		{
			mpStore[mLength - 1] &= ~((~(store_type) 0) << extra_bits);
		}
	}
};

#endif
