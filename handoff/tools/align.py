# align.py FUNC OBJ : align our compiled function with retail (asm/nonmatchings) by opcode/regs
import sys, re, struct, difflib, glob
import rabbitizer as rz
from elftools.elf.elffile import ELFFile
fn, obj = sys.argv[1], sys.argv[2]
e = ELFFile(open(obj, 'rb'))
text = e.get_section_by_name('.text'); data = text.data()
sym = next(s for s in e.get_section_by_name('.symtab').iter_symbols() if s.name == fn)
ours = [struct.unpack_from('<I', data, sym['st_value'] + i)[0] for i in range(0, sym['st_size'], 4)]
asm = open(glob.glob(f'asm/nonmatchings/*/{fn}.s')[0]).read()
ret = [int(m.group(1)[6:8]+m.group(1)[4:6]+m.group(1)[2:4]+m.group(1)[0:2],16) for m in re.finditer(r'/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/', asm)]
def norm(w):
    ins = rz.Instruction(w, 0, rz.InstrCategory.R5900)
    t = ins.disassemble()
    t = re.sub(r'func_[0-9A-F]+|0x[0-9A-Fa-f]+|-?\d+', '#', t)
    return t
def full(w): return rz.Instruction(w, 0, rz.InstrCategory.R5900).disassemble()
a=[norm(w) for w in ours]; b=[norm(w) for w in ret]
sm=difflib.SequenceMatcher(a=a,b=b,autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print(f'--- {tag} ours[{4*i1:#x}:{4*i2:#x}] retail[{4*j1:#x}:{4*j2:#x}]')
    for k in range(max(i2-i1,j2-j1)):
        o=full(ours[i1+k]) if i1+k<i2 else ''
        r=full(ret[j1+k]) if j1+k<j2 else ''
        print(f'   {o:36} | {r}')
