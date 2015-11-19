# Compile your own PHP

First get the source. Most Linux distribution provide a source package
for the PHP version they package for you. Eg:

  - php5-dev or php7-dev on Debian
  - php-devel on CentOS

But, you can also get the source from Git and use that. There is a Github mirror:

```
git clone https://github.com/php/php-src.git
cd php-src
```

Checkout the branch you want to work on.

```
git checkout PHP-5.6
git checkout PHP-7.0
git checkout master
```

If you are on master, this is bleeding edge. At the moment it points to 7.1.

## Development version

The minimal instructions to compile a working PHP version are:

```
buildconf --force

./configure \
    --enable-debug \
    --enable-maintainer-zts

make
```

The PHP binary will be in ```./sapi/cli/php```.

For development the ```enable-debug``` makes sure memory leaks are reported.
The ```enable-maintainer-zts``` enables thread safety checks.

You could run make in parallel if you want to use more cores. Eg: ```make -j2```
for 2 using cores. Not that this does not make your build process scale linearly.

## Complete working version

A minimal development version will be good to test PHP core, but will get you stuck
pretty fast if you want to test a framework with the latest PHP release. The
following shows how to compile and install a complete working version. This is
more or less what you get when you would install the Debian package.

Dependencies you need on Debian 8 are:

```
apt-get update
apt-get install -y \
    bison \
    g++ \
    autoconf \
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
    libxslt1-dev
```

Compile and install PHP CLI and FPM.

```
# Prepare installation directories

mkdir -p /etc/php7/conf.d
mkdir -p /etc/php7/{cli,fpm}/conf.d
mkdir /usr/local/php7

# Download

rm -rf php-src
git clone http://github.com/php/php-src.git
cd php-src
git checkout -b php-7.0 php-7.0

./buildconf --force

# Default options

CONFIGURE_STRING="--prefix=/usr/local/php7 \
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
                  --enable-debug \
                  --enable-maintainer-zts"
```

This will get the source ready to compile CLI or FPM. The first one to compile
is ClI:

```
# Configure and compile CLI

./configure \
    $CONFIGURE_STRING \
    --enable-pcntl \
    --with-config-file-path=/etc/php7/cli \
    --with-config-file-scan-dir=/etc/php7/cli/conf.d

make -j2
make install

# Install config files

cp php.ini-production /etc/php7/cli/php.ini
sed -i 's/;date.timezone =.*/date.timezone = Europe\/Brussels/' /etc/php7/fpm/php.ini
```

Next clean the build and do the same for FPM.

Note that I use ```www-data``` as the user for FPM. This user is available if
you install Nginx or Apache2 on Debian. If you don't have this use, create it
first or use a different user:

```
make distclean
./buildconf --force

./configure \
    $CONFIGURE_STRING \
    --with-config-file-path=/etc/php7/fpm \
    --with-config-file-scan-dir=/etc/php7/fpm/conf.d \
    --disable-cli \
    --enable-fpm \
    --with-fpm-user=www-data \
    --with-fpm-group=www-data

make -j2
make install

# Install config files

cp php.ini-production /etc/php7/fpm/php.ini
sed -i 's/;date.timezone =.*/date.timezone = Europe\/Brussels/' /etc/php7/cli/php.ini

cp sapi/fpm/php-fpm.conf.in /etc/php7/fpm/php-fpm.conf
sed -i 's#^include=.*/#include=/etc/php7/fpm/pool.d/#' /etc/php7/fpm/php-fpm.conf

mkdir /etc/php7/fpm/pool.d/
cp /usr/local/php-${PHP_VERSION}/etc/php-fpm.d/www.conf.default /etc/php7/fpm/pool.d/www.conf
sed -i 's/listen = 127.0.0.1:9000/listen = 127.0.0.1:9070/g' /etc/php7/fpm/pool.d/www.conf

# Enable in RC system (there is an init.d and systemd version available)

cp sapi/fpm/init.d.php-fpm /etc/init.d/php7-fpm
chmod +x /etc/init.d/php7-fpm

sed -i 's/Provides:          php-fpm/Provides:          php7-fpm/' /etc/init.d/php7-fpm
sed -i 's#^php_fpm_CONF=.*#php_fpm_CONF=/etc/php7/fpm/php-fpm.conf#' /etc/init.d/php7-fpm
sed -i 's#^php_fpm_PID=.*#php_fpm_PID=/var/run/php7-fpm.pid#' /etc/init.d/php7-fpm

update-rc.d php7-fpm defaults
```

Now you are ready to use PHP from ```/usr/local/php7/bin/php```. You can symlink
the binaries ```php```, ```phpize```, ... to a directory in your path or just
add the entire directory to your path.

```
export PATH=$PATH:/usr/local/php7/bin:/usr/local/php7/sbin
```

## Build extensions

You can now build extra extensions if you like. One extension, opcache, is already
built and you will want to enable this for performance reasons.

```
echo "zend_extension=opcache.so" > /etc/php7/conf.d/opcache.ini
ln -s /etc/php7/conf.d/opcache.ini /etc/php7/cli/conf.d/opcache.ini
ln -s /etc/php7/conf.d/opcache.ini /etc/php7/fpm/conf.d/opcache.ini
```
