/*
 * Copyright (C) 2003-2014 The Music Player Daemon Project
 * http://www.musicpd.org
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
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "DetachedSong.hxx"
#include "db/LightSong.hxx"
#include "util/UriUtil.hxx"
#include "fs/Traits.hxx"

DetachedSong::DetachedSong(const LightSong &other)
	:uri(other.GetURI().c_str()),
	 real_uri(other.real_uri != nullptr ? other.real_uri : ""),
	 tag(*other.tag),
	 mtime(other.mtime),
	 start_time(SongTime::FromMS(other.start_ms)),
	 end_time(SongTime::FromMS(other.end_ms)) {}

DetachedSong::~DetachedSong()
{
	/* this destructor exists here just so it won't  inlined */
}

bool
DetachedSong::IsRemote() const
{
	return uri_has_scheme(GetRealURI());
}

bool
DetachedSong::IsAbsoluteFile() const
{
	return PathTraitsUTF8::IsAbsolute(GetRealURI());
}

bool
DetachedSong::IsInDatabase() const
{
	/* here, we use GetURI() and not GetRealURI() because
	   GetRealURI() is never relative */

	const char *_uri = GetURI();
	return !uri_has_scheme(_uri) && !PathTraitsUTF8::IsAbsolute(_uri);
}

double
DetachedSong::GetDuration() const
{
	if (end_time.IsPositive())
		return (end_time - start_time).ToDoubleS();

	return tag.time - start_time.ToDoubleS();
}
