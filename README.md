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
  - [Thread Safety](#thread-safety)
    - [File Modes](#file-modes)
    - [Locks](#locks)

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

## Thread Safety

As our server is multithread capable we need to make our operations thread safe to avoid corruption of file system.

We achieve this using two strategies.

### File Modes

Files can be opened in two levels of permissions.
Files can be opened in read only mode in which user cannot modify the content of the file. This mode allows multiple users to read the file at the same time. We maintain the number of users reading the file in the header of the first block of the file.

Write mode allows both read and write operations. File needs to be opened in this mode to be modified. This mode restricts the access to the file to one user only. In addition user cannot open file in write mode if someone else has already opened it.

Directories only have a single mode unused/used. When some file is open in any mode all the folders in path from file to root directory are marked as used. This prevents deleting or moving any of these folders.

Similarly delete and move operations are disabled for the opened file.

### Locks

We have used 3 mutex locks to synchronize entire file system.

Fist one is used to make the access to the physical file atomic operations. This makes sure that the there are no errors when OS tries to seek to different parts of the file as we need random I/O. Our file perations are designed in a way that we need access to physical file for very small propotion of the time needed by the functions so locking does not affect performance for small number of users at a time. This feature essentially emulates single read/write head on disk.

Second lock is used to make our block allocation atomic. It is the biggest potential fail point where data in our file system can corrupt, if 2 or more files are allocated the same block in memory.

Last lock is used to make the the file mode operations (`get_mode`, `set_mode`, `clear_mode`) atomic. This lock ensures that information about a file's mode is up to date and stable while the mode values are being changed.
