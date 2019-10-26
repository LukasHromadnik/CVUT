--------------------------------------------------------------------------------
-- Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.1
--  \   \         Application : sch2hdl
--  /   /         Filename : schema.vhf
-- /___/   /\     Timestamp : 03/31/2014 14:16:16
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -intstyle ise -family spartan3e -flat -suppress -vhdl H:/Documents/SAP/sequence2/schema.vhf -w H:/Documents/SAP/sequence2/schema.sch
--Design Name: schema
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

entity D3_8E_MXILINX_schema is
   port ( A0 : in    std_logic; 
          A1 : in    std_logic; 
          A2 : in    std_logic; 
          E  : in    std_logic; 
          D0 : out   std_logic; 
          D1 : out   std_logic; 
          D2 : out   std_logic; 
          D3 : out   std_logic; 
          D4 : out   std_logic; 
          D5 : out   std_logic; 
          D6 : out   std_logic; 
          D7 : out   std_logic);
end D3_8E_MXILINX_schema;

architecture BEHAVIORAL of D3_8E_MXILINX_schema is
   attribute BOX_TYPE   : string ;
   component AND4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4 : component is "BLACK_BOX";
   
   component AND4B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B1 : component is "BLACK_BOX";
   
   component AND4B2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B2 : component is "BLACK_BOX";
   
   component AND4B3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B3 : component is "BLACK_BOX";
   
begin
   I_36_30 : AND4
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D7);
   
   I_36_31 : AND4B1
      port map (I0=>A0,
                I1=>A2,
                I2=>A1,
                I3=>E,
                O=>D6);
   
   I_36_32 : AND4B1
      port map (I0=>A1,
                I1=>A2,
                I2=>A0,
                I3=>E,
                O=>D5);
   
   I_36_33 : AND4B2
      port map (I0=>A1,
                I1=>A0,
                I2=>A2,
                I3=>E,
                O=>D4);
   
   I_36_34 : AND4B1
      port map (I0=>A2,
                I1=>A0,
                I2=>A1,
                I3=>E,
                O=>D3);
   
   I_36_35 : AND4B2
      port map (I0=>A2,
                I1=>A0,
                I2=>A1,
                I3=>E,
                O=>D2);
   
   I_36_36 : AND4B2
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D1);
   
   I_36_37 : AND4B3
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D0);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity schema is
   port ( B8    : in    std_logic; 
          CLEAR : in    std_logic; 
          D0    : out   std_logic; 
          D1    : out   std_logic; 
          D2    : out   std_logic; 
          D3    : out   std_logic; 
          D4    : out   std_logic; 
          D5    : out   std_logic; 
          D6    : out   std_logic; 
          D7    : out   std_logic);
end schema;

architecture BEHAVIORAL of schema is
   attribute BOX_TYPE   : string ;
   attribute HU_SET     : string ;
   signal EN      : std_logic;
   signal MEAL0   : std_logic;
   signal MEAL1   : std_logic;
   signal MEAL2   : std_logic;
   signal MOORE0  : std_logic;
   signal MOORE1  : std_logic;
   signal MOORE2  : std_logic;
   signal XLXN_60 : std_logic;
   component Q0
      port ( q0 : in    std_logic; 
             q1 : in    std_logic; 
             q2 : in    std_logic; 
             en : in    std_logic; 
             f  : out   std_logic);
   end component;
   
   component Q1
      port ( q0 : in    std_logic; 
             q1 : in    std_logic; 
             q2 : in    std_logic; 
             en : in    std_logic; 
             f  : out   std_logic);
   end component;
   
   component Q2
      port ( q0 : in    std_logic; 
             q1 : in    std_logic; 
             q2 : in    std_logic; 
             en : in    std_logic; 
             f  : out   std_logic);
   end component;
   
   component FDC
      generic( INIT : bit :=  '0');
      port ( C   : in    std_logic; 
             CLR : in    std_logic; 
             D   : in    std_logic; 
             Q   : out   std_logic);
   end component;
   attribute BOX_TYPE of FDC : component is "BLACK_BOX";
   
   component D3_8E_MXILINX_schema
      port ( A0 : in    std_logic; 
             A1 : in    std_logic; 
             A2 : in    std_logic; 
             E  : in    std_logic; 
             D0 : out   std_logic; 
             D1 : out   std_logic; 
             D2 : out   std_logic; 
             D3 : out   std_logic; 
             D4 : out   std_logic; 
             D5 : out   std_logic; 
             D6 : out   std_logic; 
             D7 : out   std_logic);
   end component;
   
   component VCC
      port ( P : out   std_logic);
   end component;
   attribute BOX_TYPE of VCC : component is "BLACK_BOX";
   
   attribute HU_SET of XLXI_25 : label is "XLXI_25_0";
begin
   XLXI_1 : Q0
      port map (en=>EN,
                q0=>MEAL0,
                q1=>MEAL1,
                q2=>MEAL2,
                f=>MOORE0);
   
   XLXI_2 : Q1
      port map (en=>EN,
                q0=>MEAL0,
                q1=>MEAL1,
                q2=>MEAL2,
                f=>MOORE1);
   
   XLXI_3 : Q2
      port map (en=>EN,
                q0=>MEAL0,
                q1=>MEAL1,
                q2=>MEAL2,
                f=>MOORE2);
   
   XLXI_8 : FDC
      port map (C=>B8,
                CLR=>CLEAR,
                D=>MOORE0,
                Q=>MEAL0);
   
   XLXI_9 : FDC
      port map (C=>B8,
                CLR=>CLEAR,
                D=>MOORE1,
                Q=>MEAL1);
   
   XLXI_10 : FDC
      port map (C=>B8,
                CLR=>CLEAR,
                D=>MOORE2,
                Q=>MEAL2);
   
   XLXI_25 : D3_8E_MXILINX_schema
      port map (A0=>MEAL0,
                A1=>MEAL1,
                A2=>MEAL2,
                E=>XLXN_60,
                D0=>D0,
                D1=>D1,
                D2=>D2,
                D3=>D3,
                D4=>D4,
                D5=>D5,
                D6=>D6,
                D7=>D7);
   
   XLXI_28 : VCC
      port map (P=>XLXN_60);
   
   XLXI_29 : VCC
      port map (P=>EN);
   
end BEHAVIORAL;


