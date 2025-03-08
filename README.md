
# InitUp

InitUp is an init system aiming to be efficient, small and simple.

# Init Process

1) Mount the entire filesystem (as specified in /etc/fstab)
2) Discard initramfs and switch to the actual filesystem
3) Start /sbin/init (Which should be initup-serviced) as PID 1

# Building

**Comming soon!**

# Installing

The below command will generate an initrd containing:
* initup-init
* /etc/fstab

```bash
initup-gen -o /boot/initrd
rm /sbin/init
ln -s /sbin/initup-serviced /sbin/init
```


# initup-init

The init system bootstrapper, mounts the whole root filesystem and passes
control to /sbin/init

Steps:
1) mount /etc/fstab entries under a prefix directory (make a sysroot)
2) remove everything under initramfs (free unused space)
3) mount-move /prefix to /
4) chroot to ensure access to /
5) Execute the new /sbin/init as PID 1

# initup-gen

Initramfs generator, bundles /etc/fstab and initup-init to a cpio archive
to be used as initramfs

# initup-serviced

Service Manager. Automatically start services and only quit if all services complete.

# initup-start

Start an initup service

# initup-stop

Stop an initup service

# /etc/fstab option support

We currently support the following options:

| Default  | Activator | Deactivator | Description |
|----------|-----------|-------------|--------------------------------------------------------------|
| defaults | defaults  |             | Specifically setting this option helps save a few CPU cycles |
| auto     | auto      | noauto      | Wether to automatically mount this entry at boot             |
| rw       | rw        | ro          | Wether to mount as read-write or readonly                    |
| exec     | exec      | noexec      | Wether to allow execution of files in this mount             |
| suid     | suid      | nosuid      | Wether to allow the suid-bit in this mount                   |
| user     | user      | nouser      | Wether to allow users to mount this mount or not             |

# Service format

A service is basically a directory with some scripts/programs inside.

services directory: `/etc/initup/services/`

Possible executables:

* `autostart` - Automatically executed at boot

Example `vt-launcher` service:

File `/etc/initup/services/vt-launcher/autostart`:
```sh
#!/bin/sh
openvt -c 1 -- getty tty1
openvt -c 2 -- getty tty2
openvt -c 3 -- getty tty3
openvt -c 4 -- getty tty4
openvt -c 5 -- getty tty5
openvt -c 6 -- getty tty6
exec openvt -c 7 -- getty tty7
```