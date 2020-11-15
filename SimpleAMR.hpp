/*
    This file is part of SimpleAMR.
    Copyright (C) 2020 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>

#include <cinttypes>

#ifdef SIMPLEAMR_ENABLE_AMRNB
#include <opencore-amrnb/interf_enc.h>
#include <opencore-amrnb/interf_dec.h>
#endif

#ifdef SIMPLEAMR_ENABLE_AMRWB
#include <opencore-amrwb/dec_if.h>
#endif

#ifdef SIMPLEAMR_ENABLE_AMRWBENC
#include <vo-amrwbenc/enc_if.h>
#endif


#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#	define __WINDOWS__

#endif

#if defined(ESP_PLATFORM)

#       define _LITTLE_ENDIAN LITTLE_ENDIAN
#       define __bswap16     __bswap_16
#       define __bswap32     __bswap_32
#       define __bswap64     __bswap_64

#	include <endian.h>

#elif defined(__linux__) || defined(__CYGWIN__)

#	include <endian.h>

#elif defined(__APPLE__)

#	include <libkern/OSByteOrder.h>

#	define htobe16(x) OSSwapHostToBigInt16(x)
#	define htole16(x) OSSwapHostToLittleInt16(x)
#	define be16toh(x) OSSwapBigToHostInt16(x)
#	define le16toh(x) OSSwapLittleToHostInt16(x)

#	define htobe32(x) OSSwapHostToBigInt32(x)
#	define htole32(x) OSSwapHostToLittleInt32(x)
#	define be32toh(x) OSSwapBigToHostInt32(x)
#	define le32toh(x) OSSwapLittleToHostInt32(x)

#	define htobe64(x) OSSwapHostToBigInt64(x)
#	define htole64(x) OSSwapHostToLittleInt64(x)
#	define be64toh(x) OSSwapBigToHostInt64(x)
#	define le64toh(x) OSSwapLittleToHostInt64(x)

#	define __BYTE_ORDER    BYTE_ORDER
#	define __BIG_ENDIAN    BIG_ENDIAN
#	define __LITTLE_ENDIAN LITTLE_ENDIAN
#	define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__)

#	include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#	include <sys/endian.h>

#	define be16toh(x) betoh16(x)
#	define le16toh(x) letoh16(x)

#	define be32toh(x) betoh32(x)
#	define le32toh(x) letoh32(x)

#	define be64toh(x) betoh64(x)
#	define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)

# include <windows.h>

# if defined(_MSC_VER)
#   include <stdlib.h>

#   define htobe16(x) _byteswap_ushort(x)
#   define htole16(x) (x)
#   define be16toh(x) _byteswap_ushort(x)
#   define le16toh(x) (x)

#   define htobe32(x) _byteswap_ulong(x)
#   define htole32(x) (x)
#   define be32toh(x) _byteswap_ulong(x)
#   define le32toh(x) (x)

#   define htobe64(x) _byteswap_uint64(x)
#   define htole64(x) (x)
#   define be64toh(x) _byteswap_uint64(x)
#   define le64toh(x) (x)

# elif defined(__GNUC__) || defined(__clang__)

#   define htobe16(x) __builtin_bswap16(x)
#   define htole16(x) (x)
#   define be16toh(x) __builtin_bswap16(x)
#   define le16toh(x) (x)

#   define htobe32(x) __builtin_bswap32(x)
#   define htole32(x) (x)
#   define be32toh(x) __builtin_bswap32(x)
#   define le32toh(x) (x)

#   define htobe64(x) __builtin_bswap64(x)
#   define htole64(x) (x)
#   define be64toh(x) __builtin_bswap64(x)
#   define le64toh(x) (x)

#   else
#     error platform not supported
#   endif

#else
#  error platform not supported
#endif

namespace YukiWorkshop {
#ifdef SIMPLEAMR_ENABLE_AMRNB
	namespace SimpleAMRNB {
		class Encoder {
		private:
			void *stupid_unsafe_fragmented_memory = nullptr;

		public:
			enum AMRNB_Bitrate {
				B4750 = 0,/* 4.75 kbps */
				B5150,    /* 5.15 kbps */
				B5900,     /* 5.90 kbps */
				B6700,     /* 6.70 kbps */
				B7400,     /* 7.40 kbps */
				B7950,    /* 7.95 kbps */
				B10200,    /* 10.2 kbps */
				B12200,    /* 12.2 kbps */
				DTX    /* DTX       */
			};

			Encoder(bool dtx_enabled = false);

			~Encoder();

			// PCM is signed 16 bit, native endian, 8 kHz, 20ms
			// buf_amr should be at least 32 bytes
			uint16_t encode(AMRNB_Bitrate bitrate, const int16_t* buf_pcm, uint8_t* buf_amr);

			std::vector<uint8_t> encode(AMRNB_Bitrate bitrate, const int16_t* buf_pcm);
		};

		class Decoder {
		private:
			void *stupid_unsafe_fragmented_memory = nullptr;
		public:
			Decoder();

			~Decoder();

			// PCM is signed 16 bit, native endian, 8 kHz, 20ms
			// buf_amr should be pad to at least 32 bytes
			void decode(const uint8_t* buf_amr, int16_t* buf_pcm);

			std::vector<int16_t> decode(const uint8_t* buf_amr);
		};
	}
#endif
	namespace SimpleAMRWB {
#ifdef SIMPLEAMR_ENABLE_AMRWBENC
		class Encoder {
		private:
			void *stupid_unsafe_fragmented_memory = nullptr;

		public:
			enum AMRWB_Bitrate {
				B6600 = 0,
				B8850,
				B12650,
				B14250,
				B15850,
				B18250,
				B19850,
				B23050,
				B23850
			};

			Encoder();

			~Encoder();

			// PCM is signed 16 bit, native endian, 16 kHz, 20ms
			// buf_amr should be at least 64 bytes
			uint16_t encode(AMRWB_Bitrate bitrate, const int16_t* buf_pcm, uint8_t* buf_amr, bool dtx = false);

			std::vector<uint8_t> encode(AMRWB_Bitrate bitrate, const int16_t* buf_pcm, bool dtx = false);
		};
#endif

#ifdef SIMPLEAMR_ENABLE_AMRWB
		class Decoder {
		private:
			void *stupid_unsafe_fragmented_memory = nullptr;
		public:
			Decoder();

			~Decoder();

			// PCM is signed 16 bit, native endian, 16 kHz, 20ms
			// buf_amr should be pad to at least 64 bytes
			void decode(const uint8_t* buf_amr, int16_t* buf_pcm);

			std::vector<int16_t> decode(const uint8_t* buf_amr);
		};
#endif
	}
}
