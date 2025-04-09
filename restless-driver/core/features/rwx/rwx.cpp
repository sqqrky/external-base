#ifndef RWX_CPP
#define RWX_CPP

// this code FUCKING SUCKSS FUCKING DELETE THIS PROJECT PLEASE PLEASEPLEASEPLEASEPLEASEPPLEAS
// v - nuh uh

#include <core/features/rwx/rwx.hpp>

[[nodiscard]] auto rwx::read_physical_address(
    std::uint64_t address,
    void* buffer,
    std::uint64_t size,
    size_t* bytes) -> bool {
    MM_COPY_ADDRESS physical_address = { 0 };

    return NT_SUCCESS(MmCopyMemory(buffer, physical_address, size, MM_COPY_MEMORY_PHYSICAL, bytes));
}

[[nodiscard]] auto rwx::write_physical_address(
    std::uint64_t address,
    void* buffer,
    std::uint64_t size,
    size_t* bytes) -> bool
{
    PHYSICAL_ADDRESS address_to_write = { 0 };

    void* mapped_page = MmMapIoSpaceEx(address_to_write, size, PAGE_READWRITE);
    if (!mapped_page) {
        return false;
    }

    *bytes = size;
    memcpy(mapped_page, buffer, size);

    MmUnmapIoSpace(mapped_page, size);
    return true;
}

[[nodiscard]] auto rwx::translate_linear_address(
    std::uint64_t cr3,
    std::uint64_t virtual_address) -> std::uint64_t
{
    cr3 &= ~0xf;

    std::uint64_t page_offset = virtual_address & (~(~0ul << 12));
    std::uint64_t pte = (virtual_address >> 12) & 0x1ffll;
    std::uint64_t pt = (virtual_address >> 21) & 0x1ffll;
    std::uint64_t pd = (virtual_address >> 30) & 0x1ffll;
    std::uint64_t pdp = (virtual_address >> 39) & 0x1ffll;

    SIZE_T read_size = 0;
    UINT64 pdpe = m_pml4_table[pdp];
    read_physical_address((cr3 + (8) * pdp), &pdpe, sizeof(pdpe), &read_size);
    if (~pdpe & (1)) {
        return (0);
    }

    std::uint64_t pde = 0;
    read_physical_address(((pdpe & (~0xfull << 8) & 0xfffffffffull) + 8 * pd), &pde, sizeof(pde), &read_size);
    if (~pde & (1)) {
        return (0);
    }

    if (pde & 0x80) {
        return (pde & (~(0ull) << (42) >> (12))) + (virtual_address & (~(~(0ull) << (30))));
    }

    std::uint64_t pte_address = (0);
    read_physical_address(((pde & (~0xfull << (8)) & (0xfffffffffull)) + (8) * pt), &pte_address, sizeof(pte_address), &read_size);
    if (~pte_address & (1)) {
        return 0;
    }

    if (pte_address & 0x80) {
        return (pte_address & (~0xfull << (8)) & (0xfffffffffull)) + (virtual_address & (~(~(0ull) << (21))));
    }

    virtual_address = (0);
    read_physical_address(((pte_address & (~(0xfull) << (8)) & (0xfffffffffull)) + (8) * pte), &virtual_address, sizeof(virtual_address), &read_size);
    if (!virtual_address) {
        return (0);
    }

    virtual_address &= ((~0xfull << 8) & 0xfffffffffull);
    return virtual_address + page_offset;
}

auto rwx::process_calls(device::requests::rwa x) -> NTSTATUS
{
    if (!x->process_id)
        return (STATUS_UNSUCCESSFUL);

    SIZE_T current_offset = NULL;
    SIZE_T total_size = x->size;
    ULONGLONG dtb{ };
    INT64 current_physical_address = translate_linear_address(dtb, (ULONG64)x->address + current_offset);
    if (!current_physical_address)
        return (STATUS_UNSUCCESSFUL);

    // why the fuck were people creating a separate function for min :skull:
    // v - cause they're stupid

    ULONG64 final_size = min(PAGE_SIZE - (current_physical_address & 0xFFF), total_size);
    SIZE_T bytes_read = (NULL);

    if (x->write) {
        write_physical_address((current_physical_address), (void*)((ULONG64)x->buffer + current_offset), final_size, &bytes_read);
    }
    else {
        read_physical_address((current_physical_address), (void*)((ULONG64)x->buffer + current_offset), final_size, &bytes_read);
    }

    return STATUS_SUCCESS;
}

#endif
                                                                              
 //         ,/(#######%#######%###################%%######################(*      
 //     /##################%########%%%%%%########%%#%#%%%%%%##########%%###//*.  
 //  .((############%%%##%%%%%######((((((#####%###%%######%%%%%%%%####%##((#%((  
 // /###%######%%%%%%%%#%%#%((((////****//((##((#########################%%%##(%. 
 //.#######%##%###%###(###(///**********////////(#((((((########%%%%####%####(*   
 // *%#%%%%%%%%%##(((((////***********************((((((((((((###%%##%%####       
 //  /%%%%%%%%##((((/(/////************,*,,**********//////////(#%#%%%####        
 //   *%#%%%%#(((////////****************,,,,,****************//(##%%####,        
 //    *%%%%%#((((/////********************,,,,******,,********/(((######         
 //     #%%%##((((//***////////(/////***//((((((((((///////*******/(####*
 //     .%%%%((((/(((///((((((((((((//***//((((##(((((////*********/((##          
 //      #%%#((/((((((#(#####*##/(((/**,*////(///(((/((//(((//******/((/,         
 //     ,###(////((((((///(((////////**,***************,,,,,,********/*//((*      
 //    .##((((///*****************//**,,,******,,,,*,,,,,,,,,,********/***/,      
 //    *((((((///*****,**,,****//*/***,,,*****//*****,,,,,,,********/*/***,       
 //     /((((((///***********////(#(//////###///////***************//****,        
 //      ,((((((////********/////////((//*********////********//***//****.        
 //       */(((((((////////////////*///*/////**//////////////////*//(,..          
 //        ,//((((((((//////(((((#((((((///**//((/(////*///////////               
 //             ,((((((((//////(((((/////////************////////*                
 //               ,(((((((((((//(((((/(//*/********//********////                 
 //                .#(((((//////*********************************,                
 //                /(((((///////////*********//******/////********,.              
 //                                                                               