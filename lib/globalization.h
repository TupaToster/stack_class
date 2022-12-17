#define pasta(str) #str
#define header(fileName) pasta(D:\los_projectados\wtfProc\lib\fileName.h)

// @brief names of regs array elements - regs[r*x]
enum reg {

    r0x = 0, ///< Zero register that serves for wrong address
    rax = 1, ///< First reg
    rbx = 2, ///< Second reg
    rcx = 3, ///< Third reg
    rdx = 4  ///< Fourth reg
};

/// @brief Masks for bits in command byte
enum masks {

    MASK_RAM = 1<<7,      ///< Mask for ram access
    MASK_REG = 1<<6,      ///< Mask for reg access
    MASK_IMM = 1<<5,      ///< Mask for immidiate constant
    MASK_CMD = (1<<5) - 1 ///< Mask for actual command number
};

/// @brief Enumerator for command number (generated automatically)
enum CMD {

    #define DEF_CMD(name, num, arg, code) CMD_##name = num,

    #define GENERAL_CMD 1
    #include header(cmd)
    #undef GENERAL_CMD

    #undef DEF_CMD
};
