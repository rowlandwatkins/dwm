# $Id: PKGBUILD 3138 2009-09-28 09:21:30Z spupykin $
# Maintainer: Patrice Peterson <runiq at archlinux dot us>
# Contributor: Sergej Pupykin <pupykin.s+arch at gmail dot com>
# Contributor: Dag Odenhall <dag.odenhall at gmail dot com>
# Contributor: Grigorios Bouzakis <grbzks at gmail dot com>

pkgname=dwm
pkgver=5.9
pkgrel=1
pkgdesc="A dynamic window manager for X"
url="http://dwm.suckless.org"
arch=('i686' 'x86_64')
license=('MIT')
depends=('libxinerama')
options=(zipman)
conflicts=('dwm-pango')
install=dwm.install
source=(http://dl.suckless.org/dwm/dwm-$pkgver.tar.gz
        config.h
        01-dwm-5.9-pertag2.diff
        02-dwm-5.9-scratchpad-stay.diff
        03-dwm-5.9-statuscolors.diff
		04-dwm-5.9-xbm_layout_icons.diff
		05-dwm-5.9-cflags.diff
        pidgin-grid.c
        nbstack.c
        push.c)

build() {
  cd $srcdir/dwm-$pkgver

  patch < ../01-dwm-5.9-pertag2.diff
  patch < ../02-dwm-5.9-scratchpad-stay.diff
  patch < ../03-dwm-5.9-statuscolors.diff
  patch < ../04-dwm-5.9-xbm_layout_icons.diff
  patch < ../05-dwm-5.9-cflags.diff
  cp $srcdir/config.h config.h
  cp $srcdir/pidgin-grid.c pidgin-grid.c
  cp $srcdir/nbstack.c nbstack.c
  cp $srcdir/push.c push.c

  make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11 || return 1
  make PREFIX=/usr DESTDIR=$pkgdir install || return 1

  install -m644 -D LICENSE $pkgdir/usr/share/licenses/$pkgname/LICENSE && \
  install -m644 -D README $pkgdir/usr/share/doc/$pkgname/README
}
