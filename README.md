Run the analysis code:

-for jet and tower efficiency plots (Validation/bin/)

    cmsenv

    scram b -j 8

    jet_effs.exe new /eos/user/s/shoienko/QIEscan_new/
    
- efficiency plots can be made from root(RAW) (Validation/PlotEffs/)
 
      root
   
      .x JetEffs.C
   
 or for EMU
 
    .x JetEffs_emu.C
  
   
