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
        0x11, 0x61, 0x16, 0x18,
};

/* Modify tonyenc_key to set the secret key */
const u_char tonyenc_key[] = {
        0x9f, 0x49, 0x52, 0x00,
        0x58, 0x9f, 0xff, 0x23,
        0x8e, 0xfe, 0xea, 0xfa,
        0xa6, 0x33, 0xf3, 0xc6,
};


#ifdef PHP_WIN32
#   define TONYENC_RES FILE*
#else
#   define TONYENC_RES FILE*
#endif

zend_op_array *(*old_compile_file)(zend_file_handle *, int);

zend_op_array *cgi_compile_file(zend_file_handle *, int);

int tonyenc_ext_fopen(FILE *, struct stat *, TONYENC_RES *, const char *);

void tonyenc_encode(char *, size_t);

void tonyenc_decode(char *, size_t);


zend_op_array *cgi_compile_file(zend_file_handle *file_handle, int type)
{
    FILE *fp;
    zend_string *opened_path;
    struct stat stat_buf;
    int data_len;
    TONYENC_RES res = 0;

    /* FIXME: If in cli mode with no args */
    if (!strcmp(file_handle->filename, "-"))
        goto final;

    /* Skip if phar */
    int size_phar = sizeof("phar") - 1;
    if (strlen(file_handle->filename) > size_phar && !memcmp(file_handle->filename, "phar", size_phar)) {
        goto final;
    }

    if (file_handle->filename) {
        fp = zend_fopen(file_handle->filename, &opened_path);
        if (fp == NULL)
            goto final;
    }

    fstat(fileno(fp), &stat_buf);
    data_len = stat_buf.st_size;
    if (data_len >= sizeof(tonyenc_header)) {
        char *t = emalloc(sizeof(tonyenc_header));
        size_t read_cnt = fread(t, sizeof(tonyenc_header), 1, fp);
        /* If not the encoded file */
        if (memcmp(t, tonyenc_header, sizeof(tonyenc_header))) {
            efree(t);
            fclose(fp);
            goto final;
        }
        efree(t);
    } else {
        fclose(fp);
        goto final;
    }
    if (tonyenc_ext_fopen(fp, &stat_buf, &res, file_handle->filename))
        goto final;

    if (file_handle->type == ZEND_HANDLE_FP) fclose(file_handle->handle.fp);
    if (file_handle->type == ZEND_HANDLE_FD) close(file_handle->handle.fd);

#ifdef PHP_WIN32
    file_handle->handle.fp = res;
    file_handle->type = ZEND_HANDLE_FP;
#else
    /* pipe() limits to buf size, so we can't use it. */
    file_handle->handle.fp = res;
    file_handle->type = ZEND_HANDLE_FP;
#endif

    /* zend_compile_file() would using the fp, so don't destroy it. */

    final:
    return old_compile_file(file_handle, type);
}


int tonyenc_ext_fopen(FILE *fp, struct stat *stat_buf, TONYENC_RES *res, const char *file_name)
{
    char *p_data;
    size_t data_len;
    size_t write_len;

    data_len = stat_buf->st_size - sizeof(tonyenc_header);
    p_data = (char *) emalloc(data_len);
    fseek(fp, sizeof(tonyenc_header), SEEK_SET);
    fread(p_data, data_len, 1, fp);
    fclose(fp);

    tonyenc_decode(p_data, data_len);


#ifdef PHP_WIN32
    /* tmpfile_s() limits the number of calls, about to 2^32 in win7 */
    if (tmpfile_s(res)) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Failed to create temp file, may be too many open files.\n");
        efree(p_data);
        return -1;
    }

    if (fwrite(p_data, data_len, 1, *res) != 1) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Failed to write temp file.\n");
        efree(p_data);
        fclose(*res);
        return -2;
    }
    rewind(*res);
#else
    /* tmpfile() limits the number of calls, about to 2^32 in Linux */
    *res = tmpfile();
    if (*res == NULL) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Failed to create temp file, may be too many open files.\n");
        efree(p_data);
        return -1;
    }

    if (fwrite(p_data, data_len, 1, *res) != 1) {
        php_error_docref(NULL, E_CORE_ERROR, "tonyenc: Failed to write temp file.\n");
        efree(p_data);
        fclose(*res);
        return -2;
    }
    rewind(*res);
#endif

    efree(p_data);

    return SUCCESS;
}


void tonyenc_encode(char *data, size_t len)
{
    size_t i, p = 0;
    for (i = 0; i < len; ++i) {
        if (i & 1) {
            p += tonyenc_key[p] + i;
            p %= sizeof(tonyenc_key);
            u_char t = tonyenc_key[p];
            data[i] = ~(data[i] ^ t);
        }
    }
}


void tonyenc_decode(char *data, size_t len)
{
    size_t i, p = 0;
    for (i = 0; i < len; ++i) {
        if (i & 1) {
            p += tonyenc_key[p] + i;
            p %= sizeof(tonyenc_key);
            u_char t = tonyenc_key[p];
            data[i] = ~data[i] ^ t;
        }
    }
}
