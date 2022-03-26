pkgname="privacy-protection-messenger-qt"
epoch=1
pkgver=0
pkgrel=0
pkgdesc="privacy protection messenger Qt GUI"
arch=("x86_64")
url="https://github.com/imperzer0/$pkgname"
license=('GPL')
depends=("qt6-base")
makedepends=("cmake>=3.0" "qt6-base")
languages=("uk_UA")
libfiles=("CMakeLists.txt" "main.cpp" "mainwindow.cpp" "mainwindow.h" "mainwindow.ui" "palette.xml"
          "resources.qrc")

for lang in ${languages[@]}
{
    libfiles=(${libfiles[@]} $pkgname"_"$lang".ts")
}

for libfile in ${libfiles[@]}
{
    source=(${source[@]} "local://$libfile")
}

for libfile in ${libfiles[@]}
{
    md5sums=(${md5sums[@]} "SKIP")
}

build()
{
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .
	cmake --build . --target $pkgname"_lrelease"
	make
}

package()
{
	install -Dm755 $pkgname "$pkgdir/usr/bin/$pkgname"
}
