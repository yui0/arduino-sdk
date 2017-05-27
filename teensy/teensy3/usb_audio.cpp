// Berry Audio
// ©2017 Yuichiro Nakada

#include "usb_dev.h"
#include "usb_audio.h"
#include "HardwareSerial.h"
#include <string.h> // for memcpy()

#ifdef AUDIO_INTERFACE // defined by usb_dev.h -> usb_desc.h
#if F_CPU >= 20000000

// Uncomment this to work around a limitation in Macintosh adaptive rates
// This is not a perfect solution.  Details here:
// https://forum.pjrc.com/threads/34855-Distorted-audio-when-using-USB-input-on-Teensy-3-1
//#define MACOSX_ADAPTIVE_LIMIT

int fifo_read(struct fifo_t *f, uint32_t *a)
{
	if (f->tail != f->head) {		// see if any data is available
		*a = f->buf[f->tail];		// grab a byte from the buffer
		f->tail++;			// increment the tail
		if (f->tail == f->size) {	// check for wrap-around
			f->tail = 0;
		}
	} else {
		*a = 0;
		return 0; // number of bytes read
	}
	return 1;
}

int fifo_write(struct fifo_t *f, const uint32_t *buf, int n)
{
	const uint32_t *p = buf;

	for (int i=0; i<n; i++) {
		// first check to see if there is space in the buffer
		if ((f->head+1 == f->tail) || ((f->head+1 == f->size) && (f->tail == 0))) {
			return i;	// no more room
		} else {
			f->buf[f->head] = *p++;
			f->head++;	// increment the head
			if (f->head == f->size) {	// check for wrap-around
				f->head = 0;
			}
		}
	}
	return n;
}

uint32_t usb_audio_data[1024];
struct fifo_t usb_audio_fifo = {
	usb_audio_data, 0, 0, 1024,
};


struct usb_audio_features_struct features = {0, 0, FEATURE_MAX_VOLUME/2};

#define DMABUFATTR __attribute__ ((section(".dmabuffers"), aligned (4)))
uint16_t usb_audio_receive_buffer[AUDIO_RX_SIZE/2] DMABUFATTR;
uint32_t usb_audio_sync_feedback DMABUFATTR;
uint8_t usb_audio_receive_setting = 0;


// Called from the USB interrupt when an isochronous packet arrives
// we must completely remove it from the receive buffer before returning
void usb_audio_receive_callback(unsigned int len)
{
	len >>= 2; // 1 sample = 4 bytes: 2 left, 2 right
	fifo_write(&usb_audio_fifo, (uint32_t*)usb_audio_receive_buffer, len);
}


uint16_t usb_audio_transmit_buffer[AUDIO_TX_SIZE/2] DMABUFATTR;
uint8_t usb_audio_transmit_setting = 0;

// Called from the USB interrupt when ready to transmit another
// isochronous packet.  If we place data into the transmit buffer,
// the return is the number of bytes.  Otherwise, return 0 means
// no data to transmit
unsigned int usb_audio_transmit_callback()
{
	/*static uint32_t count=5;
	uint32_t avail, num, target, offset, len=0;
	audio_block_t *left, *right;

	if (++count < 9) {   // TODO: dynamic adjust to match USB rate
		target = 44;
	} else {
		count = 0;
		target = 45;
	}
	while (len < target) {
		num = target - len;
		left = left_1st;
		if (left == NULL) {
			// buffer underrun - PC is consuming too quickly
			memset(usb_audio_transmit_buffer + len, 0, num * 4);
			//serial_print("%");
			break;
		}
		right = right_1st;
		offset = offset_1st;

		avail = AUDIO_BLOCK_SAMPLES - offset;
		if (num > avail) num = avail;

		copy_from_buffers((uint32_t *)usb_audio_transmit_buffer + len,
			left->data + offset, right->data + offset, num);
		len += num;
		offset += num;
		if (offset >= AUDIO_BLOCK_SAMPLES) {
			AudioStream::release(left);
			AudioStream::release(right);
			left_1st = left_2nd;
			left_2nd = NULL;
			right_1st = right_2nd;
			right_2nd = NULL;
			offset_1st = 0;
		} else {
			offset_1st = offset;
		}
	}
	return target * 4;*/
	return 45*4;
}


struct setup_struct {
	union {
		struct {
			uint8_t bmRequestType;
			uint8_t bRequest;
			union {
				struct {
					uint8_t bChannel;  // 0=main, 1=left, 2=right
					uint8_t bCS;       // Control Selector
				};
				uint16_t wValue;
			};
			union {
				struct {
					uint8_t bIfEp;     // type of entity
					uint8_t bEntityId; // UnitID, TerminalID, etc.
				};
				uint16_t wIndex;
			};
			uint16_t wLength;
		};
	};
};

int usb_audio_get_feature(void *stp, uint8_t *data, uint32_t *datalen)
{
	struct setup_struct setup = *((struct setup_struct *)stp);
	if (setup.bmRequestType==0xA1) { // should check bRequest, bChannel, and UnitID
		if (setup.bCS==0x01) { // mute
			data[0] = features.mute;  // 1=mute, 0=unmute
			*datalen = 1;
			return 1;
		} else if (setup.bCS==0x02) { // volume
			if (setup.bRequest==0x81) { // GET_CURR
				data[0] = features.volume & 0xFF;
				data[1] = (features.volume>>8) & 0xFF;
			} else if (setup.bRequest==0x82) { // GET_MIN
				//serial_print("vol get_min\n");
				data[0] = 0;     // min level is 0
				data[1] = 0;
			} else if (setup.bRequest==0x83) { // GET_MAX
				data[0] = FEATURE_MAX_VOLUME & 0xFF;  // max level, for range of 0 to MAX
				data[1] = (FEATURE_MAX_VOLUME>>8) & 0x0F;
			} else if (setup.bRequest==0x84) { // GET_RES
				data[0] = 1; // increment vol by by 1
				data[1] = 0;
			} else { // pass over SET_MEM, etc.
				return 0;
			}
			*datalen = 2;
			return 1;
		}
	}
	return 0;
}

int usb_audio_set_feature(void *stp, uint8_t *buf)
{
	struct setup_struct setup = *((struct setup_struct *)stp);
	if (setup.bmRequestType==0x21) { // should check bRequest, bChannel and UnitID
		if (setup.bCS==0x01) { // mute
			if (setup.bRequest==0x01) { // SET_CUR
				features.mute = buf[0]; // 1=mute,0=unmute
				features.change = 1;
				return 1;
			}
		} else if (setup.bCS==0x02) { // volume
			if (setup.bRequest==0x01) { // SET_CUR
				features.volume = buf[0] + (buf[1]<<8);
				features.change = 1;
				return 1;
			}
		}
	}
	return 0;
}

#endif // F_CPU
#endif // AUDIO_INTERFACE
