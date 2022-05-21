pkgname="privacy-protection-messenger-qt"
pkgver=2.5
pkgrel=0
pkgdesc="Privacy Protection Messenger Qt GUI"
author="imperzer0"
url="https://github.com/$author/$pkgname"
arch=("x86_64")
license=('GPL3')
depends=("qt6-base" "privacy-protection-messenger>=1.3-4")
makedepends=("cmake>=3.0" "qt6-base" "execute-process-linux=2.5-0")

_srcprefix="local:/"
_languages=("uk_UA" "ru_UA" "ru_RU")
_libfiles=("CMakeLists.txt" "main.cpp" "mainwindow.cpp" "mainwindow.h" "call_backend.hpp"
          "mainwindow.ui" "resources.qrc" "$pkgname.desktop" "icon.svg")

for _lang in ${_languages[@]}
{
    _libfiles=(${_libfiles[@]} $pkgname"_"$_lang".ts")
}

for _libfile in ${_libfiles[@]}
{
    source=(${source[@]} "$_srcprefix/$_libfile")
}

for _libfile in ${_libfiles[@]}
{
    md5sums=(${md5sums[@]} "SKIP")
}

build()
{
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .
	make
}

package()
{
	install -Dm755 $pkgname "$pkgdir/usr/bin/$pkgname"
	install -Dm755 "$pkgname.desktop" "$pkgdir/usr/share/applications/$pkgname.desktop"
	install -Dm644 "icon.svg" "$pkgdir/usr/share/icons/$pkgname/icon.svg"
}

notarch_package()
{
	cp -f $pkgname "$pkgdir/usr/bin/$pkgname"
	chmod 755 "$pkgdir/usr/bin/$pkgname"

	cp -f "$pkgname.desktop" "$pkgdir/usr/share/applications/$pkgname.desktop"
	chmod 755 "$pkgdir/usr/share/applications/$pkgname.desktop"

	mkdir -pm644 "$pkgdir/usr/share/icons/"
	cp -f "icon.svg" "$pkgdir/usr/share/icons/icon.svg"
	chmod 644 "$pkgdir/usr/share/icons/icon.svg"
}
