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

#include <stdio.h>
#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include "getopt.c"

#ifdef _UNICODE
#define _TS "%ls"
#else
#define _TS "%s"
#endif


void print_help()
{
    _tprintf(_T("options:\n  -? -h\n  -i<file> -f -b\n"));
}

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

    _tprintf(_T(_TS " characters API\n"),
        (sizeof(TCHAR) > 1) ? _T("wide") : _T("narrow"));

    while (1)
    {
        /**
         * colon after "i" in short options list means it requires an argument;
         * '?' is listed as a valid option, we use optopt to distinguish it from an error
         */
        int c = _tgetopt_long(argc, argv, _T("?hi:fb"), long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c)
        {
        /* print help */
        case _T('h'):
            print_help();
            return 0;

        /* flag */
        case _T('f'):
            _tprintf(_T("foo\n"));
            break;

        /* flag */
        case _T('b'):
            _tprintf(_T("bar\n"));
            break;

        /* option with an argument */
        case _T('i'):
            _tprintf(_T("input: " _TS "\n"), _toptarg);
            break;

        /* getopt printed an error message */
        case _T('?'):
            if (optopt == _T('?')) {
                /* since '?' is among the short options there is no error
                 * message and '-?' can be treated as a valid option */
                print_help();
                return 0;
            }
            return 1;

        default:
            _tprintf(_T("error: getopt returned unknown code: 0x%x\n"), c);
            return 1;
        }
    }

    return 0;
}
