


func prim_initial_load


    pload 0				nop			;;// line not counted ( not saved in controller mem)
    halt				nop			;;// line 0


        ; aici sa fie mai multe pload-uri, unul per functie,
        ; la adrese diferite, cu diferenta data de size of functie

        ; fisierul asta sa se genereze automat ?
            ; unde pun lista de include ce functii vreau in instruction mem

endfunc
