
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 * Copyright (C) Winshining
 */


#ifndef _NGX_RTMP_SCRIPT_H_INCLUDED_
#define _NGX_RTMP_SCRIPT_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include "ngx_rtmp.h"


typedef struct {
    u_char                     *ip;
    u_char                     *pos;
    ngx_rtmp_variable_value_t  *sp;

    ngx_str_t                   buf;
    ngx_str_t                   line;

    /* the start of the rewritten arguments */
    u_char                     *args;

    unsigned                    flushed:1;
    unsigned                    skip:1;
    unsigned                    quote:1;
    unsigned                    is_args:1;
    unsigned                    log:1;

    ngx_int_t                   status;
    ngx_rtmp_session_t         *request;
} ngx_rtmp_script_engine_t;


typedef struct {
    ngx_conf_t                 *cf;
    ngx_str_t                  *source;

    ngx_array_t               **flushes;
    ngx_array_t               **lengths;
    ngx_array_t               **values;

    ngx_uint_t                  variables;
    ngx_uint_t                  ncaptures;
    ngx_uint_t                  captures_mask;
    ngx_uint_t                  size;

    void                       *main;

    unsigned                    compile_args:1;
    unsigned                    complete_lengths:1;
    unsigned                    complete_values:1;
    unsigned                    zero:1;
    unsigned                    conf_prefix:1;
    unsigned                    root_prefix:1;

    unsigned                    dup_capture:1;
    unsigned                    args:1;
} ngx_rtmp_script_compile_t;


typedef struct {
    ngx_str_t                   value;
    ngx_uint_t                 *flushes;
    void                       *lengths;
    void                       *values;
} ngx_rtmp_complex_value_t;


typedef struct {
    ngx_conf_t                 *cf;
    ngx_str_t                  *value;
    ngx_rtmp_complex_value_t   *complex_value;

    unsigned                    zero:1;
    unsigned                    conf_prefix:1;
    unsigned                    root_prefix:1;
} ngx_rtmp_compile_complex_value_t;


typedef void (*ngx_rtmp_script_code_pt) (ngx_rtmp_script_engine_t *e);
typedef size_t (*ngx_rtmp_script_len_code_pt) (ngx_rtmp_script_engine_t *e);


typedef struct {
    ngx_rtmp_script_code_pt     code;
    uintptr_t                   len;
} ngx_rtmp_script_copy_code_t;


typedef struct {
    ngx_rtmp_script_code_pt     code;
    uintptr_t                   index;
} ngx_rtmp_script_var_code_t;


typedef struct {
    ngx_rtmp_script_code_pt     code;
    ngx_rtmp_set_variable_pt    handler;
    uintptr_t                   data;
} ngx_rtmp_script_var_handler_code_t;


typedef struct {
    ngx_rtmp_script_code_pt     code;
    uintptr_t                   conf_prefix;
} ngx_rtmp_script_full_name_code_t;


typedef struct {
    ngx_rtmp_script_code_pt     code;
    ngx_array_t                *lengths;
} ngx_rtmp_script_complex_value_code_t;


typedef struct {
    ngx_rtmp_script_code_pt     code;
    uintptr_t                   value;
    uintptr_t                   text_len;
    uintptr_t                   text_data;
} ngx_rtmp_script_value_code_t;


void ngx_rtmp_script_flush_complex_value(ngx_rtmp_session_t *s,
    ngx_rtmp_complex_value_t *val);
ngx_int_t ngx_rtmp_complex_value(ngx_rtmp_session_t *s,
    ngx_rtmp_complex_value_t *val, ngx_str_t *value);
ngx_int_t ngx_rtmp_compile_complex_value(ngx_rtmp_compile_complex_value_t *ccv);
char *ngx_rtmp_set_complex_value_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


ngx_int_t ngx_rtmp_test_predicates(ngx_rtmp_session_t *s,
    ngx_array_t *predicates);
char *ngx_rtmp_set_predicate_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

ngx_uint_t ngx_rtmp_script_variables_count(ngx_str_t *value);
ngx_int_t ngx_rtmp_script_compile(ngx_rtmp_script_compile_t *sc);
u_char *ngx_rtmp_script_run(ngx_rtmp_session_t *s, ngx_str_t *value,
    void *code_lengths, size_t reserved, void *code_values);
void ngx_rtmp_script_flush_no_cacheable_variables(ngx_rtmp_session_t *s,
    ngx_array_t *indices);

void *ngx_rtmp_script_start_code(ngx_pool_t *pool, ngx_array_t **codes,
    size_t size);
void *ngx_rtmp_script_add_code(ngx_array_t *codes, size_t size, void *code);

size_t ngx_rtmp_script_copy_len_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_copy_code(ngx_rtmp_script_engine_t *e);
size_t ngx_rtmp_script_copy_var_len_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_copy_var_code(ngx_rtmp_script_engine_t *e);
size_t ngx_rtmp_script_mark_args_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_start_args_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_complex_value_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_value_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_set_var_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_var_set_handler_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_var_code(ngx_rtmp_script_engine_t *e);
void ngx_rtmp_script_nop_code(ngx_rtmp_script_engine_t *e);


#endif /* _NGX_RTMP_SCRIPT_H_INCLUDED_ */
