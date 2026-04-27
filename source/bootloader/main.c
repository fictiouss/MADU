/* Structure		Offset		Function name
 ==========================================================================

 EFI System Table	0x00		Table header

			0x00 - 0x40 	(Unused)

			0x40 		ConOut

						|->	Offset	Purpose

							0x00	'reset'
							0x08	Prints text
							0x10	Check if chars are supported
							0x18	Get supported text resolutions
							0x20	Change text resolution
							0x28	Text colors
							0x30	'Clean' the screen
							0x38	Move the txt cursor
							0x40	show or hide cursor
							0x48	pointer to current mode data

 			0x48 - 0x58 	(Unused)

  			0x58 		runtime services table

			0x60		Boot services table

						|->	Offset
							0x00 - 0x38 	(Unused)
							0x38			GetMemoryMap
							0x40 - 0x140	(Unused)
							0x140			LocateProtocol
							0xC8 - 0xE8		(unused)
							0xE8			ExitBootServices
							0xF8		-

			0x68		Number of configuration table entries

			0x70		Config table pointer
*/



#include "../../std/int.h"
#include "../../std/char.h"
#include "../../std/def.h"
#include "../../std/attr.h"

// Define this struct so the compiler knows where 'GetMemoryMap' is
typedef struct PACKED {
    uint8_t  unused[56]; // Skip from offset 0x0 upto 0x38

    
    // Offset 0x38
    MSABI uint64_t (*GetMemoryMap)(
        uint64_t* MemoryMapSize,	// parameter 1: pointer to a memory address that holds the the size of memory map
        void* MemoryMap,			// param 2: a know memory location where UEFI will write all entries 
        uint64_t* MapKey,			// param 3: a key that's gonna be used for EXIT_BOOT_SERVICES
        uint64_t* DescriptorSize,	// param 4: the size of each memory entry
        uint32_t* DescriptorVersion	// param 5: as of for now descriptor version is 1
    );
} EFI_BOOT_SERVICES;


typedef struct PACKED stTextOutput {
    uint64_t Reset;
    MSABI uint64_t (*OutputString)(struct stTextOutput* ConsoleOut, uchar16_t* String);
} TextOutput;


typedef struct PACKED {
    uint8_t  unused1[64]; 		// Skip 64 bytes (from offset 0x0 upto 0x40)
    TextOutput* conout;			// Offset 0x40 upto 0x48
    uint8_t  unused[16];		// Skip 16 bytes (0x48 - 0x58)
    void* run_time_services;	// 0x58 - 0x60
    EFI_BOOT_SERVICES* boot_services;	// 0x60 - 0x68
    uint64_t config_entry_number;   	// 0x68 - 0x70
    void* config_table;            	 	// It's a collection of structure (entries) where each contains a GUID (128-bit) and a pointer 
} EFI_SYSTEM_TABLE;



static void* image_handle; 
static EFI_SYSTEM_TABLE* system_table; 
static void* memmap               = (void*)0x220000;	// It's a memory address where the memory map is gonna get stored on
static uint64_t memmapsize         = 32768;				// The maximum size of the memory map (in byte)
// The next 3 are gonna get filled by UEFI
static uint64_t mmapkey            = 0;					
static uint64_t descriptor_size    = 0;
static uint32_t descriptor_version = 0;

MSABI uint64_t efi_main(void* rcx, EFI_SYSTEM_TABLE* rdx) {
    image_handle = rcx;
    system_table = rdx;


    uint64_t status = system_table->boot_services->GetMemoryMap(
        (uint64_t*)&memmapsize, 
        memmap,
        (uint64_t*)&mmapkey,
        (uint64_t*)&descriptor_size,
        &descriptor_version
    );
/*
If UEFI returns:
0 = Success
5 = The buffer that were allocated (memmapsize) is small 
anything else = Failure
*/

    if (status == 5) 
	{
        system_table->conout->OutputString(system_table->conout, L"The buffer is too small\r\n");
    } 
	
	else if (status != 0) 
	{
        system_table->conout->OutputString(system_table->conout, L"Error\r\n");
    } 
	
	else 
	{
        system_table->conout->OutputString(system_table->conout, L"The memory is mapped successfully!\r\n");
    }

    
	while(1) 
	{
        __asm__ volatile ("pause");
    }

    return 0;
}
