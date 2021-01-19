# File System

## Table of Contents

- [File System](#file-system)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Salient features](#salient-features)
  - [Build Instructions](#build-instructions)
  - [Local File System](#local-file-system)
    - [Single Threaded](#single-threaded)
    - [Multi Threaded](#multi-threaded)
  - [File System Over Network](#file-system-over-network)
    - [Server](#server)
    - [Client](#client)

## Introduction

This is an emulation of a simple filesystem for Operating Systems Course (CS330).

We have built a scalable system which supports harddisk size upto 16KB without any changes but the architecture can be extended by allocating more bits for the addresses.

Our filesystem draws inspiration from ext2 system used in linux operating system. It has fixed allocation size implemented by dividing the disk into blocks of 256 Bytes.

Our system can be accessed locally as well as over the network.

To see inner working of the system, refer to the [guide](Guide.pdf).

## Salient features

- Heirarchical indexing of files, stemming from a root directory.
- Unrestricted file size restricted only by disk size.
- Lazy deletion. Only occupied bit is unset and later the space overwritten.
- Little overhead. <1% wasted for File, 3% for Directory.
- Thread Safe

## Build Instructions

1. To make required folders, use:
   ```
   make
   ```
2. To build the local filesystem emulator, use:
   ```
   make filesystem
   ```
3. To build the server version of filesystem emulator, use:
   ```
   make server
   ```
4. To build the client for server version, use:
   `make client`
   > Client is an independent program and does not require access to the main source code.

> `make clean` can be used to remove all binary files.

## Local File System

File System can be used in two modes when run locally.

### Single Threaded

This is the interactive mode which emulates the linux terminal with similar commands.

To run system in single threaded mode, use:

```
./filesystem
```

### Multi Threaded

This mode is used to emulate batch processing with multiple threads.

This mode does not have an interactive window rather users provide scripts containing the commands to be executed. Any number of scripts may be provided, each of which will be executed in a separate thread.

To run system in multi threaded mode provide the scripts as command line arguments as following:

```
./filesystem script1.txt script2.txt
```

## File System Over Network

Our system provides a centralized filesystem which can be accessed by many users simultaneously. Like local single threaded mode it provides an interactive window to access content.

### Server

In our architecture all of the processing is done on the server side with very thin clients. The server is multithreaded with each client allocated a dedicated thread and socket to communicate over.

By default, the server is hosted on port **95**. Being lower order port it requires elevated privileges.

To run the server, use:

```
sudo ./server
```

### Client

The client application is just an Input/Output shell to communicate with the server. The user must provide the IP address of the server to the client.

To run the client application, use:

```
./client
```
