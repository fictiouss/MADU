#include "../../std/int.h"
#include "../../std/char.h"
#include "../../std/def.h"
#include "../../std/attr.h"


typedef struct PACKED _SIMPLE_TEXT_OUTPUT_INTERFACE {
    uint64_t Reset; // Offset 0x0 (skips the first 8 offset)
    MSABI uint64_t (*OutputString)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE* Self, uchar16_t* String); // Offset 8
} SIMPLE_TEXT_OUTPUT_INTERFACE;


typedef struct PACKED {
    uint8_t  _unused_header[64];    // It skips to offset 0x40
    SIMPLE_TEXT_OUTPUT_INTERFACE* conout;
    
    uint8_t  _unused_gap[16];       // It skips the value in between offset 0x40 + 16 (simply, it skips to offset 0x58)
    
    void* run_time_services;     	// Offset 0x58
    void* boot_services;         	// +0x60
    uint64_t config_entry_number;   // +0x68 
    void* config_table;          	// +0x70
} EFI_SYSTEM_TABLE;

// Reserves both variable
static void* image_handle; // Raw 64-bit number
static EFI_SYSTEM_TABLE* system_table; // Pointer to a Structure



// The entry function
MSABI uint64_t efi_main(void* rcx, EFI_SYSTEM_TABLE* rdx) {
    image_handle = rcx;
    system_table = rdx;

    uchar16_t msg[] = { L'H', L'e', L'l', L'l', L'o', L' ', L'W', L'o', L'r', L'l', L'd', L';', L')', L'\r', L'\n', 0 };
    

    system_table->conout->OutputString(system_table->conout, msg);

    // jmp $ in asm
    while(1) {
        __asm__ volatile ("pause");
    }

    return 0;
}