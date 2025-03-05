

project('initup', 'c', version: '0.0.1', default_options: ['b_ndebug=if-release'])

configure_file(
  output: 'config.h',
  configuration: {
    'PROJECT_VERSION': '"' + meson.project_version() + '"'
  }
)

executable(
  meson.project_name() + '-init',
  'src/initup-init.c', 'src/utils.c', 'src/mountentry.c',
  link_args: ['-static'])