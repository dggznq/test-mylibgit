/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#ifndef INCLUDE_mwindow__
#define INCLUDE_mwindow__

#include "common.h"

#include "map.h"
#include "vector.h"

extern git_mutex git__mwindow_mutex;

typedef struct git_mwindow {
	struct git_mwindow *next;
	git_map window_map;
	off64_t offset;
	size_t last_used;
	size_t inuse_cnt;
} git_mwindow;

typedef struct git_mwindow_file {
	git_mwindow *windows;
	int fd;
	off64_t size;
} git_mwindow_file;

typedef struct git_mwindow_ctl {
	size_t mapped;
	unsigned int open_windows;
	unsigned int mmap_calls;
	unsigned int peak_open_windows;
	size_t peak_mapped;
	size_t used_ctr;
	git_vector windowfiles;
} git_mwindow_ctl;

int git_mwindow_contains(git_mwindow *win, off64_t offset);
int git_mwindow_free_all(git_mwindow_file *mwf); /* locks */
int git_mwindow_free_all_locked(git_mwindow_file *mwf); /* run under lock */
unsigned char *git_mwindow_open(git_mwindow_file *mwf, git_mwindow **cursor, off64_t offset, size_t extra, unsigned int *left);
int git_mwindow_file_register(git_mwindow_file *mwf);
void git_mwindow_file_deregister(git_mwindow_file *mwf);
void git_mwindow_close(git_mwindow **w_cursor);

extern int git_mwindow_global_init(void);

struct git_pack_file; /* just declaration to avoid cyclical includes */
int git_mwindow_get_pack(struct git_pack_file **out, const char *path);
int git_mwindow_put_pack(struct git_pack_file *pack);

#endif
