

project('initup', 'c', version: '0.0.1')

configure_file(
  output: 'config.h',
  configuration: {
    'PROJECT_VERSION': '"' + meson.project_version() + '"'
  }
)

executable(
  meson.project_name() + '-init',
  'src/initup-init.c', 'src/utils.c',
  link_args: ['-static'])