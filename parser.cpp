//kaleidoscope - expr, proto & funcObj
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
    std::string name;
    public:
        varExprAST(const std::string &name) : name(name) {}
};

class binExprAST : public exprAST
{
    char op;
    //below line is imp
    std::unique_ptr<exprAST> LHS,RHS;
    public:
        //
        binExprAST
        (
            char op,
            std::unique_ptr<exprAST> LHS,
            std::unique_ptr<exprAST> RHS
        ) : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        //
        //what does move do?
};

class callExprAST : public exprAST
{
    std::string callee;
    std::vector<std::unique_ptr<exprAST>> args;
    public:
        callExprAST
        (
            const std::string &callee,
            std::vector<std::unique_ptr<ExprAST>> args
        ) : callee(callee), args(std::move(args)) {}
};