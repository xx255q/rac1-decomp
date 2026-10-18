/* NON_MATCHING func_00125F88 -- src/core/00125630.c
 * Best so far: SIZE ours 1028 / retail 1052, checked 2026-10-18.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   _getRef0 (libmpeg motion-compensation fetch setup), written from the assembly with the
 *   file's MpegDecoder / MpegMbBuffer / MpegMcFetch types (replaces the older functional draft).
 *   Retail re-reads d->mb_buf[d->mb_buf_index] for each use (not a local pointer); keep it that way.
 *   Left: retail spills bx to 0x4($sp) at once and keeps ref in $fp; ours keeps bx in a register.
 *   Loading ref->data into a local early (refdata) came from ~1,500 permuter iterations (14% better).
 *   Retail folds the chroma half-vectors as (dx + (dx<0)) >> 2 directly; see cy/cx.
 */

extern MpegMcFunc D_00132F30_25F88[] __asm__("D_00132F30");
extern MpegMcFunc D_00132F50_25F88[] __asm__("D_00132F50");
/* _getRef0: queue the luma and chroma fetches of one motion-compensated
   prediction for the current macroblock. Written from the retail routine. */
void func_00125F88(struct MpegDecoder *d, struct MpegRefImage *ref, int sfield, int dfield, int yofs, int h, int bx, int by, int dx, int dy, int fieldpred, int avg)
{
  int n = d->mb_buf[d->mb_buf_index].fetch_count;
  u8 *work = d->mc_work;
  struct MpegMcFetch *fy = &d->mb_buf[d->mb_buf_index].luma[n];
  struct MpegMcFetch *fc = &d->mb_buf[d->mb_buf_index].chroma[n];
  int x;
  int y;
  int mbx;
  u8 *refdata;
  int mby;
  int mb;
  int ylow;
  int v;
  int hx;
  int hy;
  int code;
  u8 *dst;
  if (fieldpred)
  {
    y = (dy >> 1) << 1;
  }
  else
  {
    y = dy >> 1;
  }
  y = (y + by) + (yofs + sfield);
  x = (dx >> 1) + bx;
  mbx = x >> 4;
  mby = y >> 4;
  mb = (ref->unk10 * mbx) + mby;
  fy->x = x - (mbx << 4);
  fy->addr = work + ((dfield + yofs) << 5);
  ylow = y - (mby << 4);
  hy = dy & 1;
  hx = dx & 1;
  if (hy)
  {
    if ((ylow + (h << fieldpred)) < 16)
    {
      fy->n0 = h;
      fy->n1 = 0;
    }
    else
    {
      v = ((16 >> fieldpred) - (ylow >> fieldpred)) - 1;
      fy->n0 = v;
      fy->n1 = h - v;
    }
  }
  else
    if ((ylow + (h << fieldpred)) < 17)
  {
    fy->n0 = h;
    fy->n1 = 0;
  }
  else
  {
    v = (16 >> fieldpred) - (ylow >> fieldpred);
    fy->n0 = v;
    fy->n1 = h - v;
  }
  dst = ((u8 *) d->mb_buf[d->mb_buf_index].spr_base) + (n * 0x600);
  code = ((avg << 2) | (hx << 1)) | hy;
  fy->step = 16 << fieldpred;
  fy->p0 = dst + (ylow << 4);
  fy->p1 = (dst + (ylow << 4)) + 0x300;
  {
    int cx;
    int cy;
    int cmbx;
    int cmby;
    int clow;
    int ch;
    int chx;
    int chy;
    int ccode;
    int dxs;
    int dys;
    dxs = dx / 2;
    dys = dy / 2;
    ch = h >> 1;
    if (fieldpred)
    {
      cy = (((dys >> 1) << 1) + (by >> 1)) + ((yofs >> 1) + sfield);
    }
    else
    {
      cy = ((dys >> 1) + (by >> 1)) + ((yofs >> 1) + sfield);
    }
    cx = (dxs >> 1) + (bx >> 1);
    cmby = cy >> 3;
    cmbx = cx >> 3;
    fc->x = cx - (cmbx << 3);
    fc->addr = (work + ((dfield + (yofs >> 1)) << 4)) + 0x200;
    clow = cy - (cmby << 3);
    refdata = ref->data;
    chy = dys & 1;
    chx = dxs & 1;
    if (chy)
    {
      if ((clow + (ch << fieldpred)) < 8)
      {
        fc->n0 = ch;
        fc->n1 = 0;
      }
      else
      {
        v = ((8 >> fieldpred) - (clow >> fieldpred)) - 1;
        fc->n0 = v;
        fc->n1 = ch - v;
      }
    }
    else
      if ((clow + (ch << fieldpred)) < 9)
    {
      fc->n0 = ch;
      fc->n1 = 0;
    }
    else
    {
      v = (8 >> fieldpred) - (clow >> fieldpred);
      fc->n0 = v;
      fc->n1 = ch - v;
    }
    fc->step = 8 << fieldpred;
    ccode = ((avg << 2) | (chx << 1)) | chy;
    d->mb_buf[d->mb_buf_index].luma_func[n] = D_00132F30_25F88[code];
    dst = dst + ((((cmbx - mbx) << 1) + (cmby - mby)) * 0x180);
    fc->p0 = (dst + (clow << 3)) + 0x100;
    fc->p1 = (dst + (clow << 3)) + 0x400;
    d->mb_buf[d->mb_buf_index].luma_ref[n] = refdata + (mb * 0x180);
    d->mb_buf[d->mb_buf_index].chroma_func[n] = D_00132F50_25F88[ccode];
    d->mb_buf[d->mb_buf_index].chroma_ref[n] = refdata + ((mb + ref->unk10) * 0x180);
    d->mb_buf[d->mb_buf_index].fetch_count++;
  }
}
