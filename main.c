/* 
    This code originates from the Getting started with Raspberry Pi Pico document
    https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
    CC BY-ND Raspberry Pi (Trading) Ltd
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

const uint LED_PIN = 25;

#define quotient 0
#define remainder 1


//27 assembly instructions with no optimizations
//22 bytes in flash
//11 us execution speed with numerator of 5000 and denominator of 3
//The smallest, and the second fastest algorithm. 
//Good candidate for a balance of code size and speed
void methodOne(int num, int denom, int *results){
    results[quotient] = num/denom;
    results[remainder] = num-(results[quotient]*denom);
}

//subtraction method
//38 assembly instructions with no optimization
//22 bytes in flash
//178 us execution speed with numerator of 5000 and denominator of 3
//One of the smallest, but the slowest algorithm

void methodTwo(int num, int denom, int *results){
    results[remainder] = num;
    while(results[remainder] >= denom){
        results[remainder] -= denom;
        results[quotient] += 1;
    }     
}


//26 assembly instructions with no optimizations
//26 bytes in flash
//5 us execution speed with numerator of 5000 and denominator of 3
//The fastest, but not the smallest algorithm
//Simplest to imeplement, Good for when speed is key
void methodThree(int num, int denom, int *results){
    results[quotient] = num/denom;
    results[remainder] = num%denom;
}

//45 assembly instructions with no optimizations
//34 bytes in flash
//113 us execution speed with numerator of 5000 and denominator of 3
//Large and takes a while, but not the slowest
//It seems that I went backwards here.  Compared to the others this doesn't seem to have any redeeming qualities
void methodFour(int num, int denom, int *results){

    for(int i = 0; i < num; i++)
    {
        if(denom*i > num){
            results[quotient] = i-1;
            break;
        }
    }
    results[remainder] = num-(results[quotient]*denom);
}


int main() {
    int num = 5000;
    int denom = 3;
    int methodOneResults[2] = {0,0};
    int methodTwoResults[2] = {0,0};
    int methodThreeResults[2] = {0,0};
    int methodFourResults[2] = {0,0};

    uint64_t initialTime = 0;
    uint64_t timeAfterOne = 0;
    uint64_t timeAfterTwo = 0;
    uint64_t timeAfterThree = 0;
    uint64_t timeAfterFour = 0;

    uint32_t executionTimes[4];

    bi_decl(bi_program_description("PROJECT DESCRIPTION"));
    
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    initialTime = to_us_since_boot(get_absolute_time());
    methodOne(num, denom, methodOneResults);
    timeAfterOne = to_us_since_boot(get_absolute_time());
    methodTwo(num,denom, methodTwoResults);
    timeAfterTwo = to_us_since_boot(get_absolute_time());
    methodThree(num,denom, methodThreeResults);
    timeAfterThree = to_us_since_boot(get_absolute_time());
    methodFour(num,denom,methodFourResults);
    timeAfterFour = to_us_since_boot(get_absolute_time());

    executionTimes[0] = timeAfterOne - initialTime;
    executionTimes[1] = timeAfterTwo - timeAfterOne;
    executionTimes[2] = timeAfterThree - timeAfterTwo;
    executionTimes[3] = timeAfterFour - timeAfterThree;

    printf("Method One: Quotient: %d, Remainder: %d Execution Time: %d us \n",methodOneResults[quotient], methodOneResults[remainder], executionTimes[0]);
    printf("Method Two: Quotient: %d, Remainder: %d Execution Time: %d us \n",methodTwoResults[quotient], methodTwoResults[remainder],executionTimes[1]);
    printf("Method Three: Quotient: %d, Remainder: %d Execution Time: %d us \n",methodThreeResults[quotient], methodThreeResults[remainder], executionTimes[2]);
    printf("Method Four: Quotient: %d, Remainder: %d Execution Time: %d us \n",methodFourResults[quotient], methodFourResults[remainder], executionTimes[3]);
    while(1) {
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
        gpio_put(LED_PIN, 1);
        puts("Hello World\n");
        sleep_ms(1000);
    }
}