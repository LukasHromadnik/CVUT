--------------------------------------------------------------------------------
-- Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.1
--  \   \         Application : sch2hdl
--  /   /         Filename : segment_drc.vhf
-- /___/   /\     Timestamp : 03/02/2014 13:41:40
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: C:\Xilinx\14.1\ISE_DS\ISE\bin\nt64\unwrapped\sch2hdl.exe -intstyle ise -family spartan3e -flat -suppress -vhdl segment_drc.vhf -w C:/Users/Luke/Documents/BI-SAP/segments/segment.sch
--Design Name: segment
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

entity segment is
   port ( A : in    std_logic; 
          B : in    std_logic; 
          C : in    std_logic; 
          D : in    std_logic; 
          P : out   std_logic; 
          Q : out   std_logic; 
          R : out   std_logic; 
          S : out   std_logic; 
          T : out   std_logic; 
          U : out   std_logic; 
          V : out   std_logic);
end segment;

architecture BEHAVIORAL of segment is
   component sega
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segb
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segc
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segd
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component sege
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segf
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
   component segg
      port ( d : in    std_logic; 
             c : in    std_logic; 
             b : in    std_logic; 
             a : in    std_logic; 
             f : out   std_logic);
   end component;
   
begin
   XLXI_1 : sega
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>P);
   
   XLXI_2 : segb
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>Q);
   
   XLXI_3 : segc
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>R);
   
   XLXI_4 : segd
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>S);
   
   XLXI_5 : sege
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>T);
   
   XLXI_6 : segf
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>U);
   
   XLXI_7 : segg
      port map (a=>A,
                b=>B,
                c=>C,
                d=>D,
                f=>V);
   
end BEHAVIORAL;


