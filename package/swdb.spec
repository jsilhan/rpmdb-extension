%global gitrev f5edea1

Name:       swdb
Version:    0.1.0
Release:    1%{?dist}
Summary:    Library for Package Managers for storing additional package related information
Group:      System Environment/Libraries
License:    LGPLv2+
URL:        https://github.com/jsilhan/swdb
Source0:    swdb-%{gitrev}.tar.xz
BuildRequires:  sqlite-devel
BuildRequires:  gtest-devel
Requires:   sqlite-devel

%description
Library for Package Managers for storing additional package related information


%build
%cmake .
make %{?_smp_mflags}

%check
make ARGS="-V" test

%install
make install DESTDIR=$RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/libswdb.so.*

%files devel
%{_libdir}/libswdb.so
%{_includedir}/swdb/

%changelog
