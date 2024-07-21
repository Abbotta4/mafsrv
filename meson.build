project(
    'mafsrv',
    'cpp',
    version: '0.5.0',
    default_options: [
        'cpp_std=c++17',
	'buildtype=release',
    ],
    meson_version: '>=0.55.0'
    )
inc = include_directories('inc')
src = [
    'src/main.cpp',
    'src/pgdb.cpp',
    'src/restserver.cpp'
    ]
dep = [
    dependency('libpistache', fallback: 'pistache', default_options: ['PISTACHE_USE_SSL=true']),
    dependency('sqlpp11', fallback: 'sqlpp11', default_options: ['postgresql_connector=true']),
    dependency('jwtpp', fallback: 'jwtpp'),
    dependency('jsoncpp', fallback: 'jsoncpp'),
    dependency('openssl', fallback: 'openssl')
    ]
executable('mafsrv', sources: src, include_directories: inc, dependencies: dep)