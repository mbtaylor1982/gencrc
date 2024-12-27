/*
 * gencrc
 * ------
 * Simple program to compute CRC32 of a file used for resdmac.
 *
 * Copyright Chris Hooper & Mike Taylor. This program and source may be
 * used and distributed freely, for any purpose which benefits the Amiga
 * community. Commercial use of the binary, source, or algorithms
 * requires prior written or email approval from me.
 * All redistributions must retain this Copyright notice.
 *
 * DISCLAIMER: THE SOFTWARE IS PROVIDED "AS-IS", WITHOUT ANY WARRANTY.
 * THE AUTHOR ASSUMES NO LIABILITY FOR ANY DAMAGE ARISING OUT OF THE USE
 * OR MISUSE OF THIS UTILITY OR INFORMATION REPORTED BY THIS UTILITY.
 *
 *
 * cc -o gencrc gencrc.c gencrc.c
 *
 * ./gencrc resdmac_cfm0_auto.rpd
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "crc32.h"

static uint
show_crc(const char *filename, uint show_filename)
{
    char *buf;
    struct stat st;
    unsigned int fsize;
    FILE *fp;

    if (stat(filename, &st)) {
        printf("%s failed stat\n", filename);
        return (1);
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("%s failed open: %d %s\n", filename, errno, strerror(errno));
        return (1);
    }

    fsize = st.st_size;
    buf = malloc(fsize);
    if (buf == NULL) {
        printf("Failed to allocate %d bytes\n", fsize);
        fclose(fp);
        return (1);
    }

    if (fread(buf, fsize, 1, fp) != 1) {
        printf("%s read failure: %d %s\n", filename, errno, strerror(errno));
        free(buf);
        fclose(fp);
        return (1);
    }

    if (show_filename)
        printf("%s: ", filename);

    printf("0x%08x\n", crc32(0, buf, fsize));

    free(buf);
    fclose(fp);
    return (0);
}

int
main(int argc, char *argv[])
{
    uint arg;
    uint errs = 0;

    if (argc < 2) {
        printf("Supply the filename(s) for CRC\n");
        exit(EXIT_FAILURE);
    }

    for (arg = 1; arg < argc; arg++)
        errs += show_crc(argv[arg], argc > 2);

    if (errs > 0)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}
