%global gitrev f5edea1
%global libsolv_version 0.4.0-2

Name:       swdb
Version:    0.1.0
Release:    1%{?dist}
Summary:    Library providing simplified C and Python API to libsolv
Group:      System Environment/Libraries
License:    LGPLv2+
URL:        https://github.com/akozumpl/swdb
# git clone https://github.com/akozumpl/swdb.git && cd swdb && package/archive
Source0:    swdb-%{gitrev}.tar.xz
BuildRequires:  sqlite-devel
Requires:   sqlite-devel
# prevent provides from nonstandard paths:
%filter_provides_in %{python_sitearch}/.*\.so$
%filter_provides_in %{python3_sitearch}/.*\.so$
# filter out _swdb_testmodule.so DT_NEEDED _swdbmodule.so:
%filter_requires_in %{python_sitearch}/swdb/test/.*\.so$
%filter_requires_in %{python3_sitearch}/swdb/test/.*\.so$
%filter_setup

%description
A Library providing simplified C and Python API to libsolv.

%package devel
Summary:    A Library providing simplified C and Python API to libsolv
Group:      Development/Libraries
Requires:   swdb%{?_isa} = %{version}-%{release}
Requires:   libsolv-devel

%description devel
Development files for swdb.

%package -n python-swdb
Summary:    Python 2 bindings for the swdb library
Group:      Development/Languages
BuildRequires:  python2-devel
BuildRequires:  python-nose
BuildRequires:  python-sphinx
Requires:   %{name}%{?_isa} = %{version}-%{release}

%description -n python-swdb
Python 2 bindings for the swdb library.

%package -n python3-swdb
Summary:    Python 3 bindings for the swdb library
Group:      Development/Languages
BuildRequires:  python3-devel
BuildRequires:  python3-nose
BuildRequires:  python3-sphinx
Requires:   %{name}%{?_isa} = %{version}-%{release}

%description -n python3-swdb
Python 3 bindings for the swdb library.

%prep
%setup -q -n swdb

rm -rf py3
mkdir ../py3
cp -a . ../py3/
mv ../py3 ./

%build
%cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .
make %{?_smp_mflags}
make doc-man

pushd py3
%cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPYTHON_DESIRED:str=3.
make %{?_smp_mflags}
make doc-man
popd

%check
make ARGS="-V" test
./py3/tests/python/tests/run_nosetests.sh

%install
make install DESTDIR=$RPM_BUILD_ROOT
pushd py3
make install DESTDIR=$RPM_BUILD_ROOT
popd

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%doc COPYING README.rst
%{_libdir}/libswdb.so.*

%files devel
%{_libdir}/libswdb.so
%{_libdir}/pkgconfig/swdb.pc
%{_includedir}/swdb/
%{_mandir}/man3/swdb.3.gz

%files -n python-swdb
%{python_sitearch}/

%files -n python3-swdb
%{python3_sitearch}/
%exclude %{python3_sitearch}/swdb/__pycache__
%exclude %{python3_sitearch}/swdb/test/__pycache__

%changelog
