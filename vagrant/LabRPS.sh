#!/bin/bash

# Vagrant provisioning script to build up LabRPS based on Linux Ubuntu
# (c) 2016 Jean-Marie Verdun / vejmarie (vejmarie@ruggedpod.qyshare.com)
# Released under GPL v2.0
# Provided without any warranty
# Warning: compilation time is long quite long
# Must add the autlogin script
# Must add a lightdm start / reboot ?

LABRPS_GIT="https://github.com/vejmarie/LabRPS"
LABRPS_BRANCH=""

function create_deb {
rm -rf /tmp/$1-$2
mkdir /tmp/$1-$2
mkdir /tmp/$1-$2/DEBIAN
echo "Package:$1" >> /tmp/$1-$2/DEBIAN/control
echo "Version:$2" >> /tmp/$1-$2/DEBIAN/control
echo "Section:base" >> /tmp/$1-$2/DEBIAN/control
echo "Priority:optional" >> /tmp/$1-$2/DEBIAN/control
echo "Architecture:amd64" >> /tmp/$1-$2/DEBIAN/control
echo "Depends:"$3 >> /tmp/$1-$2/DEBIAN/control
echo "Maintainer:vejmarie@ruggedpod.qyshare.com" >> /tmp/$1-$2/DEBIAN/control
echo "Homepage:http://ruggedpod.qyshare.com" >> /tmp/$1-$2/DEBIAN/control
echo "Description:TEST PACKAGE" >> /tmp/$1-$2/DEBIAN/control
file_list=`ls -ltd $(find /opt/local/LabRPS-0.17) | awk '{ print $9}'`
for file in $file_list
do
  is_done=`cat /tmp/stage | grep $file`
if [ "$is_done" == "" ]
then
# The file must be integrated into the new deb
    cp --parents $file /tmp/$1-$2
    echo $file >> /tmp/stage
fi    
done
current_dir=`pwd`
cd /tmp
dpkg-deb --build $1-$2
mv $1-$2.deb $1-$2_trusty-amd64.deb
cd $current_dir
}

# If we are running ubuntu we must know if we are under Xenial (latest release) or Trusty which
# doesn't support the same package name

is_ubuntu=`lsb_release -a | grep -i Distributor | awk '{ print $3}'`
if [ "$is_ubuntu" == "Ubuntu" ]
then
	ubuntu_version=`lsb_release -a | grep -i codename | awk '{ print $2 }'`
        if [ "$ubuntu_version" == "xenial" ]
        then
	package_list="         doxygen                          \
                               libboost1.58-dev                 \
                               libboost-filesystem1.58-dev      \
                               libboost-program-options1.58-dev \
                               libboost-python1.58-dev          \
                               libboost-regex1.58-dev           \
                               libboost-signals1.58-dev         \
                               libboost-system1.58-dev          \
                               libboost-thread1.58-dev          \
                               libcoin80v5                      \
                               libcoin80-dev                    \
                               libeigen3-dev                    \
                               libpyside-dev                    \
                               libqtcore4                       \
                               libshiboken-dev                  \
                               libxerces-c-dev                  \
                               libxmu-dev                       \
                               libxmu-headers                   \
                               libxmu6                          \
                               libxmuu-dev                      \
                               libxmuu1                         \
                               pyside-tools                     \
                               python-dev                       \
                               python-pyside                    \
                               python-matplotlib                \
                               qt4-dev-tools                    \
                               qt4-qmake                        \
			       libqtwebkit-dev			\
                               shiboken                         \
                               calculix-ccx                     \
                               swig"
        else
	ubuntu_version="unknown"
	package_list="	       doxygen                          \
                               libboost1.55-dev                 \
                               libboost-filesystem1.55-dev      \
                               libboost-program-options1.55-dev \
                               libboost-python1.55-dev          \
                               libboost-regex1.55-dev           \
                               libboost-signals1.55-dev         \
                               libboost-system1.55-dev          \
                               libboost-thread1.55-dev          \
                               libcoin80                        \
                               libcoin80-dev                    \
                               libeigen3-dev                    \
                               libpyside-dev                    \
                               libqtcore4                       \
                               libshiboken-dev                  \
                               libxerces-c-dev                  \
                               libxmu-dev                       \
                               libxmu-headers                   \
                               libxmu6                          \
                               libxmuu-dev                      \
                               libxmuu1                         \
                               pyside-tools                     \
                               python-dev                       \
                               python-pyside                    \
                               python-matplotlib                \
                               qt4-dev-tools                    \
                               qt4-qmake                        \
                               shiboken                         \
                               swig"
       fi
