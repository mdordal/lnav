/**
 * Copyright (c) 2013, Timothy Stack
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file state-extension-functions.cc
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include <string>

#include "sqlite3.h"

#include "lnav.hh"
#include "lnav_log.hh"
#include "sql_util.hh"
#include "sqlite-extension-func.h"

static void sql_log_top_line(sqlite3_context *context,
                             int argc, sqlite3_value **argv)
{
    sqlite3_result_int64(context,
                         (int64_t)lnav_data.ld_views[LNV_LOG].get_top());
}

static void sql_log_top_datetime(sqlite3_context *context,
                                 int argc, sqlite3_value **argv)
{
    char buffer[64];

    require(argc == 0);

    sql_strftime(buffer, sizeof(buffer),
                 lnav_data.ld_top_time,
                 lnav_data.ld_top_time_millis);
    sqlite3_result_text(context, buffer, strlen(buffer), SQLITE_TRANSIENT);
}

int state_extension_functions(const struct FuncDef **basic_funcs,
                              const struct FuncDefAgg **agg_funcs)
{
    static const struct FuncDef datetime_funcs[] = {
        { "log_top_line", 0, 0, SQLITE_UTF8, 0, sql_log_top_line },
        { "log_top_datetime", 0, 0, SQLITE_UTF8, 0, sql_log_top_datetime },

        { NULL }
    };

    *basic_funcs = datetime_funcs;

    return SQLITE_OK;
}
