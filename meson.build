

project('initup', 'c', version: '0.0.1', default_options: ['b_ndebug=if-release'])

configure_file(
  output: 'config.h',
  configuration: {
    'PROJECT_VERSION': '"' + meson.project_version() + '"'
  }
)

executable(
  meson.project_name() + '-init',
  'src/initup-init.c', 'src/utils.c', 'src/mountentry.c', 'src/routines.c',
  link_args: ['-static'], install: true, install_dir: '/sbin')

executable(
  meson.project_name() + '-gen',
  'initup-gen/src/initup-gen.c', 'initup-gen/src/cpio.c',
  install: true, install_dir: '/bin'
)

executable(
  meson.project_name() + '-serviced',
  'initup-serviced/src/initup-serviced.c', 'initup-serviced/src/service.c', 'initup-serviced/src/linked_list.c',
  install: true, install_dir: '/sbin'
)