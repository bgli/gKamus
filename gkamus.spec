%define version 0.3

Name: gkamus
Version: %{version}
Release: 1
Summary: Kamus Bahasa Inggris - Indonesia
Group: Applications/Utility
License: GPLv2
URL: http://gkamus.sourceforge.net
Vendor: http://gkamus.sourceforge.net
Packager: Ardhan Madras <ajhwb@knac.com>
Source: gkamus-%{version}.tar.gz

%description
Kamus sederhana bahasa Inggris - Indonesia, ditulis dengan GTK+.

%prep
%setup -q

%build
./configure --prefix=$RPM_BUILD_ROOT/usr
make

%install
rm -rf $RPM_BUILD_ROOT
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL NEWS README TODO
/usr/bin/gkamus
/usr/share/applications/gkamus.desktop
/usr/share/gkamus/gkamus.ui
/usr/share/gkamus/gkamus-en.dict
/usr/share/gkamus/gkamus-id.dict
/usr/share/gkamus/irregular-verbs
/usr/share/pixmaps/gkamus.png
