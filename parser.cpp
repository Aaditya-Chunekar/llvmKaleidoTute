using namespace std;
class exprAST
{
    public:
        virtual ~exprAST()=default;
        //understanding virtual and default
};
/* numExprAST class captures the numeric value of the literal as an instance variable. 
This allows later phases of the compiler to know what the stored numeric value is. */
class numExprAST : public exprAST
{
    double val;
    public:
        numExprAST(double val) : val(val) {}
        //what does above line do?
};

class varExprAST : public exprAST 
{
    string name;
    public:
        varExprAST(const string &name) : name(name) {}
};

class binExprAST : public exprAST
{
    char op;
    //below line is imp
    unique_ptr<exprAST> LHS,RHS;
    public:
        //
        binExprAST
        (
            char op,
            unique_ptr<exprAST> LHS,
            unique_ptr<exprAST> RHS
        ) : op(op), LHS(move(LHS)), RHS(move(RHS)) {}
        //
        //what does move do?
};

class callExprAST : public exprAST
{
    string callee;
    vector<unique_ptr<exprAST>> args;
    public:
        callExprAST
        (
            const string &callee,
            vector<unique_ptr<exprAST>> args
        ) : callee(callee), args(move(args)) {}
};

class protoAST
{
    string name;
    vector<string> args;
    public:
        protoAST (const string &name, vector<string> args)
            : name(name), args(move(args)) {}
        const string &getName() const {return name;}
};

class funcAST
{
    unique_ptr<protoAST> proto;
    unique_ptr<exprAST> body;
    public:
        funcAST(unique_ptr<protoAST> proto, unique_ptr<exprAST> body)
            : proto(move(proto)), body(move(body)) {}
};
//what is make_unique?
//todo: adding header files
//todo: connecting multiple .cpp files

//parser
static int curTok;
static int getNextToken()
{
    return curTok=getTok();
    //how to access this func from lexer.cpp
    //  maybe I need to convert lexer.cpp to lexer.h
}

unique_ptr<exprAST> logError(const char *str)
{
    fprintf(stderr, "Error : %s\n", str);
    return nullptr;
}

unique_ptr<protoAST> logErrorP(const char *str)
{
    logError(str);
    return nullptr;
}

static unique_ptr<exprAST> parseNumExpr()
{
    auto res = make_unique<numExprAST>(numVal);
    getNextToken(); //consume the num
    return move(res);
}

static unique_ptr<exprAST> parseParenExpr()
{
    getNextToken();
    auto v = parseExpr();
    //parseExpr yet to be defined
    if(!v) return nullptr;
    if(curTok!=')') return logError("Expected ')'");
    getNextToken();
    return v;
}

static unique_ptr<exprAST> parseIdentExpr()
{
    string idName = identStr;
    //identStr is in lexer.cpp
    getNextToken(); //eat identifier

    if(curTok!='(')
    return make_unique<varExprAST>(idName);

    getNextToken(); //eat the '('
    vector<unique_ptr<exprAST>> args;
    if(curTok!=')')
    {
        while(true)
        {
            if(auto arg = parseExpr())
            args.push_back(move(arg));
            else return nullptr

            if(curTok==')') break;

            if(curTok!=',') 
            return logError("Expected ')' or ',' in argument list")

            getNextToken();
        }
    }
    getNextToken(); //eat the ')'
    return make_unique<callExprAST>(idName, move(args));
}

static unique_ptr<exprAST> parsePrimary()
{
    switch(curTok)
    {
        default:
            return logError("unknown token when expecting an expression");
        case tok_ident;
            return parseIdentExpr();
        case tok_num:
            return parseNumExpr();
        case '(':
            return parseParenExpr();

    }
}

static map<char,int> binOpPrecedence;

static int getTokPrecedence()
{
    if(!isascii(curTok)) return -1;

    int tokPrec = binOpPrecedence[curTok];
    if(tokPrec<=0) return -1;

    return tokPrec;
}

int main()
{
    binOpPrecedence['<']=10;
    binOpPrecedence['+']=20;
    binOpPrecedence['-']=20;
    binOpPrecedence['*']=40;
}

static unique_ptr<exprAST> parseBinOpRHS(int exprPrec, unique_ptr<exprAST> LHS)
{
    while(true)
    {
        int tokPrec = getTokPrecedence();
        if(tokPrec<exprPrec)return LHS;

        int binOp = curTok;
        getNextToken();

        auto RHS = parsePrimary();
        if(!RHS) return nullptr;

        int nextPrec = getTokPrecedence();
        if(tokPrec<nextPrec)
        {
            RHS=parseBinOpRHS(tokPrec+1, move(RHS));
            if(!RHS) return nullptr;
        }
        LHS=make_unique<binExprAST>(binOp, move(LHS), move(RHS));
    }
}

static unique_ptr<protoAST> parseProto()
{
    if(curTok!=tok_ident)
    return logErrorP("Expected function name in prototype");

    string fnName = identStr;
    getNextToken();

    if(curTok!='(')
    return logErrorP("Expected '(' in prototype");

    vector<string> argNames;
    while(getNextToken()==tok_ident)
    argNames.push_back(identStr);
    if(curTok!=')')
    return logErrorP("Expected ')' in prototype");

    getNextToken();
    return make_unique<protoAST>(fnName, move(argNames));
}

static unique_ptr<funcAST> parseDef()
{
    getNextToken();
    auto proto = parseProto();
    if(!proto) return nullptr;

    if(auto e = parseExpr())
    return make_unique<funcAST>(move(proto), move(e));
    return nullptr;
}