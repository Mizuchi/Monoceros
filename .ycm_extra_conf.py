def FlagsForFile(filename, **kwargs):
    return {
        'flags': ['-stdlib=libc++', '-std=c++1y', '-Wall', '-Wextra', '-Werror'],
        'do_cache': True
    }
