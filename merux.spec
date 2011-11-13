# no post install
#%define __os_install_post %{nil}
# no strip
#%define __strip /bin/true
# no debug package
#%define debug_package %{nil}
# disable automatic dependency processing
#AutoReqProv: no

Name:           merux
Version:        0.0.1
Release:        1
License:        GPLv2.0 and above
Summary:        Mer bringup demo
Source0:        %{name}-%{version}.tar.gz
#BuildRequires:  pkgconfig(QtCore)
BuildRequires:  libqt-devel
BuildRequires: pkgconfig(QtOpenGL)
BuildRequires: pkgconfig(QtFeedback)

%description
# Add here description of the package.

%prep
%setup -q

%build
# Add here commands to configure the package.
#%qmake
#qmake -makefile -nocache QMAKE_STRIP=: PREFIX=%{_prefix}

# Add here commands to compile the package.
#make %{?jobs:-j%jobs}
#make %{?_smp_mflags}
make

%install
# Add here commands to install the package.
#%qmake_install
make install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root,-)
/opt/merux
