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
#include <cstdio>
#include <cassert>

int main(int argc, char** argv) {
	if (argc < 3) {
		puts("Usage: me <foo_s16_8khz_mono.pcm> <foo.amr>");
		return 2;
	}

	YukiWorkshop::SimpleAMRNB::Encoder e;

	FILE *f_in = fopen(argv[1], "rb");
	FILE *f_out = fopen(argv[2], "wb");

	assert(f_in);
	assert(f_out);

	int16_t pcm[160];

	fwrite("#!AMR\n", 1, 6, f_out);

	while (fread(pcm, 1, sizeof(pcm), f_in) == sizeof(pcm)) {
		printf("pos: %ld\n", ftell(f_in));
		auto enc = e.encode(YukiWorkshop::SimpleAMRNB::Encoder::B12200, pcm);
		printf("encoded %zu bytes\n", enc.size());
		fwrite(enc.data(), 1, enc.size(), f_out);
	}

	fclose(f_in);
	fclose(f_out);

	return 0;
}