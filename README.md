## LootKit

Loot kit is an opensource project built with Laravel and C.

### Features
- Dashboard
- Stub Builder
- Basic Stealer Agent
- Windows Stub Output
- Optimised Compilating + UPX for tiny binaries!

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
This project requies x86_64-w64-mingw32-gcc compiler and UPX located in /resources/
More information about usage can be found in CompileController.php
