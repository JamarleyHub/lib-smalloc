# Maintainer: Chris Hölzer <jamarleydev at proton dot me>
pkgname='libsmalloc-git'
pkgver='r13.1327c69'
pkgrel=1
pkgdesc="Allocation management library for C"
arch=(x86_64)
url="https://gitlab.com/Jamarley/lib-smalloc"
license=('GPL')
depends=('glibc')
makedepends=('git' 'cmake')
provides=("${pkgname%-git}")
conflicts=("${pkgname%-git}")
source=("$pkgname::git+$url")
sha256sums=('SKIP')

pkgver() {
	cd "$pkgname"
	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
	cd "$pkgname"
	cmake -B cmake-build
    cmake --build cmake-build -j 6
}

package() {
	cd "$pkgname"
	sudo cmake --install cmake-build
}
