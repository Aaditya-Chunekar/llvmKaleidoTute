enum Token{
    tok_eof=-1,

    // commands
    tok_def=-2,
    tok_extern=-3,

    //primary
    tok_ident=-4,
    tok_num=-5,
};
static std::string identStr;
static double numVal;