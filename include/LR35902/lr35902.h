#ifndef __LR35902_H__
#define __LR35902_H__

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include "rom.h"
#include "bus.h"

class lr35902 {
public:
    lr35902();
    ~lr35902();

    /********** REGISTERS *********/

    struct AF { //accumulator and flags
        uint8_t a; 
        uint8_t f; 
    };

    struct BC { //user register
        uint8_t b;
        uint8_t c;
    };

    struct DE { //user register
        uint8_t d;
        uint8_t e;
    };

    struct HL { //user register
        uint8_t h;
        uint8_t l;
    };

    AF af;
    BC bc;
    DE de;
    HL hl;

    uint16_t sp; // Stack Pointer
    uint16_t pc; // Program Counter

    enum  FLAGSLR35902 { // Flags for F register
        NA1 = (1 << 0),
        NA2 = (1 << 1),
        NA3 = (1 << 2),
        NA4 = (1 << 3),
        C   = (1 << 4),
        H   = (1 << 5),
        N   = (1 << 6),
        Z   = (1 << 7)
    };

    /******************************/

    uint8_t temp8_1;
    uint8_t temp8_2;
    uint16_t temp16_1;
    uint16_t temp16_2;

    uint8_t* op8_1;
    uint8_t* op8_2;
    uint16_t* op16_1;
    uint16_t* op16_2;

    /******************************/

    struct instruction {
        std::string name;
        std::string op1;
        std::string op2;
        uint8_t (lr35902::*operate)(void) = nullptr;
        uint8_t clock;
    };

    struct register8 {
        std::string name;
        uint8_t *r;
    };

    struct register16 {
        std::string name;
        uint16_t *r;
    };

    /*****************************/

    Bus *bus;

    std::vector<instruction> instlist;
    std::vector<instruction> preflist;
    std::vector<register8> r8list;
    std::vector<register16> r16list;
    std::vector<std::string> op8list;
    std::vector<std::string> op16list;

    void registerBus(Bus *bus);

    uint8_t *getRegister8(std::string name);
    uint16_t *getRegister16(std::string name);
    bool prepareOperand8(std::string opn, uint8_t** op);
    bool prepareOperand16(std::string opn, uint16_t** op);
    bool isOperand8(std::string name);
    bool isOperand16(std::string name);
    void prepareOperands(uint8_t instcode);
    uint16_t getRegister(std::string name);
    uint16_t read16(uint16_t addr);
    uint8_t read8(uint16_t addr);
    void write16(uint16_t addr, uint16_t data);
    void write8(uint16_t addr, uint8_t data);
    
    uint8_t getFlag(FLAGSLR35902 flag);
    void setFlag(FLAGSLR35902 flag, bool value);

    void disassemble(rom_t *data);
    void step(void);
    std::string instruct_to_string(uint8_t inst, uint8_t *data, size_t *i);

    /******************************/

    // Main

    uint8_t LDH(); uint8_t LD(); //load/store
    uint8_t LDw(); uint8_t LDsp();
    uint8_t POP(); uint8_t PUSH();

    uint8_t NOP(); uint8_t STOP(); //misc.
    uint8_t HALT(); uint8_t DI();
    uint8_t EI(); uint8_t PREFIX();

    uint8_t INC(); uint8_t INCw();
    uint8_t DEC(); uint8_t DECw();// Maths & logic
    uint8_t ADD(); uint8_t ADDw();
    
    uint8_t ADC(); 
    uint8_t SUB(); uint8_t SBC();
    uint8_t AND(); uint8_t OR();
    uint8_t XOR(); uint8_t CP();
    uint8_t DAA(); uint8_t SCF();
    uint8_t CPL(); uint8_t CCF();


    uint8_t RLA(); uint8_t RLCA(); // Rotation/Shift
    uint8_t RRA(); uint8_t RRCA();

    uint8_t JR(); uint8_t JP();
    uint8_t RET(); uint8_t RETI();
    uint8_t RST(); uint8_t CALL();

    // Prefix

    uint8_t RLC(); uint8_t RRC(); //Rotation/Shift
    uint8_t RL(); uint8_t RR();
    uint8_t SLA(); uint8_t SRA();
    uint8_t SRL(); uint8_t SWAP();
    uint8_t BIT(); uint8_t RES();
    uint8_t SET();

};

#endif