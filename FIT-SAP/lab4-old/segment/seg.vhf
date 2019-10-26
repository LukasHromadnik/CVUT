--------------------------------------------------------------------------------
-- Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.1
--  \   \         Application : sch2hdl
--  /   /         Filename : seg.vhf
-- /___/   /\     Timestamp : 03/10/2014 12:02:21
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -intstyle ise -family spartan3e -flat -suppress -vhdl H:/sap/lab4/segment/seg.vhf -w H:/sap/lab4/segment/seg.sch
--Design Name: seg
--Device: spartan3e
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity seg is
   port ( A   : in    std_logic; 
          B   : in    std_logic; 
          C   : in    std_logic; 
          D   : in    std_logic; 
          AN1 : out   std_logic; 
          AN2 : out   std_logic; 
          AN3 : out   std_logic; 
          AN4 : out   std_logic; 
          P   : out   std_logic; 
          Q   : out   std_logic; 
          R   : out   std_logic; 
          S   : out   std_logic; 
          T   : out   std_logic; 
          U   : out   std_logic; 
          V   : out   std_logic);
end seg;

architecture BEHAVIORAL of seg is
   attribute BOX_TYPE   : string ;
   component segA
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segB
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segC
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segD
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segE
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segF
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segG
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component GND
      port ( G : out   std_logic);
   end component;
   attribute BOX_TYPE of GND : component is "BLACK_BOX";
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
begin
   XLXI_1 : segA
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>P);
   
   XLXI_2 : segB
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>Q);
   
   XLXI_3 : segC
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>R);
   
   XLXI_4 : segD
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>S);
   
   XLXI_5 : segE
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>T);
   
   XLXI_6 : segF
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>U);
   
   XLXI_7 : segG
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>V);
   
   XLXI_8 : GND
      port map (G=>AN1);
   
   XLXI_11 : VCC
      port map (P=>AN4);
   
   XLXI_12 : VCC
      port map (P=>AN3);
   
   XLXI_13 : VCC
      port map (P=>AN2);
   
end BEHAVIORAL;


