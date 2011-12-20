# $Id: PKGBUILD 3138 2009-09-28 09:21:30Z spupykin $
# Maintainer: Patrice Peterson <runiq at archlinux dot us>
# Contributor: Sergej Pupykin <pupykin.s+arch at gmail dot com>
# Contributor: Dag Odenhall <dag.odenhall at gmail dot com>
# Contributor: Grigorios Bouzakis <grbzks at gmail dot com>

pkgname=dwm
pkgver=6.0
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
        tilemovemouse.c
        im-grid.c
        togglefullscreen.c
        push.c)
_patches=(01-dwm-$pkgver-pertag2.diff
          02-dwm-$pkgver-scratchpad-stay.diff
          03-dwm-$pkgver-xbm_layout_icons.diff
          04-dwm-$pkgver-cflags.diff
          05-dwm-$pkgver-urgentcolor.diff)
source=(${source[@]} ${_patches[@]})


build() {
  cd $srcdir/dwm-$pkgver
  
  # patch time!
  for p in "${_patches[@]}"; do
    patch < ../$p || return 1
  done

  cp $srcdir/config.h config.h
  cp $srcdir/im-grid.c im-grid.c
  cp $srcdir/push.c push.c
  cp $srcdir/tilemovemouse.c tilemovemouse.c
  cp $srcdir/togglefullscreen.c togglefullscreen.c

  make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11 || return 1
  make PREFIX=/usr DESTDIR=$pkgdir install || return 1

  install -m644 -D LICENSE $pkgdir/usr/share/licenses/$pkgname/LICENSE && \
  install -m644 -D README $pkgdir/usr/share/doc/$pkgname/README
}
