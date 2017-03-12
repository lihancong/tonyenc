/**
 * A high performance and cross-platform encrypt extension for PHP source code.
 *
 * @author:  Tony
 * @site:    lihancong.cn
 * @github:  github.com/lihancong/tonyenc
 */

/* Modify tonyenc_header to disguise your encrypt file */
const u_char tonyenc_header[] = {
        0x66, 0x88, 0xff, 0x4f,
        0x68, 0x86, 0x00, 0x56,
        0x11, 0x16, 0x16, 0x18,
};

/* Modify tonyenc_key to set the secret key */
const u_char tonyenc_key[] = {
        0x9f, 0x49, 0x52, 0x00,
        0x58, 0x9f, 0xff, 0x21,
        0x3e, 0xfe, 0xea, 0xfa,
        0xa6, 0x33, 0xf3, 0xc6,
};


zend_op_array *(*old_compile_file)(zend_file_handle*, int);
zend_op_array *cgi_compile_file(zend_file_handle*, int);
int tonyenc_ext_fopen(FILE*, struct stat*, int*);
void tonyenc_encode(char*, size_t);
void tonyenc_decode(char*, size_t);


zend_op_array *cgi_compile_file(zend_file_handle *file_handle, int type)
{
    FILE *fp;
    zend_string *opened_path;
    struct stat stat_buf;
    int data_len;
    int fd = 0;

    /* FIXME: If in cli mode with no args */
    if (!strcmp(file_handle->filename, "-"))
        goto final;
    if (file_handle->filename) {
        fp = zend_fopen(file_handle->filename, &opened_path);
        if (fp == NULL)
            goto final;
    }

    fstat(fileno(fp), &stat_buf);
    data_len = stat_buf.st_size;
    if (data_len >= sizeof(tonyenc_header)) {
        char *t = emalloc(sizeof(tonyenc_header));
        fread(t, sizeof(tonyenc_header), 1, fp);
        /* If not the encoded file */
        if (memcmp(t, tonyenc_header, sizeof(tonyenc_header))) {
            efree(t);
            fclose(fp);
            goto final;
        }
        efree(t);
    }

    if (tonyenc_ext_fopen(fp, &stat_buf, &fd))
        goto final;

    if (file_handle->type == ZEND_HANDLE_FP) fclose(file_handle->handle.fp);
    if (file_handle->type == ZEND_HANDLE_FD) close(file_handle->handle.fd);

    file_handle->handle.fd = fd;
    file_handle->type = ZEND_HANDLE_FD;

    /*
     * zend_compile_file() would using the fd, so don't destroy it.
     */

    final:return old_compile_file(file_handle, type);
}


int tonyenc_ext_fopen(FILE *fp, struct stat *stat_buf, int *res)
{
    char *p_data;
    size_t data_len;
    int shadow[2] = {};

    data_len = stat_buf->st_size - sizeof(tonyenc_header);
    p_data = (char*)emalloc(data_len);
    fseek(fp, sizeof(tonyenc_header), SEEK_SET);
    fread(p_data, data_len, 1, fp);
    fclose(fp);

    tonyenc_decode(p_data, data_len);

    /* tmpfile() limits the number of calls, so we use pipe() */

    if (pipe(shadow)) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Fail to open pipe, may be too many open files.\n");
        efree(p_data);
        return -1;
    }
    if (write(shadow[1], p_data, data_len) != data_len) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Fail to write pipe.\n");
        efree(p_data);
        close(shadow[1]);
        close(shadow[0]);
        return -2;
    }

    close(shadow[1]);
    efree(p_data);
    *res = shadow[0];

    return SUCCESS;
}


void tonyenc_encode(char *data, size_t len)
{
    tonyenc_decode(data, len);
}


void tonyenc_decode(char *data, size_t len)
{
    size_t i, p = 0;
    for (i = 0; i < len; ++i) {
        if (i & 1) {
            p += tonyenc_key[p] + i;
            p %= sizeof(tonyenc_key);
            u_char t = tonyenc_key[p];
            data[i] ^= t;
        }
    }
}