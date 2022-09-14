# SL

This program is a clone of the original [SL](https://github.com/mtoyoda/sl).

SL (Steam Locomotive) runs across your terminal when you type "sl" as you meant to type "ls". It's just a joke command, and not useful at all.

After installing the package, you should be able to type `sl` and at least see the following or a random animation:

## Install

### Linux packages

[Packages](https://github.com/alexlren/codetest/releases/) are available in .rpm, .deb or simply as tar.gz

### From sources

Requirements:
- ncurses

Download the [sources](https://github.com/alexlren/codetest/releases/) for a given release, and simply type:

```
make install DESTDIR=/usr/local
```

## Usage

```
Usage: sl [-n N] [-s M] [-v] [-h]
A cruel program made to punish users who mispell ls

Available options:
	-h
		Display this help
	-v
		Show the current version
	-n N
		Select the Nth animation (instead of a random one)
	-s M
		Select the speed mode: 0, 1, 2 (default = 1)
```

## Development

### Requirements

- [fpm](https://github.com/jordansissel/fpm) a package builder
- ncurses devel

### Build

#### Debug mode

```
make
```

#### Release mode

```
make RELEASE=1
```

#### Build with a specific version

```
make VERSION=1.2.3
```

#### Build the animations

```
make animation_headers
```

### Create linux packages

#### .rpm / .deb

```
make rpm VERSION=1.2.3 RELEASE=1
```

```
make deb VERSION=1.2.3 RELEASE=1
```

#### .tar.gz

```
make dist VERSION=1.2.3 RELEASE=1
```