fi
sudo apt-get update
sudo apt-get install -y dictionaries-common
sudo apt-get install -y $package_list
sudo apt-get install -y python-pivy
sudo apt-get install -y git
sudo apt-get install -y cmake
sudo apt-get install -y g++
sudo apt-get install -y libfreetype6-dev
sudo apt-get install -y tcl8.5-dev tk8.5-dev
sudo apt-get install -y libtogl-dev
sudo apt-get install -y libhdf5-dev
sudo apt-get install -y xfce4 xfce4-goodies
sudo apt-get install -y xubuntu-default-settings
sudo apt-get install -y lightdm
sudo apt-get install -y automake
sudo apt-get install -y libcanberra-gtk-module
sudo apt-get install -y libcanberra-gtk3-module overlay-scrollbar-gtk2 unity-gtk-module-common libatk-bridge2.0-0 unity-gtk2-module libatk-adaptor
myversion=`lsb_release -a | grep -i Distributor | awk '{ print $3}'`
if [ "$myversion" != "Ubuntu" ]
then
sudo cat /etc/lightdm/lightdm.conf | sed 's/\#autologin-user=/autologin-user=vagrant/' > /tmp/lightdm.conf
sudo cp /tmp/lightdm.conf /etc/lightdm/lightdm.conf
sudo /etc/init.d/lightdm start
sudo apt-get install libmed
sudo apt-get install libmedc
sudo apt-get install -y libmed-dev
sudo apt-get install -y libmedc-dev
else
cat <<EOF >& /tmp/lightdm.conf
[SeatDefaults]
user-session=xfce
autologin-session=xfce
autologin-user=ubuntu
autologin-user-timeout=0
greeter-session=xfce
pam-service=lightdm-autologin
EOF
sudo cp /tmp/lightdm.conf /etc/lightdm/
sudo rm /etc/lightdm/lightdm-gtk-greeter.conf
sudo /etc/init.d/lightdm start &

# Building MED

git clone https://github.com/vejmarie/libMED.git
cd libMED
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/local/LabRPS-0.17 ..
make -j 2
sudo make install
cd ../..
fi

# I must create the package

create_deb MED 3.10 ""


#building LabRPS

git clone $LABRPS_GIT
cd LabRPS
git checkout $LABRPS_BRANCH
cd ..
mkdir build
cd build
cmake ../LabRPS -DCMAKE_INSTALL_PREFIX:PATH=/opt/local/LabRPS-0.17 -DBUILD_ASSEMBLY=1 -DBUILD_FEM=1 -DBUILD_FEM_VTK=1 -DBUILD_FEM_NETGEN=1 -DCMAKE_CXX_FLAGS="-DNETGEN_V5"
make -j 2
make install
create_deb LabRPS 0.17 "med (>= 3.10)"
source_dir=`pwd`
cd /tmp
mkdir deb
mv *.deb deb
cd deb
dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz

if [ "$ubuntu_version" == "xenial" ]
then
#Let's build the snap
	mkdir snap
	cd snap
	cp -Rf $source_dir/LabRPS/vagrant/Xenial .
	cd Xenial
        apt-get install -y snapcraft
	./generate_yaml.sh
	snapcraft
	mv labrps_0.17_amd64.snap /tmp
fi
mkdir $source_dir/Results
mv /tmp/*.deb $source_dir/Results
if [ -f "/tmp/labrps_0.17_amd64.snap" ]
then
	mv /tmp/labrps_0.17_amd64.snap $source_dir/Results
fi
