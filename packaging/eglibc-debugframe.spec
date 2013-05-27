%define debug_package %{nil}
%define __strip /bin/true

Name:		eglibc-debugframe
Summary:	libc debug library with .debug_frame section only
Version:	2.13
Release:	1
License:	LGPLv2+ and LGPLv2+ with exceptions and GPLv2+
Group:		Development/Libraries
Source0:	%{name}-${version}.tar.gz
BuildRequires:	eglibc-debuginfo = %{version}
BuildRequires:	binutils

%description
The package contains the libc debug library with .debug_frame section only
which can be helpful during signal frame backtrace.

%prep
%setup -q 

%install
mkdir -p %{buildroot}/%{_prefix}/%{_lib}/debug/%{_lib}
objcopy -j .note.gnu.build-id -j .debug_frame %{_prefix}/%{_lib}/debug/%{_lib}/libc-%{version}.so.debug %{buildroot}%{_prefix}/%{_lib}/debug/%{_lib}/libc-%{version}.so.debug

%files
%defattr(-,root,root)
%{_prefix}/%{_lib}/debug/%{_lib}/*.so.debug

