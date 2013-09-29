/*
 * filelock.h
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#ifndef FILELOCK_H_
#define FILELOCK_H_
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

#define read_lock(fd) \
                        lock_reg(fd, F_SETLKW, F_RDLCK, 0, SEEK_SET, 0)

#define write_lock(fd) \
                        lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 0)

#define un_lock(fd) \
                        lock_reg(fd, F_SETLKW, F_UNLCK, 0, SEEK_SET, 0)
#endif /* FILELOCK_H_ */
