// basic approximation of dhrystone

#include <stdint.h>

typedef struct {
    int32_t IntComp;
    char    CharComp1;
    char    CharComp2;
    char    StringComp[31];
} RecType, *RecPtr;

RecType PtrGlbMem = {0};
RecType PtrGlbNextMem = {0};
RecPtr PtrGlb = &PtrGlbMem;
RecPtr PtrGlbNext = &PtrGlbNextMem;

int32_t IntGlb1 = 0;
int32_t IntGlb2 = 0;
int32_t IntGlb3 = 0;

char    CharGlb1 = 0;
char    CharGlb2 = 0;

int32_t ArrGlb1[50];
int32_t ArrGlb2[50][50];

void Proc_1(RecPtr PtrParIn);
void Proc_2(int32_t *IntParIO);
void Proc_3(RecPtr *PtrParOut);
void Proc_4(void);
void Proc_5(void);

void Proc_1(RecPtr PtrParIn)
{
    PtrParIn->IntComp = 5;
    PtrParIn->CharComp1 = 'A';
    PtrParIn->CharComp2 = 'B';
}

void Proc_2(int32_t *IntParIO)
{
    int32_t IntLoc;
    IntLoc = *IntParIO + 10;
    *IntParIO = IntLoc + IntGlb1;
}

void Proc_3(RecPtr *PtrParOut)
{
    *PtrParOut = PtrGlb;
}

void Proc_4(void)
{
    int32_t BoolLoc = CharGlb1 == 'A';
    CharGlb2 = BoolLoc ? 'B' : 'C';
}

void Proc_5(void)
{
    CharGlb1 = 'A';
}

int32_t dry_main(void)
{
    int32_t i;
    int32_t IntLoc1 = 0;
    int32_t IntLoc2 = 0;
    int32_t IntLoc3 = 0;
    char CharLoc = 0;

    PtrGlb->IntComp = 0;
    PtrGlb->CharComp1 = 'A';
    PtrGlb->CharComp2 = 'B';

    IntGlb1 = 0;
    IntGlb2 = 0;
    IntGlb3 = 0;

    CharGlb1 = 'A';
    CharGlb2 = 'B';

    for (i = 0; i < 1000000; i++) {
        Proc_5();
        Proc_4();

        IntLoc1 = 2;
        IntLoc2 = 3;
        IntLoc3 = IntLoc1 + IntLoc2;
        Proc_2(&IntLoc3);

        Proc_1(PtrGlb);
        Proc_3(&PtrGlbNext);
    }
}
