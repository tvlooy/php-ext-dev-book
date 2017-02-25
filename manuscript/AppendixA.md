# Appendix A

## A complete example script to build your PHP

The variable ```PHP_VERSION``` has to be valid branch name.
The ```PHP_INSTALL_NAME``` can be any name. So, you can have multiple installed
versions of the same PHP version. Eg: with / without debug, more / less extensions
or a different configuration, ...

```bash
#!/bin/bash

# Dependencies for building
#    make autoconf gcc bison g++

# Dependencies for the selected extensions
#    libxml2-dev
#    libbz2-dev
#    libcurl4-openssl-dev
#    libltdl-dev
#    libpng12-dev
#    libjpeg62-turbo-dev
#    libfreetype6-dev
#    libxpm-dev
#    libimlib2-dev
#    libicu-dev
#    libreadline6-dev
#    libxslt1-dev
#    libssl-dev
#    libsystemd-dev

set -e

# Use a release version like 7.0.8 for a stable release
PHP_VERSION=PHP-7.1
PHP_INSTALL_NAME=7.1-latest

FILE_OWNER=`whoami`

TIMEZONE="Europe\/Brussels"
FPM_PORT=9071
FPM_USER=$FILE_OWNER
FPM_GROUP=$FILE_OWNER
APCU_VERSION=5.1.5

sudo rm -rf /etc/php-${PHP_INSTALL_NAME}
sudo rm -rf /usr/local/php-${PHP_INSTALL_NAME}

sudo mkdir -p /etc/php-${PHP_INSTALL_NAME}/conf.d
sudo mkdir -p /etc/php-${PHP_INSTALL_NAME}/{cli,fpm}/conf.d
sudo mkdir /usr/local/php-${PHP_INSTALL_NAME}

sudo chown -R ${FILE_OWNER}:${FILE_OWNER} /etc/php-${PHP_INSTALL_NAME}
sudo chown -R ${FILE_OWNER}:${FILE_OWNER} /usr/local/php-${PHP_INSTALL_NAME}

# Download

if [ ! -d php-src ]; then
    git clone http://github.com/php/php-src.git
fi

cd php-src
git checkout ${PHP_VERSION}

if [ -f Makefile ]; then
    make distclean
fi
git clean -xdf
./buildconf --force

CONFIGURE_STRING="--prefix=/usr/local/php-${PHP_INSTALL_NAME} \
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
                  --with-xsl \
                  --disable-cgi"

# Options for development
CONFIGURE_STRING="$CONFIGURE_STRING \
                  --enable-debug"

# Build FPM

./configure \
    $CONFIGURE_STRING \
    --with-config-file-path=/etc/php-${PHP_INSTALL_NAME}/fpm \
    --with-config-file-scan-dir=/etc/php-${PHP_INSTALL_NAME}/fpm/conf.d \
    --disable-cli \
    --enable-fpm \
    --with-fpm-systemd \
    --with-fpm-user=${FPM_USER} \
    --with-fpm-group=${FPM_GROUP}

make -j`nproc`
make install

# Install config files

cp php.ini-production /etc/php-${PHP_INSTALL_NAME}/fpm/php.ini
sed -i "s/;date.timezone =.*/date.timezone = ${TIMEZONE}/" /etc/php-${PHP_INSTALL_NAME}/fpm/php.ini

cp sapi/fpm/php-fpm.conf.in /etc/php-${PHP_INSTALL_NAME}/fpm/php-fpm.conf
sed -i "s#^include=.*/#include=/etc/php-${PHP_INSTALL_NAME}/fpm/pool.d/#" /etc/php-${PHP_INSTALL_NAME}/fpm/php-fpm.conf

mkdir /etc/php-${PHP_INSTALL_NAME}/fpm/pool.d/
cp /usr/local/php-${PHP_INSTALL_NAME}/etc/php-fpm.d/www.conf.default /etc/php-${PHP_INSTALL_NAME}/fpm/pool.d/www.conf
sed -i "s/listen = 127.0.0.1:9000/listen = 127.0.0.1:${FPM_PORT}/g" /etc/php-${PHP_INSTALL_NAME}/fpm/pool.d/www.conf

sudo tee /etc/systemd/system/php-${PHP_INSTALL_NAME}-fpm.service >/dev/null <<EOF
[Unit]
Description=The PHP FastCGI Process Manager
After=syslog.target network.target

[Service]
Type=notify
PIDFile=/var/run/php-${PHP_INSTALL_NAME}-fpm.pid
ExecStart=/usr/local/php-${PHP_INSTALL_NAME}/sbin/php-fpm --nodaemonize --fpm-config /etc/php-${PHP_INSTALL_NAME}/fpm/php-fpm.conf
ExecReload=/bin/kill -USR2 $MAINPID
PrivateTmp=yes

[Install]
WantedBy=multi-user.target
EOF

# Watch out for "address already in use"
#sudo systemctl enable php-${PHP_INSTALL_NAME}-fpm
#sudo systemctl start php-${PHP_INSTALL_NAME}-fpm

# Cleanup

make distclean
./buildconf --force

# Build CLI

./configure \
    $CONFIGURE_STRING \
    --enable-pcntl \
    --with-config-file-path=/etc/php-${PHP_INSTALL_NAME}/cli \
    --with-config-file-scan-dir=/etc/php-${PHP_INSTALL_NAME}/cli/conf.d

make -j`nproc`
make install

# Install config files

cp php.ini-production /etc/php-${PHP_INSTALL_NAME}/cli/php.ini
sed -i "s/;date.timezone =.*/date.timezone = ${TIMEZONE}/" /etc/php-${PHP_INSTALL_NAME}/cli/php.ini

# Build extensions

cd ..

PATH=/usr/local/php-${PHP_INSTALL_NAME}/bin:/usr/local/php-${PHP_INSTALL_NAME}/sbin:$PATH

# opcache
echo "zend_extension=opcache.so" > /etc/php-${PHP_INSTALL_NAME}/conf.d/opcache.ini
ln -s /etc/php-${PHP_INSTALL_NAME}/conf.d/opcache.ini /etc/php-${PHP_INSTALL_NAME}/cli/conf.d/opcache.ini
ln -s /etc/php-${PHP_INSTALL_NAME}/conf.d/opcache.ini /etc/php-${PHP_INSTALL_NAME}/fpm/conf.d/opcache.ini

# apcu
printf "\n" | pecl install apcu-${APCU_VERSION}
echo "extension=apcu.so" > /etc/php-${PHP_INSTALL_NAME}/conf.d/apcu.ini
ln -s /etc/php-${PHP_INSTALL_NAME}/conf.d/apcu.ini /etc/php-${PHP_INSTALL_NAME}/cli/conf.d/apcu.ini
ln -s /etc/php-${PHP_INSTALL_NAME}/conf.d/apcu.ini /etc/php-${PHP_INSTALL_NAME}/fpm/conf.d/apcu.ini

# Symlink PHP into the path
sudo ln -sf /usr/local/php-${PHP_INSTALL_NAME}/bin/php /usr/bin/php-${PHP_INSTALL_NAME}

pecl install xdebug

# Ready

echo "Don't forget to run 'make test'."
```

## An example script to switch your PHP version

I have a file named ```pe.sh``` in my ```Tools``` directory. I use this script
to set a PHP engine (therefore pe.sh).

I call this script from my ```.bashrc``` to set a default PHP version like this:

```
. ~/Tools/pe.sh 7.1
```

I also alias the script in my ```.bash_aliases``` as ```pe``` like this:

```
alias pe='. ~/Tools/pe.sh'
```

This is the ```pe.sh``` script:

```bash
#!/bin/bash

VERSIONS=($(find /usr/local/ -maxdepth 1 -name 'php-*' | sort | cut -c16-))
VERSION=""

if [ -n "$1" ]; then
    VERSION=$1
fi

if [[ ! " ${VERSIONS[@]} " =~ " ${VERSION} " ]]; then
    PS3="Choose a version: "
    select VERSION in ${VERSIONS[*]}; do
       echo -e "\nSetting to option $REPLY: $VERSION\n"
       /usr/local/php-${VERSION}/bin/php -v
       break
    done
fi

PATH=/usr/local/php-${VERSION}/bin:/usr/local/php-${VERSION}/sbin
PATH=$PATH:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
PATH=$PATH:$HOME/Tools

export PATH="$PATH"
```
