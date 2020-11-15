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

#include "SimpleAMR.hpp"

using namespace YukiWorkshop;

#ifdef SIMPLEAMR_ENABLE_AMRNB

SimpleAMRNB::Encoder::Encoder(bool dtx_enabled) {
	stupid_unsafe_fragmented_memory = Encoder_Interface_init(dtx_enabled);
}

SimpleAMRNB::Encoder::~Encoder() {
	Encoder_Interface_exit(stupid_unsafe_fragmented_memory);
}

uint16_t SimpleAMRNB::Encoder::encode(AMRNB_Bitrate bitrate, const int16_t *buf_pcm, unsigned char *buf_amr) {
	int16_t stupid_bigendian_pcm[160];

	for (int i=0; i<160; i++) {
		stupid_bigendian_pcm[i] = htobe16(buf_pcm[i]);
	}

	return Encoder_Interface_Encode(stupid_unsafe_fragmented_memory, static_cast<Mode>(bitrate), buf_pcm, buf_amr, 0);
}

std::vector<uint8_t> SimpleAMRNB::Encoder::encode(AMRNB_Bitrate bitrate, const int16_t *buf_pcm) {
	std::vector<uint8_t> ret;
	ret.resize(32);
	auto rc = encode(bitrate, buf_pcm, ret.data());
	ret.resize(rc);
	return ret;
}

#endif

SimpleAMRNB::Decoder::Decoder() {
	stupid_unsafe_fragmented_memory = Decoder_Interface_init();
}

SimpleAMRNB::Decoder::~Decoder() {
	Decoder_Interface_exit(stupid_unsafe_fragmented_memory);
}

void SimpleAMRNB::Decoder::decode(const uint8_t *buf_amr, int16_t *buf_pcm) {
	int16_t stupid_bigendian_pcm[160];
	Decoder_Interface_Decode(stupid_unsafe_fragmented_memory, buf_amr, stupid_bigendian_pcm, 0);

	for (int i=0; i<160; i++) {
		buf_pcm[i] = be16toh(stupid_bigendian_pcm[i]);
	}
}

std::vector<int16_t> SimpleAMRNB::Decoder::decode(const uint8_t *buf_amr) {
	std::vector<int16_t> ret;
	ret.resize(160);
	decode(buf_amr, ret.data());
	return ret;
}

#ifdef SIMPLEAMR_ENABLE_AMRWBENC

SimpleAMRWB::Encoder::Encoder() {
	stupid_unsafe_fragmented_memory = E_IF_init();
}

SimpleAMRWB::Encoder::~Encoder() {
	E_IF_exit(stupid_unsafe_fragmented_memory);
}

uint16_t SimpleAMRWB::Encoder::encode(SimpleAMRWB::Encoder::AMRWB_Bitrate bitrate, const int16_t *buf_pcm, uint8_t *buf_amr, bool dtx) {
	return E_IF_encode(stupid_unsafe_fragmented_memory, bitrate, buf_pcm, buf_amr, dtx);
}

std::vector<uint8_t> SimpleAMRWB::Encoder::encode(SimpleAMRWB::Encoder::AMRWB_Bitrate bitrate, const int16_t *buf_pcm, bool dtx) {
	std::vector<uint8_t> ret(64);
	auto rc = encode(bitrate, buf_pcm, ret.data(), dtx);
	ret.resize(rc);
	return ret;
}

#endif

#ifdef SIMPLEAMR_ENABLE_AMRWB

SimpleAMRWB::Decoder::Decoder() {
	stupid_unsafe_fragmented_memory = D_IF_init();
}

SimpleAMRWB::Decoder::~Decoder() {
	D_IF_exit(stupid_unsafe_fragmented_memory);
}

void SimpleAMRWB::Decoder::decode(const uint8_t *buf_amr, int16_t *buf_pcm) {
	D_IF_decode(stupid_unsafe_fragmented_memory, buf_amr, buf_pcm, 0);
}

std::vector<int16_t> SimpleAMRWB::Decoder::decode(const uint8_t *buf_amr) {
	std::vector<int16_t> ret;
	ret.resize(320);
	decode(buf_amr, ret.data());
	return ret;
}

#endif