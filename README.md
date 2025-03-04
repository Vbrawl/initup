
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

# initup-gen

Initramfs generator, bundles /etc/fstab and initup-init to a cpio archive
to be used as initramfs

# initup-serviced

Service Manager. Start, stop and overall manage initup services

# initup-start

Start an initup service

# initup-stop

Stop an initup service
