//kaleidoscope - expr, proto & funcObj
class exprAST
{
    public:
        virtual ~exprAST()=default;
        //understanding virtual and default
};

class numExprAST : public exprAST
{
    double val;
    public:
        numExprAST(double val) : val(val) {}
        //what does above line do?
};
