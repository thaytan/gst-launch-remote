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

struct LocalData {
  GstLaunchRemoteAppContext app_context;
  GstLaunchRemote *launch;
  GCond main_cond;
  GMutex main_lock;
};

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

static void
quit_cb (gpointer user_data)
{
  struct LocalData *app_data = (struct LocalData *)(user_data);

  printf ("Quitting\n");
  g_mutex_lock (&app_data->main_lock);
  g_cond_broadcast (&app_data->main_cond);
  g_mutex_unlock (&app_data->main_lock);
}

int
main (int argc, char ** argv)
{
  struct LocalData app_data;
  gchar *listen_mode = NULL;
  GError *err = NULL;
  GOptionContext *ctx;

  GOptionEntry options[] = {
    {"listen-mode", 0, 0, G_OPTION_ARG_STRING, &listen_mode,
        "listener mode - tcp:" G_STRINGIFY (PORT) " (default) or stdio", NULL},
    {NULL}
  };

  ctx = g_option_context_new (NULL);
  g_option_context_add_main_entries (ctx, options, NULL);
  g_option_context_add_group (ctx, gst_init_get_option_group ());

  if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
    g_print ("Error initializing: %s\n", GST_STR_NULL (err->message));
    g_option_context_free (ctx);
    g_clear_error (&err);
    return 1;
  }
  g_option_context_free (ctx);

  GST_DEBUG_CATEGORY_INIT (debug_category, "gst-remote-launch", 0,
      "GStreamer Remote Launch");

  g_cond_init (&app_data.main_cond);
  g_mutex_init (&app_data.main_lock);

  app_data.app_context.app = &app_data;
  app_data.app_context.set_message = send_message;
  app_data.app_context.set_current_position = set_current_position;
  app_data.app_context.initialized = initialized;
  app_data.app_context.media_size_changed = media_size_changed;
  app_data.app_context.quit = quit_cb;

  app_data.launch = gst_launch_remote_new (&app_data.app_context);

  g_mutex_lock (&app_data.main_lock);
  g_cond_wait (&app_data.main_cond, &app_data.main_lock);
  g_mutex_unlock (&app_data.main_lock);

  g_cond_clear (&app_data.main_cond);
  g_mutex_clear (&app_data.main_lock);

  gst_launch_remote_free (app_data.launch);

  return 0;
}
