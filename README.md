# SimpleAMR

Easy-to-use wrapper of opencore & vo AMR libraries.

Like all Simple* projects made by me, this project aims to provide a convenient & uniformed interface for AMR encoding & decoding.

## Warning
AMR codecs incorporate several patents of Nokia, Ericsson, NTT and VoiceAge. Some of them are probably expired in your country. But anyway you should be careful. [Read more](https://en.wikipedia.org/wiki/Adaptive_Multi-Rate_audio_codec#Licensing_and_patent_issues)

You are advised to use the FOSS replacements, such as Opus, Speex, and Codec2.
 
## Requirements
- C++14
- CMake 3.8+
- OpenCore AMRNB & AMRWB (optional, detected at compile time)
- VO AMRWB Encoder (optional, detected at compile time)

Currently you need to install these AMR libraries system wide.

Quick command for Ubuntu/Debian users:
```shell script
apt install libopencore-amrnb-dev libopencore-amrwb-dev libvo-amrwbenc-dev
``` 

## Usage
See the tests files for more usages.

```cpp
using namespace YukiWorkshop;

int16_t pcm_s16_native_8khz_20ms[160];

SimpleAMRWB::Encoder enc;

std::vector<uint8_t> encoded_data = enc.encode(Encoder::B12200, pcm_s16_native_8khz_20ms);
```

## License
LGPLv3