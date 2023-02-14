#include  <DSP28x_Project.h>
#include "F2806x_Device.h"
#include "testConfig.h"

#if TEST_CPLUS
class test
{
public:
    test(){}
    ~test(){}
};
#endif


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
    EDIS;


    #if TEST_CPLUS
    test* a = new test();
    delete a;
    #endif

    #if TEST_CAN
    #if TEST_CAN_SEND
    InitECanGpio();
    InitECan();
    GpioDataRegs.GPBTOGGLE.bit.GPIO34=1; //LED�Ʒ�ת
         GpioDataRegs.GPBTOGGLE.bit.GPIO39=1;
    // Transmit Mail Box
    ECanaMboxes.MBOX0.MSGID.all = 0x15100000; // address for MBOX0 (trans)
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;    // 8 bytes data would be sent
    ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;

    // Receive Mail Box
    ECanaMboxes.MBOX16.MSGID.all = 0x15200000; // address for MBOX16 (recei)
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;     //  8 bytes data would be received
    ECanaMboxes.MBOX1.MSGCTRL.bit.RTR = 0;

    #endif
    #endif


    while(1)
    {
     GpioDataRegs.GPBTOGGLE.bit.GPIO34=1; //LED�Ʒ�ת
     GpioDataRegs.GPBTOGGLE.bit.GPIO39=1;
//     volatile struct MBOX* Mailbox;
//         Mailbox = &ECanaMboxes.MBOX0 + 1;
//
//         Mailbox->MDL.all = 0x2333;
//         Mailbox->MDH.all = 0x2333;
//
//         // Send specified mail box
//         ECanaRegs.CANTRS.all = 1 << (1 - 1);
//         while (ECanaRegs.CANTA.all != 0x0001) {};
//
//         ECanaRegs.CANTA.all = 0xFFFF;// clear, by writing 1
     DELAY_US(1000000);

    }
}

