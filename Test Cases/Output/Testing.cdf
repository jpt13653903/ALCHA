JedecChain;
 FileRevision(JESD32A);
 DefaultMfr(6E);

 P ActionCode(Cfg)
  Device PartName(10M08DAF484C8GES) Path("output_files/") File("Testing.sof") MfrSpec(OpMask(1));

ChainEnd;

AlteraBegin;
 ChainType(JTAG);
AlteraEnd
