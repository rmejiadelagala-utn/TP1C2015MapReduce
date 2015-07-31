echo "Ingresá la ip fija a usar:"
read ip

# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

# The loopback network interface
echo "auto lo" > interfaces
echo "iface lo inet loopback" >> interfaces

# The primary network interface
echo "auto eth0" >> interfaces
echo "iface eth0 inet static" >> interfaces
echo "	address 192.168.0.$ip"  >> interfaces
echo "	network 192.168.0.0" >> interfaces
echo "	netmask 255.255.255.0" >> interfaces
echo "	broadcast 192.168.0.255" >> interfaces
echo "	gateaway 192.168.0.1" >> interfaces

echo "dns-nameservers 8.8.8.8" >> interfaces
sudo cp interfaces /etc/network/interfaces
sudo reboot
