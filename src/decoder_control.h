/* the Music Player Daemon (MPD)
 * Copyright (C) 2003-2007 by Warren Dukes (warren.dukes@gmail.com)
 * Copyright (C) 2008 Max Kellermann <max@duempel.org>
 * This project's homepage is: http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DECODE_H
#define DECODE_H

#include "decoder_api.h"
#include "song.h"

#include "audio_format.h"
#include "notify.h"

#define DECODE_TYPE_FILE	0
#define DECODE_TYPE_URL		1

enum decoder_state {
	DECODE_STATE_STOP = 0,
	DECODE_STATE_START,
	DECODE_STATE_DECODE
};

#define DECODE_ERROR_NOERROR	0
#define DECODE_ERROR_UNKTYPE	10
#define DECODE_ERROR_FILE	20

struct decoder_control {
	Notify notify;

	volatile enum decoder_state state;
	volatile enum decoder_command command;
	volatile mpd_uint16 error;
	volatile mpd_sint8 seekError;
	volatile mpd_sint8 seekable;
	volatile double seekWhere;
	struct audio_format audioFormat;
	Song *current_song;
	Song *volatile next_song;
	volatile float totalTime;
};

extern struct decoder_control dc;

void dc_init(void);

void dc_deinit(void);

static inline int decoder_is_idle(void)
{
	return dc.state == DECODE_STATE_STOP &&
		dc.command != DECODE_COMMAND_START;
}

static inline int decoder_is_starting(void)
{
	return dc.command == DECODE_COMMAND_START ||
		dc.state == DECODE_STATE_START;
}

static inline Song *decoder_current_song(void)
{
	if (dc.state == DECODE_STATE_STOP ||
	    dc.error != DECODE_ERROR_NOERROR)
		return NULL;

	return dc.current_song;
}

void dc_command_wait(Notify *notify);

void dc_start(Notify *notify, Song *song);

void dc_start_async(Song *song);

void dc_stop(Notify *notify);

int dc_seek(Notify *notify, double where);

#endif
