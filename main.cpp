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
    //LED1�˿�����
    GpioCtrlRegs.GPBMUX1.bit.GPIO34=0;//����Ϊͨ��GPIO����
    GpioCtrlRegs.GPBDIR.bit.GPIO34=1;//����GPIO����Ϊ���
    GpioCtrlRegs.GPBPUD.bit.GPIO34=0;//ʹ��GPIO��������

    //LED2�˿�����
    //LED1�˿�����
    GpioCtrlRegs.GPBMUX1.bit.GPIO39=0;//����Ϊͨ��GPIO����
    GpioCtrlRegs.GPBDIR.bit.GPIO39=1;//����GPIO����Ϊ���
    GpioCtrlRegs.GPBPUD.bit.GPIO39=0;//ʹ��GPIO��������
    test* a = new test();
    delete a;
    EDIS;
    while(1)
    {
     GpioDataRegs.GPBTOGGLE.bit.GPIO34=1; //LED�Ʒ�ת
     GpioDataRegs.GPBTOGGLE.bit.GPIO39=1;
     DELAY_US(100000);

    }
}

