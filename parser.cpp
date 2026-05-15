using namespace std;
class exprAST
{
    public:
        virtual ~exprAST()=default;
        //understanding virtual and default
};
/* NumberExprAST class captures the numeric value of the literal as an instance variable. 
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

