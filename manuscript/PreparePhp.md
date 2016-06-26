# Compile your own PHP

## Getting the source

First get the source. Most Linux distribution provide a source package
for the PHP version they package for you. Eg:

  - php7-dev on Debian
  - php-devel on CentOS

But, you can also get the source from Git and use that. I prefer this option.
There is a GitHub mirror of official PHP repository http://git.php.net/ at:

```bash
git clone https://github.com/php/php-src.git
cd php-src
```

Checkout the branch you want to work on.

```bash
git checkout PHP-7.0
git checkout PHP-7.0.8
git checkout PHP-7.1
git checkout master
```

If you are on master, this is bleeding edge. At this moment it points to the
development version of PHP-7.1. The PHP-7.0 will point to the latest PHP 7.0
release, with all patches that were merged after the release. If you want to
use a released versions, checkout as specific branch like PHP-7.0.8.

## Building a minimal version

The minimal instructions to compile a working PHP version are:

```bash
./buildconf --force

./configure \
    --enable-debug

make
```

The buildconf script makes sure your directory is cleaned up and any leftovers
of previous builds are removed.

The ```--enable-debug``` is optional. You need it to be able to properly debug
your extensions. When PHP is compiled with this flag, it will report memory leaks.
The flag also makes sure no debug symbols are stripped out and the binaries are
not optimized. With optimized binaries, tools like the GNU debugger (gdb) will
have a difficult time trying to translate things back to what was your original
code.

> **Efficiency tip:** make will use GCC to compile the code. There is a wrapper,
called "colorgcc" that colorizes the output with warning / error messages. It is
packaged on Ubuntu/Debian, When installed, GCC will automatically colorize it's
output.

The PHP binary will be in ```./sapi/cli/php```.

You can run make in parallel if you want to use more cores. Eg: ```make -j2```
for 2 using cores. Or use ```make -j`nproc` ``` as a shorthand. Note that this
does not make your build process scale linearly.

## About ZTS

Zend Thread Safety (ZTS) is a compile time configuration option. If you don't
have it, your PHP will be Not Thread Safe (NTS). If you want people with a ZTS
PHP version to be able to use your extension, you have to make your extension
thread safe.

Configure your PHP with ```--enable-maintainer-zts``` to build a ZTS version.
This flag will enable thread safety checks. These checks will also be applied on
your own extensions.

Consider that you need ZTS when the pthreads (POSIX threads) extension is used,
or when the webserver (Apache2 mpm-worker or IIS) uses PHP as a module. When
FastCGI / FPM is used, or Apache2 mpm-worker, you don't need ZTS because forking
(new processes) is used over threads.

NTS is the preferred way of running PHP. NTS also makes it a little bit easier
to write and debug your extensions.

You can check how your PHP was built with ```php -v```. It will show ZTS or NTS
on the first line of output. In older PHP versions, if neither of these is
displayed it will be NTS.

## Building a full version

A minimal PHP version will be good to test PHP core, but will get you stuck
pretty fast if you want to test a framework with the latest PHP release.

If you have never built PHP, how do you figure out what to include in your
configure string? Easy. The ```php-config``` binary from php development package
will display the --configure-options that were used. If you want to review all
possible options, use ```./configure --help```.

This is how I build my PHP, it's more or less what you get from your Linux distro.
Dependencies you need on Debian 8 are:

```bash
apt-get update

# Dependencies for building
apt-get install -y \
    make \
    autoconf \
    gcc \
    bison \
    g++

# Dependencies for the selected extensions
apt-get install -y \
    libxml2-dev \
    libbz2-dev \
    libcurl4-openssl-dev \
    libltdl-dev \
    libpng12-dev \
    libjpeg62-turbo-dev \
    libfreetype6-dev \
    libxpm-dev \
    libimlib2-dev \
    libicu-dev \
    libreadline6-dev \
    libmcrypt-dev \
    libxslt1-dev \
    libssl-dev
```

Compile and install PHP CLI and FPM. I build them separately because I like to
have different php.ini configurations for both of them.

