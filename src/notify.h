/* the Music Player Daemon (MPD)
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

#ifndef NOTIFY_H
#define NOTIFY_H

#include "os_compat.h"

typedef struct notify {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int pending;
} Notify;

void notify_init(struct notify *notify);

void notify_deinit(struct notify *notify);

/**
 * The thread which shall be notified by this object must call this
 * function before any notify_wait() invocation.  It locks the mutex.
 */
void notify_enter(struct notify *notify);

/**
 * Neutralize notify_leave().
 */
void notify_leave(struct notify *notify);

/**
 * Wait for a notification.  Return immediately if we have already
 * been notified since we last returned from notify_wait().
 */
void notify_wait(struct notify *notify);

/**
 * Notify the thread.  This function never blocks.
 */
void notify_signal(struct notify *notify);

/**
 * Notify the thread synchonously, i.e. wait until it has received the
 * notification.
 */
void notify_signal_sync(struct notify *notify);

#endif
