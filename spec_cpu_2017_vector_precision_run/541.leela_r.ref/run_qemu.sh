$share/qemu/experimental/build/qemu-riscv64 -cpu rv64,v=true,zba=true,zbb=true,vlen=128,zicond=true  -plugin $share/qemu/plugin/libinsn.so -d plugin -D qemu_insn.txt ../541.leela_r.origin.elf ref.sgf
