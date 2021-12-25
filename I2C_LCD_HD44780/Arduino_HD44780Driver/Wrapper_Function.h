#ifdef __cplusplus
 #define EXTERNC extern "C"
#else
 #define EXTERNC
#endif

    EXTERNC void Wrapper_Delay(uint32_t);
    EXTERNC void Wrapper_delayMicroseconds(uint32_t);
    EXTERNC void Wrapper_WriteOneByte_I2C(uint8_t);

#undef EXTERNC