# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

# The loopback network interface
echo "auto lo" > interfaces
echo "iface lo inet loopback" >> interfaces

# The primary network interface
echo "auto eth0" >> interfaces
echo "iface eth0 inet dhcp" >> interfaces

sudo cp interfaces /etc/network/interfaces
sudo reboot
