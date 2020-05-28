

#ifndef AUDIO_H
#define AUDIO_H

#define WBANK_START	_wbankSegmentRomStart
#define PBANK_START	_pbankSegmentRomStart
#define PBANK_END	_pbankSegmentRomEnd
#define MUSIC1_START	_song1SegmentRomStart
#define MUSIC1_END   	_song1SegmentRomEnd

extern u8 _wbankSegmentRomStart[];
extern u8 _pbankSegmentRomStart[];
extern u8 _pbankSegmentRomEnd[];
extern u8 _song1SegmentRomStart[];
extern u8 _song1SegmentRomEnd[];
extern u8 _sfxSegmentRomStart[];
extern u8 _sfxSegmentRomEnd[];

extern int ptr_buf[];
extern int tune_buf[];
extern int sfx_buf[];

#endif /* AUDIO_H */