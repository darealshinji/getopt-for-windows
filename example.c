/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Carsten Janssen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE
 */

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include "getopt.c"


#ifdef _UNICODE

/* using wide char API, set output mode to _O_U16TEXT and print */
static void print_as_u16(const wchar_t *fmt, const wchar_t *arg)
{
    int oldmode = _setmode(_fileno(stdout), _O_U16TEXT);
    wprintf(fmt, arg);
    _setmode(_fileno(stdout), oldmode);
}

#else

static wchar_t *convert_str_to_wcs(const char *str)
{
    size_t len, n;
    wchar_t *buf;

    if (!str) return NULL;

    if (mbstowcs_s(&len, NULL, 0, str, 0) != 0 || len == 0) {
        return NULL;
    }

    buf = malloc((len + 1) * sizeof(wchar_t));
    if (!buf) return NULL;

    if (mbstowcs_s(&n, buf, len+1, str, len) != 0 || n == 0) {
        free(buf);
        return NULL;
    }

    buf[len] = '\0';

    return buf;
}

/* convert char string to wchar_t, set output mode to _O_U16TEXT and print */
static void print_as_u16(const wchar_t *fmt, const char *arg)
{
    int oldmode = _setmode(_fileno(stdout), _O_U16TEXT);

    wchar_t *wstr = convert_str_to_wcs(arg);
    wprintf(fmt, wstr);
    free(wstr);

    _setmode(_fileno(stdout), oldmode);
}

#endif


int _tmain(int argc, TCHAR **argv)
{
    int option_index = 0;

    static struct _toption long_options[] = {
        { _T("help"),    no_argument,       0,  _T('h') },
        { _T("input"),   required_argument, 0,  _T('i') },
        { _T("foo"),     no_argument,       0,  _T('f') },
        { _T("bar"),     no_argument,       0,  _T('b') },
        { NULL,          0,                 0,      0   }
    };

    if (sizeof(TCHAR) > 1) {
        puts("wide characters API");
    } else {
        puts("narrow characters API");
    }

    while (1)
    {
        /* short options are "hi:fb";
         * the colon after "i" means it requires an argument */
        int c = _tgetopt_long(argc, argv, _T("hi:fb"), long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c)
        {
        /* print help */
        case _T('h'):
            puts("options: -h -i<file> -f -b");
            return 0;

        /* flag */
        case _T('f'):
            puts("foo");
            break;

        /* flag */
        case _T('b'):
            puts("bar");
            break;

        /* option with an argument;
         * print as UTF-16 to not mess up multibyte characters */
        case _T('i'):
            print_as_u16(L"input: %ls\n", _toptarg);
            break;

        /* getopt printed an error message */
        case _T('?'):
            /* we cannot use '?' as a valid option but
             * we can at least print a hint */
            if (optopt == _T('?')) {
                puts("Did you mean `--help' ?");
            }
            return 1;

        default:
            printf("error: getopt returned unknown code: 0x%x\n", c);
            return 1;
        }
    }

    puts("exit 0");

    return 0;
}
