--------------------------------------------------------------------------------
-- Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.1
--  \   \         Application : sch2hdl
--  /   /         Filename : delic_drc.vhf
-- /___/   /\     Timestamp : 03/31/2014 09:31:49
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: C:\Xilinx\14.1\ISE_DS\ISE\bin\nt64\unwrapped\sch2hdl.exe -intstyle ise -family spartan3e -flat -suppress -vhdl delic_drc.vhf -w C:/Users/Luke/Documents/BI-SAP/snake/delic.sch
--Design Name: delic
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

entity klopD_MUSER_delic is
   port ( D  : in    std_logic; 
          E  : in    std_logic; 
          Q1 : out   std_logic; 
          Q2 : out   std_logic);
end klopD_MUSER_delic;

architecture BEHAVIORAL of klopD_MUSER_delic is
   attribute BOX_TYPE   : string ;
   signal XLXN_5   : std_logic;
   signal XLXN_7   : std_logic;
   signal Q1_DUMMY : std_logic;
   signal Q2_DUMMY : std_logic;
   component NAND2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of NAND2 : component is "BLACK_BOX";
   
begin
   Q1 <= Q1_DUMMY;
   Q2 <= Q2_DUMMY;
   XLXI_1 : NAND2
      port map (I0=>E,
                I1=>D,
                O=>XLXN_5);
   
   XLXI_2 : NAND2
      port map (I0=>E,
                I1=>XLXN_5,
                O=>XLXN_7);
   
   XLXI_3 : NAND2
      port map (I0=>Q2_DUMMY,
                I1=>XLXN_5,
                O=>Q1_DUMMY);
   
   XLXI_5 : NAND2
      port map (I0=>XLXN_7,
                I1=>Q1_DUMMY,
                O=>Q2_DUMMY);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity delic is
   port ( CLK : in    std_logic; 
          F2  : out   std_logic; 
          F4  : out   std_logic; 
          F8  : out   std_logic; 
          F16 : out   std_logic);
end delic;

architecture BEHAVIORAL of delic is
   signal XLXN_2  : std_logic;
   signal XLXN_7  : std_logic;
   signal XLXN_9  : std_logic;
   signal XLXN_12 : std_logic;
   component klopD_MUSER_delic
      port ( D  : in    std_logic; 
             E  : in    std_logic; 
             Q1 : out   std_logic; 
             Q2 : out   std_logic);
   end component;
   
begin
   XLXI_6 : klopD_MUSER_delic
      port map (D=>XLXN_9,
                E=>CLK,
                Q1=>F2,
                Q2=>XLXN_9);
   
   XLXI_7 : klopD_MUSER_delic
      port map (D=>XLXN_2,
                E=>XLXN_9,
                Q1=>F4,
                Q2=>XLXN_2);
   
   XLXI_8 : klopD_MUSER_delic
      port map (D=>XLXN_7,
                E=>XLXN_2,
                Q1=>F8,
                Q2=>XLXN_7);
   
   XLXI_9 : klopD_MUSER_delic
      port map (D=>XLXN_12,
                E=>XLXN_7,
                Q1=>F16,
                Q2=>XLXN_12);
   
end BEHAVIORAL;


