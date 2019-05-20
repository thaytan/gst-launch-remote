/* GStreamer
 *
 * Copyright (C) 2014 Sebastian Dröge <sebastian@centricular.com>
 * Copyright (C) 2019 Jan Schmidt <jan@centricular.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <gio/gio.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <pthread.h>

#include "../gst-launch-remote/gst-launch-remote.h"

GST_DEBUG_CATEGORY_STATIC (debug_category);
#define GST_CAT_DEFAULT debug_category

static void
send_message (const gchar * message, gpointer user_data)
{
}

static void
set_current_position (gint position, gint duration, gpointer user_data)
{
}

static void
media_size_changed (gint width, gint height, gpointer user_data)
{
}

static void
initialized (gpointer user_data)
{
    printf("Initialised\n");
}

int
main (int argc, char ** argv)
{
  GstLaunchRemoteAppContext app_context;
  GstLaunchRemote *launch;

  GST_DEBUG_CATEGORY_INIT (debug_category, "gst-remote-launch", 0,
      "GStreamer Remote Launch");

  app_context.app = NULL;
  app_context.set_message = send_message;
  app_context.set_current_position = set_current_position;
  app_context.initialized = initialized;
  app_context.media_size_changed = media_size_changed;

  launch = gst_launch_remote_new (&app_context);

  gst_launch_remote_free (launch);

  return 0;
}
