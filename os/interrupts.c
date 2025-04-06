
// forward declaration of ISR
static void default_isr(void);
extern void pendSV_handler(void);
extern void systick_handler(void);


// vector table
__attribute__( ( used, section( ".vectors" ) ) ) void ( *vectors[] )( void ) =
{
    0,                //  0 stack pointer value (NA)
    default_isr,      //  1 reset (NA)
    default_isr,      //  2 NMI
    default_isr,      //  3 hardFault
    0,                //  4 reserved
    0,                //  5 reserved
    0,                //  6 reserved
    0,                //  7 reserved
    0,                //  8 reserved
    0,                //  9 reserved
    0,                // 10 reserved
    default_isr,      // 11 SVCall
    0,                // 12 reserved
    0,                // 13 reserved
    default_isr,      // 14 pendSV
    systick_handler,  // 15 sysTick
};

void default_isr(void){
    while ( 1 );
}
