/// Definitions
enum Token
{
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

/// gettok() - return next tok from std ip
static int gettok()
{
    static int lastCh=' ';

    while(isspace(lastCh)) lastCh=getchar();

    if(isalpha(lastCh))
    {
        identStr=lastCh;
        while(isalnum((lastCh=getchar()))) identStr+=lastCh;
        //why the double bracket? - for precedence
        if(identStr=="def") return tok_def;
        if(identStr=="extern") return tok_extern;
        return tok_ident;
    }

    if(isdigit(lastCh) || lastCh=='.')
    {
        std::string numStr ;
        do  
        {
            numStr+=lastCh;
            lastCh=getchar();
        } while(isdigit(lastCh) || lastCh=='.');
        // this could also process "..." as a number - gotta fix
        numVal=strtod(numStr.c_str(),0);
        //is 0 added above?
        return tok_num;
    }

    if(lastCh=='#')
    {
        //comment until end of line
        do
        lastCh=getchar();
        while (lastCh!=EOF && lastCh!='\n' && lastCh!='\r');
        if (lastCh!=EOF) return gettok();
    }

    if(lastCh==EOF) return tok_eof;

    int thisCh = lastCh;
    lastCh = getchar();
    return thisCh;
}
//gotta dry run