```bash
./buildconf --force

# Prepare installation directories

mkdir -p /etc/php7.0/conf.d
mkdir -p /etc/php7.0/{cli,fpm}/conf.d
mkdir /usr/local/php7.0

# Default options

CONFIGURE_STRING="--prefix=/usr/local/php7.0 \
                  --enable-bcmath \
                  --with-bz2 \
                  --with-zlib \
                  --enable-zip \
                  --enable-calendar \
                  --enable-exif \
                  --enable-ftp \
                  --with-gettext \
                  --with-gd \
                  --with-jpeg-dir \
                  --with-png-dir \
                  --with-freetype-dir \
                  --with-xpm-dir \
                  --enable-mbstring \
                  --enable-mysqlnd \
                  --with-mysqli=mysqlnd \
                  --with-pdo-mysql=mysqlnd \
                  --with-openssl \
                  --enable-intl \
                  --enable-soap \
                  --with-readline \
                  --with-curl \
                  --with-mcrypt \
                  --with-xsl \
                  --disable-cgi"

# Options for development

CONFIGURE_STRING="$CONFIGURE_STRING \
                  --enable-debug"
```

This will get the source ready to compile CLI or FPM. The first one to compile
is CLI:

```bash
# Configure and compile CLI

./configure \
    $CONFIGURE_STRING \
    --enable-pcntl \
    --with-config-file-path=/etc/php7.0/cli \
    --with-config-file-scan-dir=/etc/php7.0/cli/conf.d

make -j2
make install

# Install config files

cp php.ini-production /etc/php7.0/cli/php.ini
sed -i 's/;date.timezone =.*/date.timezone = Europe\/Brussels/' /etc/php7.0/cli/php.ini
```

Now would be the right time to run ```make test``` to help the PHP project.

Next clean the build and do the same for FPM.

Note that I use ```www-data``` as the user for FPM. This user is available if
you install Nginx or Apache2 on Debian. If you don't have this user, create it
first or use a different user:

```bash
make distclean
./buildconf --force
```

```bash
./configure \
    $CONFIGURE_STRING \
    --with-config-file-path=/etc/php7.0/fpm \
    --with-config-file-scan-dir=/etc/php7.0/fpm/conf.d \
    --disable-cli \
    --enable-fpm \
    --with-fpm-user=www-data \
    --with-fpm-group=www-data

make -j2
make install

# Install config files

cp php.ini-production /etc/php7.0/fpm/php.ini
sed -i 's/;date.timezone =.*/date.timezone = Europe\/Brussels/' /etc/php7.0/fpm/php.ini

cp sapi/fpm/php-fpm.conf.in /etc/php7.0/fpm/php-fpm.conf
sed -i 's#^include=.*/#include=/etc/php7.0/fpm/pool.d/#' /etc/php7.0/fpm/php-fpm.conf

mkdir /etc/php7.0/fpm/pool.d/
cp /usr/local/php7.0/etc/php-fpm.d/www.conf.default /etc/php7.0/fpm/pool.d/www.conf
sed -i 's/listen = 127.0.0.1:9000/listen = 127.0.0.1:9070/g' /etc/php7.0/fpm/pool.d/www.conf

# Enable in RC system (there is an init.d and systemd version available)

cp sapi/fpm/init.d.php-fpm /etc/init.d/php7.0-fpm
chmod +x /etc/init.d/php7.0-fpm

sed -i 's/Provides:          php-fpm/Provides:          php7.0-fpm/' /etc/init.d/php7.0-fpm
sed -i 's#^php_fpm_CONF=.*#php_fpm_CONF=/etc/php7.0/fpm/php-fpm.conf#' /etc/init.d/php7.0-fpm
sed -i 's#^php_fpm_PID=.*#php_fpm_PID=/var/run/php7.0-fpm.pid#' /etc/init.d/php7.0-fpm

update-rc.d php7.0-fpm defaults
```

Now you are ready to use PHP from ```/usr/local/php7.0/bin/php```. You can symlink
the binaries ```php```, ```phpize```, ... to a directory in your path or just
add the entire directory to your path.

```bash
export PATH=$PATH:/usr/local/php7.0/bin:/usr/local/php7.0/sbin
```

## Building extensions

You can now build extra extensions if you like. One extension, opcache, is already
built and you will want to enable this for performance reasons.

```bash
echo "zend_extension=opcache.so" > /etc/php7.0/conf.d/opcache.ini
ln -s /etc/php7.0/conf.d/opcache.ini /etc/php7.0/cli/conf.d/opcache.ini
ln -s /etc/php7.0/conf.d/opcache.ini /etc/php7.0/fpm/conf.d/opcache.ini
```
