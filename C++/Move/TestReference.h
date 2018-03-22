//**************************************************************
//         Test routine for reference
//**************************************************************

// one rule: lvalue reference bind to lvalue, rvalue reference bind to rvalue.

// function has lvalue reference argument which can only bind to lvalue.
int& func_lv(int&)
{
    int a = 1;
    return a;
}

// function has rvalue reference argument which can only bind to rvalue.
int&& func_rv(int&&)
{
    // this is wrong, a is lvalue cannot bind to return type which is rvalue reference!
    //int a = 1;
    //return a;

    // now a+b is rvalue, can bind to int&&.
    int a = 1;
    int b = 2;
    return a+b;
}

void TestLvalueReference()
{
    int a = 0;
    func_lv(a);
    //func_lv(1);// wrong

    // why this works? func_lv(a) does not have a name, should it be rvalue?
    // rvalue cannot bind to argument of func_lr.
    func_lv(func_lv(a));
    
    // this is certainly wrong.
    //func_lv(std::move(func_lv(a)));

    // wrong
    //func_lv(std::forward<int>(func_lv(a)));

    func_lv(std::forward<int&>(func_lv(a)));

    // wrong
    //func_lv(std::forward<int&&>(func_lv(a)));
}

void TestRvalueReference()
{
    int a = 1;

    //func_rv(a);//wrong, a is lvalue.

    // correct, 1 is rvalue, can bind to int&& rvalue reference
    func_rv(1);

    // why wrong? func_lv(a) is lvalue??????
    //func_rv(func_lv(a));

    func_rv(func_rv(1));
}

void TestReference()
{
    TestLvalueReference();
    TestRvalueReference();
}

