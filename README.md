OpenBSD's implementation of `getopt(3)` ported to be usable with
Microsoft's compiler (MSVC) and MinGW.

Both "narrow" and "wide" characters are supported using `TCHAR`:

```
TCHAR macro           _UNICODE not defined    _UNICODE defined

_tgetopt              getopt                  _wgetopt
_tgetopt_long         getopt_long             _wgetopt_long
_tgetopt_long_only    getopt_long_only        _wgetopt_long_only
_toption              option                  _woption
_toptarg              optarg                  _woptarg
```

I hope that this will be useful to some people since MSVC lacks getopt and
MinGW doesn't have a wide characters version of it.
This code is released under permissive licenses (see source code for details).

