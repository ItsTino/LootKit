## LootKit

Loot kit is an opensource project built with Laravel and C.

### Features
- Dashboard
- Stub Builder
- Basic Stealer Agent

## Installation Instructions

**This Project uses a local SQLite database!!**


1. Clone this repo directly, or fork it to your own account
```git clone git@github.com:ItsTino/LootKit.git```

**This Project requires x86_64-w64-mingw32-gcc!!**

```
sudo apt install mingw-w64
```
Modifying this line in CompileController.php if necessary
```
$process->setEnv(['PATH' => '/usr/lib/gcc/x86_64-w64-mingw32/12-win32:/usr/lib/gcc/x86_64-w64-mingw32/12-posix:' . getenv('PATH')]);
```


### Disclaimer
It is a terrible idea to use this for crime.

It is a good idea to use this project as a learning opportunity

# Warning!
This application requieres a complicated compiler setup for the stub builder when targetting Windows.

To Configure Curl

Download the most recent curl tar.gz from [curl.se](https://curl.se/download)
```
wget URL
tar -xvf curl_file.tar.gz

```

Download most recent OpenSSL source tar.gz from (openssl.org)[https://www.openssl.org/source/]
```
wget URL
tar -xvf openssl.tar.gz
cd openssl_dir

./Configure mingw64 --cross-compile-prefix=x86_64-w64-mingw32- --prefix=/openssl_build
make
make install
```
Now you must rename openssl_build/lib64 to openssl_build/lib
Navigate to curl source folder

```
./configure --host=x86_64-w64-mingw32 --prefix=/path/to/libcurl/output/folder --with-ssl=openssl_build_dir
make
make install
```
