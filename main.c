////////////////////////////////////////////////////////////////////////
//** ENGR-2350 Template Project 
//** NAME: Evan Lacey
//** RIN: 662057116
//** This is the base project for several activities and labs throughout
//** the course.  The outline provided below isn't necessarily *required*
//** by a C program; however, this format is required within ENGR-2350
//** to ease debugging/grading by the staff.
////////////////////////////////////////////////////////////////////////

// We'll always add this include statement. This basically takes the
// code contained within the "engr_2350_msp432.h" file and adds it here.
#include "engr2350_msp432.h"

// Add function prototypes here as needed.
void GPIOInit();
void Turn(uint32_t degrees);
void PreProg(uint32_t turns[]);
void CCR_ISR();
void TimerInit();

// Add global variables here as needed.
int32_t enc_total1;
uint32_t turns[6] = {};

int32_t enc_counts_track1;
int32_t enc_counts1;

int32_t sum_wheel_speed1; // store summation of wheel speed (timer counts)
int32_t sum_measurements1; // track number of measurements in the summation variable
int32_t pwm1 = 200; // store the current PWM
int32_t setpoint1 = 65000;
int8_t count1;
Timer_A_UpModeConfig timer;
Timer_A_ContinuousModeConfig time;
Timer_A_CaptureModeConfig timerL;
Timer_A_CaptureModeConfig timerR;
Timer_A_CompareModeConfig CCR1;
int16_t CCR2comp1;
int32_t enc_counts_track1;
uint8_t enc_flag1 = 0;

int32_t enc_counts_track2; // track timer counts since capture event
int32_t enc_counts_track2;
int32_t enc_counts2;
int32_t enc_total2;
int8_t count2;
int32_t sum_wheel_speed2; // store summation of wheel speed (timer counts)
int32_t sum_measurements2; // track number of measurements in the summation variable
int32_t pwm2 = 200; // store the current PWM
int32_t setpoint2= 65000;
Timer_A_CompareModeConfig CCR2;
int16_t CCR2comp2;
int32_t enc_counts_track2;
uint8_t enc_flag2 = 0;

int main( void ) {    /** Main Function ****/
  
    // Add local variables here as needed.

    // We always call the SysInit function first to set up the 
    // microcontroller for how we are going to use it.
    SysInit();
    GPIOInit();
    TimerInit();

    // Place initialization code (or run-once) code here
    PreProg(turns);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
    while( 1 ) {
        // Place code that runs continuously in here
        if(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6)){
            printf("Start\r\n");
            uint8_t x = 0;
            printf("First: %u Second: %u Third %u\r\n",turns[0],turns[1],turns[2]);
            while(x <= 6){
                //If any bumpers are hit, stop car and turn
                if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN2)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7)){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    printf("Degrees: %u\r\n", turns[x]);
                    __delay_cycles(50e6);
                    Turn(turns[x]);
                    __delay_cycles(24e6);
                    x+=1;
                    printf("Block\r\n");
                }else{ //Otherwise drive forward
                    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
                }
            }
        }
    }
}    /** End Main Function ****/   

