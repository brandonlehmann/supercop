      r0Sigma1 = r0>>>14
    r3 += wc4567[0]
      r018 = r0>>>18
    ch3 = r2
    ch3 ^= r1
      r041 = r0>>>41
      r0Sigma1 ^= r018
    ch3 &= r0
      r4Sigma0 = r4>>>28
      r0Sigma1 ^= r041
    ch3 ^= r2
      r434 = r4>>>34
  maj2 = r5
  maj2 ^= r4
      r4Sigma0 ^= r434
    r3 += ch3
      r439 = r4>>>39
  r4andr5 = r5
  r4andr5 &= r4
      r4Sigma0 ^= r439
      r3 += r0Sigma1
  maj3 = r6
  maj3 &= maj2
            r2 += wc4567[1]
  r7 += r3
      r3 += r4Sigma0
  maj3 ^= r4andr5
            ch2 = r1
            ch2 ^= r0
          r7Sigma1 = r7>>>14
  r3 += maj3
            ch2 &= r7
              r718 = r7>>>18
              r7Sigma1 ^= r718
          maj2 &= r3
            ch2 ^= r1
              r741 = r7>>>41
              r7Sigma1 ^= r741
          maj2 ^= r4andr5
              r3Sigma0 = r3>>>28
            r2 += ch2
              r2 += r7Sigma1
              r334 = r3>>>34
              r3Sigma0 ^= r334
          r6 += r2
          r2 += maj2
              r339 = r3>>>39
              r3Sigma0 ^= r339
              r2 += r3Sigma0