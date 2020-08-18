#File makes setup related for libxml2

git clone https://gitlab.gnome.org/GNOME/libxml2
cd libxml2

# build:
./autogen.sh  # version currently dont not have ./configure
              # May have to be changed to ./configure in future versions
make


