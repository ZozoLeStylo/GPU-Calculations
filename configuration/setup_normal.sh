#! /bin/sh

services=("abrt-journal-core.service" "abrt-oops.service" "abrt-vmcore.service" "abrt-xorg.service" "abrt.service" "accounts-daemon.service" "avahi-daemon.service" "bluetooth.service" "flatpak-add-fedora-repos.service" "gdm.service" "iscsi.onboot.service" "iscsi.starter.service" "ModemManager.service" "NetworkManager-wait-online.service")

sudo systemctl set-default graphical.target

for service in "${services[@]}"
do
	sudo systemctl enable $service
done

sudo reboot