// Add function declarations here as needed
void GPIOInit(){
    //Pin0 for 0, Pin2 for 25, Pin3 for 45, Pin5 for 75, Pin6 for 90, Pin7 for 110
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsPeripheralModuleFunctionOutputPin( GPIO_PORT_P2 , GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION );
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10,GPIO_PIN4 | GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
}
void Turn(uint32_t degrees){
    float degree1 = ((degrees * 3.1415/180) * (149/2) * 1.55);
    printf("enc_total1: %d, degrees: %f\n", enc_total1, degree1);
    enc_total1 = 0;
    while(enc_total1 < degree1){
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5);
        printf("enc_total1: %d, degrees: %f\r\n", enc_total1, degree1);
    }
    printf("Turned\r\n");
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
    enc_total1 = 0;
}
void PreProg(uint32_t turns[]){
    uint8_t i = 0;
    uint8_t arraycount = 0;
    while(i < 6){
        if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 0;
            arraycount += 1;
            i+=1;
        }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN2)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 25;
            arraycount += 1;
            i+=1;
        }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 45;
            arraycount += 1;
            i+=1;
        }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 75;
            arraycount += 1;
            i+=1;
        }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 90;
            arraycount += 1;
            i+=1;
        }else if(!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7)){
            printf("Pressed\r\n");
            __delay_cycles(24e6);
            turns[arraycount] = 110;
            arraycount += 1;
            i+=1;
        }

    }
}
void TimerInit(){

    timer.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    timer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    timer.timerPeriod = 800;
    timer.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    timer.timerClear = TIMER_A_DO_CLEAR;
    Timer_A_configureUpMode(TIMER_A0_BASE, &timer);

    CCR1.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
    CCR1.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    CCR1.compareValue = 400;
    Timer_A_initCompare(TIMER_A0_BASE, &CCR1);

    CCR2.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
    CCR2.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    CCR2.compareValue = 400;
    Timer_A_initCompare(TIMER_A0_BASE, &CCR2);

    time.clockSource=TIMER_A_CLOCKSOURCE_SMCLK;
    time.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_1;
    time.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_ENABLE;
    Timer_A_registerInterrupt(TIMER_A3_BASE,TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, CCR_ISR);
    Timer_A_configureContinuousMode(TIMER_A3_BASE,&time);

    timerL.captureRegister=TIMER_A_CAPTURECOMPARE_REGISTER_1;
    timerL.captureMode=TIMER_A_CAPTUREMODE_RISING_EDGE;
    timerL.captureInputSelect=TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    timerL.synchronizeCaptureSource=TIMER_A_CAPTURE_SYNCHRONOUS;
    timerL.captureInterruptEnable=TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    Timer_A_initCapture(TIMER_A3_BASE,&timerL);

    timerR.captureRegister=TIMER_A_CAPTURECOMPARE_REGISTER_0;
    timerR.captureMode=TIMER_A_CAPTUREMODE_RISING_EDGE;
    timerR.captureInputSelect=TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    timerR.synchronizeCaptureSource=TIMER_A_CAPTURE_SYNCHRONOUS;
    timerR.captureInterruptEnable=TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    Timer_A_initCapture(TIMER_A3_BASE,&timerR);

    Timer_A_startCounter(TIMER_A0_BASE , TIMER_A_UP_MODE);
    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, CCR_ISR);
    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCR0_INTERRUPT, CCR_ISR);
    Timer_A_startCounter(TIMER_A3_BASE , TIMER_A_CONTINUOUS_MODE);
}
// Add interrupt functions last so they are easy to find

void CCR_ISR() {

    // Wheel 1 Encoder_ISR

    // calc counts between edges
    if (Timer_A_getInterruptStatus(TIMER_A3_BASE) == TIMER_A_INTERRUPT_PENDING) {
        Timer_A_clearInterruptFlag(TIMER_A3_BASE);
        enc_counts_track1 += 65536;
        enc_counts_track2 += 65536;
    }

    // sum with previous results
    if(Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1) & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){
        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
        count1++;
        enc_counts1 = enc_counts_track1 + Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
        enc_counts_track1 = -(Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1));
        sum_measurements1 += enc_counts1;
        if(enc_total1 >= 2){
            if(sum_measurements1/2 > setpoint1){
                pwm1 += 1;
            }
            if((sum_measurements1/2) < setpoint1){
                pwm1 -= 1;
            }
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, pwm1);
            sum_measurements1 = 0;
            count1 = 0;
        }
        enc_total1++;
        enc_flag1 = 1;

    }

    // Wheel 2 Encoder_ISR
    if(Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0) & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){
        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
        count2++;
        enc_counts2 = enc_counts_track2 + Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
        enc_counts_track2 = -(Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0));
        sum_measurements2 += enc_counts2;
        if(enc_total2 >= 2){
            if(sum_measurements2/2 > setpoint2){
                pwm2 += 1;
            }
            if((sum_measurements2/2) < setpoint2){
                pwm2 -= 1;
            }
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, pwm2);
            sum_measurements2 = 0;
            count2 = 0;
        }
        enc_total2 ++;
        enc_flag2 = 1;

    }
}
