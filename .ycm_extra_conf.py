def Settings(**kwargs):
    return {
            'flags': [
                '-x', 'c++',
                '-std=c++17',
                '-Wall', '-Wextra', '-Wpedantic'
                '-D_UNICODE', '-D__WXGTK3__', '-D__WXGTK__',
                '-Iinclude',
                '-Idepend/wxWidgets/include',
                '-Ibuild/lib/wx/include/gtk3-unicode-3.1',
                '-Idepend/docopt',
                '-Idepend/googletest/googlemock/include',
                '-Idepend/googletest/googletest/include',
                ]
            }

