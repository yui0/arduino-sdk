// Berry Audio
// ©2017 Yuichiro Nakada

#ifndef USBaudio_h_
#define USBaudio_h_

#include "usb_desc.h"
#ifdef AUDIO_INTERFACE

#define FEATURE_MAX_VOLUME 0xFF  // volume accepted from 0 to 0xFF

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t usb_audio_receive_buffer[];
extern uint16_t usb_audio_transmit_buffer[];
extern void usb_audio_receive_callback(unsigned int len);
extern unsigned int usb_audio_transmit_callback();
int usb_audio_set_feature(void *stp, uint8_t *buf);
int usb_audio_get_feature(void *stp, uint8_t *data, uint32_t *datalen);

extern uint32_t usb_audio_sync_feedback;
extern uint8_t usb_audio_receive_setting;
extern uint8_t usb_audio_transmit_setting;

struct fifo_t {
	uint32_t *buf;
	int head;
	int tail;
	int size;
};
extern struct fifo_t usb_audio_fifo;
int fifo_read(struct fifo_t *f, uint32_t *a);
#ifdef __cplusplus
}

// audio features supported
struct usb_audio_features_struct {
	int change;  // set to 1 when any value is changed
	int mute;    // 1=mute, 0=unmute
	int volume;  // volume from 0 to FEATURE_MAX_VOLUME, maybe should be float from 0.0 to 1.0
};

#endif // __cplusplus
#endif // AUDIO_INTERFACE
#endif // USBaudio_h_
