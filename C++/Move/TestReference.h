//**************************************************************
//         Test routine for reference
//**************************************************************

// 1: lvalue reference bind to lvalue, rvalue reference bind to rvalue.
// 2: function return value is lvalue if return type is reference, othewise it is rvalue since it will not have a name.

// function has lvalue reference argument which can only bind to lvalue.
// fuction return value is rvalue.
int func_lv_1(int& x)
{
    return x;
}

// function return value is reference, which is lvalue.
// note return value is actually x, certainly has a name.
int& func_lv_2(int& x)
{
    return x;
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
    func_lv_1(a);
    func_lv_1(a);

    //func_lv_1(a) = 1;// wrong, func_lv_1(a) is rvalue.
    func_lv_2(a) = 1;// correct, func_lv_2(a) is lvalue.

    //func_lv_1(1);// wrong
    //func_lv_2(1);// wrong

    //func_lv_1(func_lv_1(a));// wrong, func_lv_1(a) is rvalue.

    // why this works? func_lv_2(a) does not have a name, should it be rvalue?
    // No. func_lv_2() return type is reference, so it is rlvalue.
    func_lv_1(func_lv_2(a));

    // this is certainly wrong.
    //func_lv_2(std::move(func_lv_2(a)));

    // wrong
    //func_lv_1(std::forward<int>(func_lv_2(a)));

    func_lv_1(std::forward<int&>(func_lv_2(a)));

    // wrong
    //func_lv_1(std::forward<int&&>(func_lv_2(a)));
}

void TestRvalueReference()
{
    int a = 1;

    //func_rv(a);//wrong, a is lvalue.

    // correct, 1 is rvalue, can bind to int&& rvalue reference
    func_rv(1);

    func_rv(func_lv_1(a));// correct. func_lv_1 is rvalue.
    //func_rv(func_lv_2(a));// wrong. func_lv_2 is lvalue.

    func_rv(func_rv(1));
}

void TestReference()
{
    TestLvalueReference();
    TestRvalueReference();
}

