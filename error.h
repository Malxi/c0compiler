#ifndef _ERROR_H
#define _ERROR_H

enum errcode{
    char_ill, id_len, intcon_size, charcon_len, miss_sq, miss_dq, strcon_len,

    ep_ident, ep_type, ep_intcon, ep_charcon, ep_const, ep_prog,
    ep_cs, ep_becomes, ep_lparent, ep_rparent, ep_lbrace, ep_rbrace,
    ep_colon, ep_expr, ep_state,

    arr_lbract, arr_rbract, arr_int, arr_index,
    
    id_redel,
    func_ret, func_nret,
    id_ndel, const_assign, if_else, else_errplace,
    case_colon, case_con, case_stat, case_errplace,
    default_colon, default_errplace,
    expr_syntax, div_zero, 
    para_few, para_extra, para_mismatch,
    ep_main
};

enum fatalcode{
    file_in, file_out, cp_arg, out_tab, out_buf
};

enum warningcode{
    of_conversion
};

extern struct  Set Errset;

void error(int n);

void fatal(int n);

void warning(int n);
#endif