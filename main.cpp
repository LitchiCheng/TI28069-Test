#include  <DSP28x_Project.h>

class test
{
public:
    test(){}
    ~test(){}
};


int main(void)
{
    InitSysCtrl();  //Step1
    EALLOW;
    //LED1端口配置
    GpioCtrlRegs.GPBMUX1.bit.GPIO34=0;//设置为通用GPIO功能
    GpioCtrlRegs.GPBDIR.bit.GPIO34=1;//设置GPIO方向为输出
    GpioCtrlRegs.GPBPUD.bit.GPIO34=0;//使能GPIO上拉电阻

    //LED2端口配置
    //LED1端口配置
    GpioCtrlRegs.GPBMUX1.bit.GPIO39=0;//设置为通用GPIO功能
    GpioCtrlRegs.GPBDIR.bit.GPIO39=1;//设置GPIO方向为输出
    GpioCtrlRegs.GPBPUD.bit.GPIO39=0;//使能GPIO上拉电阻
    test* a = new test();
    delete a;
    EDIS;
    while(1)
    {
     GpioDataRegs.GPBTOGGLE.bit.GPIO34=1; //LED灯翻转
     GpioDataRegs.GPBTOGGLE.bit.GPIO39=1;
     DELAY_US(100000);

    }
}

