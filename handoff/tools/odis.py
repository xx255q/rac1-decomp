# dis.py FUNC OBJ [start end] : our disassembly with offsets
import sys, struct
import rabbitizer as rz
from elftools.elf.elffile import ELFFile
fn, obj = sys.argv[1], sys.argv[2]
e = ELFFile(open(obj, 'rb'))
data = e.get_section_by_name('.text').data()
sym = next(s for s in e.get_section_by_name('.symtab').iter_symbols() if s.name == fn)
lo = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0
hi = int(sys.argv[4], 16) if len(sys.argv) > 4 else sym['st_size']
for i in range(lo, min(hi, sym['st_size']), 4):
    w = struct.unpack_from('<I', data, sym['st_value'] + i)[0]
    print(f'{i:4x}  {rz.Instruction(w, 0, rz.InstrCategory.R5900).disassemble()}')
