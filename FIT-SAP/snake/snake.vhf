--------------------------------------------------------------------------------
-- Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.1
--  \   \         Application : sch2hdl
--  /   /         Filename : snake.vhf
-- /___/   /\     Timestamp : 03/31/2014 09:01:49
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -intstyle ise -family spartan3e -flat -suppress -vhdl C:/Users/Luke/Documents/BI-SAP/snake/snake.vhf -w C:/Users/Luke/Documents/BI-SAP/snake/snake.sch
--Design Name: snake
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

entity snake is
   port ( D  : in    std_logic; 
          E  : in    std_logic; 
          Q1 : out   std_logic; 
          Q2 : out   std_logic);
end snake;

architecture BEHAVIORAL of snake is
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
   XLXI_2 : NAND2
      port map (I0=>E,
                I1=>D,
                O=>XLXN_5);
   
   XLXI_3 : NAND2
      port map (I0=>E,
                I1=>XLXN_5,
                O=>XLXN_7);
   
   XLXI_4 : NAND2
      port map (I0=>Q2_DUMMY,
                I1=>XLXN_5,
                O=>Q1_DUMMY);
   
   XLXI_5 : NAND2
      port map (I0=>XLXN_7,
                I1=>Q1_DUMMY,
                O=>Q2_DUMMY);
   
end BEHAVIORAL;


