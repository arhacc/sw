





    asm_code_new_style.asm contains all instructions, not separated by functions, as they should be loaded by accelerator
        it does not contain function arguments



    asm_code_new_style_w_arguments.asm
        contains function arguments, after prun jump address



    functions folder contains all functions for this test program
        + some additional ones
        + some additional cases such as:
            multiple "1" labels defined in different functions
                (should be processed and ok)
            use of macros (here with `)
                decisions are needed as to macro processing
            main calling functions by name not by address
                each function call in main is followed by argument
                    line processing is needed for proper send to axi lite program interface


